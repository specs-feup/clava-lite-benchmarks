DEFAULT_APP = disparity
DEFAULT_INPUT = sim_fast
DEFAULT_N=10
DEFAULT_TARGET=x86_64
DEFAULT_X86_64_COMPILER = gcc
DEFAULT_ARM_COMPILER = /tools/Xilinx/Vitis/2024.2/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-gcc

APP ?= $(DEFAULT_APP)
INPUT ?= $(DEFAULT_INPUT)
N ?= $(DEFAULT_N)
TARGET ?= $(DEFAULT_TARGET)
X86_64_COMPILER ?= $(DEFAULT_X86_64_COMPILER)
ARM_COMPILER ?= $(DEFAULT_ARM_COMPILER)

# General rules for any CortexSuite-Vision benchmark
ifeq ($(TARGET),x86_64)
    CC = $(X86_64_COMPILER)
else ifeq ($(TARGET),aarch64)
    CC = $(ARM_COMPILER)
else
    $(error Unknown TARGET '$(TARGET)'; must be 'x86' or 'arm')
endif

ifeq ($(APP), $(notdir $(CURDIR)))
    BASEFOLDER := .
else ifeq ($(shell [ -d ./$(APP) ] && echo yes),yes)
    BASEFOLDER := ./$(APP)
else ifeq ($(shell [ -d ./data ] && echo yes),yes)
    BASEFOLDER := .
else
    $(error Could not find source folder for APP '$(APP)'; expected ./$(APP) or the current directory to contain ./data)
endif

CFLAGS_UNSAFE = -fno-omit-frame-pointer
CFLAGS_PROF = -fno-inline -fno-inline-functions -fno-inline-functions-called-once -fno-default-inline -fno-optimize-sibling-calls
CFLAGS_PERF = -w -O3 -g
CFLAGS = $(CFLAGS_PERF) $(CFLAGS_PROF) #$(CFLAGS_UNSAFE)
LDFLAGS = -lm
SRC := $(wildcard $(BASEFOLDER)/*.c)
OBJ = $(SRC:.c=.o)
ELFNAME=$(APP)-$(INPUT)-$(TARGET)
ELF=$(ELFNAME).elf

ifeq ($(APP),pca)
	ARGS = $(BASEFOLDER)/data/sat.trn 4435 37 $(INPUT)
else
	ARGS = $(BASEFOLDER)/data/$(INPUT)
endif

CMD = ./$(ELF) $(ARGS)

# Targets
$(ELF):
	@echo "Building $(ELF) for APP=$(APP), INPUT=$(INPUT), TARGET=$(TARGET)"
	@$(CC) $(CFLAGS) -o $(ELF) $(SRC) -D$(INPUT) -DGENERATE_OUTPUT -DCHECK $(LDFLAGS)
	@echo "Build complete: $(ELF) created"

.PHONY: all clean help run exectime-time allinputs exectime-perf profiling-perf exectime-time-allinputs exectime-perf-allinputs

all: $(ELF)

help:
	@echo "Usage: make [option] [VARIABLE=value]"
	@echo ""
	@echo "Options:"
	@echo "  all                      Build an application based on the APP, INPUT, and TARGET variables"
	@echo "  clean                    Remove build artifacts"
	@echo "  help                     Show this help message"
	@echo "  run                      Run an application based on the APP, INPUT, and TARGET variables"
	@echo "  run-allinputs            Run an application based on the APP and TARGET variables on all inputs in the data folder"
	@echo "  exectime-time            Run an app N times and measure ms time using /usr/bin/time"
	@echo "  exectime-time-allinputs  Run an app N times on all inputs in data folder using /usr/bin/time"
	@echo "  exectime-perf            Run an app N times and measure time using perf stat"
	@echo "  exectime-perf-allinputs  Run an app N times on all inputs in data folder using perf stat"
	@echo "  profiling-perf           Profile an app and generate a call graph using perf"
	@echo ""
	@echo "Settable variables:"
	@echo "  APP                Application name           Options: disparity, localization, mser, multi-ncut, pca, sift, stitch, svm, texture-synthesis, tracking (default: $(DEFAULT_APP))"
	@echo "  INPUT              Input dataset              Options: sim, sim_fast, (other benchmark-dependent inputs) (default: $(DEFAULT_INPUT))"
	@echo "  TARGET             Target architecture        Options: x86_64, aarch64 (default: $(DEFAULT_TARGET))"
	@echo "  N                  Number of runs for timing  Options: N > 0 (default: $(DEFAULT_N))"
	@echo "  X86_64_COMPILER    Compiler for x86_64        (default: $(DEFAULT_X86_64_COMPILER))"
	@echo "  ARM_COMPILER       Compiler for ARM           (default: $(DEFAULT_ARM_COMPILER))"
	@echo ""
	@echo "Examples:"
	@echo "  make APP=disparity INPUT=sim_fast TARGET=x86"
	@echo "  make exectime-time N=100 APP=disparity INPUT=sim_fast TARGET=x86"
	@echo "  make clean"

run: $(ELF)
	@echo "Running $(CMD)"
	@$(CMD)

run-allinputs: $(ELF)
	@echo "Running $(CMD) on all inputs in: $(BASEFOLDER)/data"
	@for dir in $(BASEFOLDER)/data/*/; do \
		input_name=$$(basename "$$dir"); \
		if [ "$$input_name" = "GroundData" ]; then \
			echo "Skipping $$input_name"; \
			continue; \
		fi; \
		echo "Running with input: $$input_name"; \
		$(MAKE) run INPUT=$$input_name; \
	done; \
	echo "All inputs processed."

exectime-time: $(ELF)
	@echo "Running with millisecond timing $N times on: $(CMD)"
	@total=0; count=0; \
	for i in $$(seq 1 $(N)); do \
		echo "Run $$i/$(N)..."; \
		start=$$(date +%s%3N 2>/dev/null || echo "0"); \
		if [ "$$start" = "0" ]; then \
			echo "Warning: 'date +%s%3N' not supported, cannot measure milliseconds"; \
			exit 1; \
		fi; \
		if [ $$i -eq 1 ]; then \
			$(CMD); \
		else \
			$(CMD) > /dev/null; \
		fi; \
		end=$$(date +%s%3N 2>/dev/null || echo "0"); \
		if [ "$$end" = "0" ]; then \
			echo "Warning: 'date +%s%3N' not supported, cannot measure milliseconds"; \
			exit 1; \
		fi; \
		elapsed=$$((end - start)); \
		if [ "$$elapsed" -ge 0 ]; then \
			total=$$((total + elapsed)); \
			count=$$((count + 1)); \
		else \
			echo "Warning: Invalid elapsed time on run $$i"; \
		fi; \
	done; \
	if [ $$count -gt 0 ]; then \
		avg=$$(echo "scale=2; $$total / $$count" | bc); \
		echo "Average elapsed time over $$count runs: $$avg ms"; \
	else \
		echo "No valid timings collected."; \
	fi

exectime-time-allinputs: $(ELF)
	@echo "Running with millisecond timing $$N times on all inputs in: $(BASEFOLDER)/data"
	@total=0; count=0; \
	for dir in $(BASEFOLDER)/data/*/; do \
		input_name=$$(basename "$$dir"); \
		if [ "$$input_name" = "GroundData" ]; then \
			echo "Skipping $$input_name"; \
			continue; \
		fi; \
		echo "Running with input: $$input_name"; \
		$(MAKE) exectime-time INPUT=$$input_name; \
	done; \
	echo "All inputs processed."

exectime-perf: $(ELF)
	@echo "Running perf stat $N times on: $(CMD)"
	@total=0; count=0; \
	for i in $$(seq 1 $(N)); do \
		echo "Run $$i/$(N)..."; \
		if [ $$i -eq 1 ]; then \
			out=$$(perf stat -e task-clock -- $(CMD) 2>&1); \
			# Separate stdout and stderr: stdout before first perf line \
			echo "$$out" | sed '/^ Performance counter stats/d'; \
		else \
			out=$$(perf stat -e task-clock -- $(CMD) 2>&1 >/dev/null); \
		fi; \
		time_ms=$$(echo "$$out" | grep "task-clock" | awk '{print $$1}' | tr -d ','); \
		if echo "$$time_ms" | grep -qE '^[0-9]+(\.[0-9]+)?$$'; then \
			total=$$(echo "$$total + $$time_ms" | bc); \
			count=$$((count + 1)); \
		else \
			echo "Warning: Could not parse task-clock in run $$i"; \
		fi; \
	done; \
	if [ $$count -gt 0 ]; then \
		avg=$$(echo "scale=2; $$total / $$count" | bc); \
		echo "Average task-clock over $$count runs: $$avg ms"; \
	else \
		echo "No valid task-clock times collected."; \
	fi

exectime-perf-allinputs: $(ELF)
	@echo "Running perf stat $$N times on all inputs in: $(BASEFOLDER)/data"
	@total=0; count=0; \
	for dir in $(BASEFOLDER)/data/*/; do \
		input_name=$$(basename "$$dir"); \
		if [ "$$input_name" = "GroundData" ]; then \
			echo "Skipping $$input_name"; \
			continue; \
		fi; \
		echo "Running with input: $$input_name"; \
		$(MAKE) exectime-perf INPUT=$$input_name; \
	done; \
	echo "All inputs processed."

profiling-perf: $(ELF)
	@echo "Running perf on: $(CMD)"
	@sudo perf record -F 72000 -g --call-graph dwarf --all-user -- $(CMD)
	@sudo perf script > $(ELFNAME).perf
	@sudo perf report --stdio --sort symbol --percent-limit 0 > $(ELFNAME)-callgraph.txt
	@sudo perf report --stdio --call-graph=folded > $(ELFNAME)-callgraph-folded.txt
	@echo "perf output saved to $(ELFNAME).perf, $(ELFNAME)-callgraph.txt and $(ELFNAME)-callgraph-folded.txt"

	@echo "Checking if perf2csv.py is in PATH..."
	@if command -v perf2csv.py >/dev/null 2>&1; then \
		echo "perf2csv.py found, generating CSV profile..."; \
		perf2csv.py $(ELFNAME)-callgraph.txt $(ELFNAME)-profile.csv; \
		echo "CSV profile generated: $(ELFNAME)-profile.csv"; \
	else \
		echo "perf2csv.py is missing from PATH, skipping"; \
		echo "Get the script from https://github.com/tiagolascasas/wonderland/PerfTools and put it in your PATH"; \
	fi
	@echo ""
	@echo "Checking if perf2dotfile.py is in PATH..."
	@if command -v perf2dotfile.py >/dev/null 2>&1; then \
		echo "perf2csv.py found, generating CSV profile..."; \
		perf2dotfile.py $(ELFNAME)-callgraph-folded.txt $(ELFNAME)-callgraph.dot -f; \
		echo "Dotfile generated: $(ELFNAME)-callgraph.dot"; \
		if command -v dot >/dev/null 2>&1; then \
			echo "Generating SVG from dotfile..."; \
			dot -Tsvg $(ELFNAME)-callgraph.dot -o $(ELFNAME)-callgraph.svg; \
			echo "SVG generated: $(ELFNAME)-callgraph.svg"; \
			echo "Generating PNG from dotfile..."; \
			dot -Tpng $(ELFNAME)-callgraph.dot -o $(ELFNAME)-callgraph.png; \
			echo "PNG generated: $(ELFNAME)-callgraph"; \
		else \
			echo "Dot command not found, skipping SVG generation"; \
		fi; \
	else \
		echo "perf2dotfile.py is missing from PATH, skipping"; \
		echo "Get the script from https://github.com/tiagolascasas/wonderland/PerfTools and put it in your PATH"; \
	fi
	@echo ""
	@echo "Checking if both stackcollapse-perf.pl and flamegraph.pl are in PATH..."
	@if command -v stackcollapse-perf.pl >/dev/null 2>&1 && command -v flamegraph.pl >/dev/null 2>&1; then \
		echo "Both scripts found, generating flamegraph..."; \
		stackcollapse-perf.pl $(ELFNAME).perf > $(ELFNAME).perf.folded; \
		flamegraph.pl --title "CPU Flamegraph" --countname=Samples --width 1800 --colors hot --minwidth 0.5 $(ELFNAME).perf.folded > $(ELFNAME)-flamegraph.svg; \
		echo "Flamegraph generated: $(APP)_flamegraph.svg"; \
	else \
		echo "One or both scripts are missing from PATH, skipping"; \
		echo "Get the scripts from https://github.com/brendangregg/FlameGraph and put them in your PATH"; \
	fi
	@rm -f $(ELFNAME).perf $(ELFNAME).perf.folded perf.data perf.data.old $(ELFNAME)-callgraph.txt $(ELFNAME)-callgraph-folded.txt

clean:
	@rm -f *.elf out.perf out.folded perf.data perf.data.old *-callgraph.txt $(OBJ) *.svg *.csv *.dot *.png

