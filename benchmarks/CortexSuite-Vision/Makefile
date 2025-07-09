DEFAULT_APP = disparity
DEFAULT_INPUT = sim_fast
DEFAULT_N=10
DEFAULT_TARGET=x86_64
DEFAULT_X86_64_COMPILER = clang
DEFAULT_ARM_COMPILER = /tools/Xilinx/Vitis/2024.2/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-gcc

APP ?= $(DEFAULT_APP)
INPUT ?= $(DEFAULT_INPUT)
N ?= $(DEFAULT_N)
TARGET ?= $(DEFAULT_TARGET)
X86_64_COMPILER ?= $(DEFAULT_X86_64_COMPILER)
ARM_COMPILER ?= $(DEFAULT_ARM_COMPILER)

# General rules for any CortexSuite-Vision benchmark
ifeq ($(TARGET),x86_64)
    CC = X86_64_COMPILER
else ifeq ($(TARGET),arm)
    CC = $(ARM_COMPILER)
else
    $(error Unknown TARGET '$(TARGET)'; must be 'x86' or 'arm')
endif

CFLAGS = -w -O3 -fno-inline -fno-inline-functions -fno-inline-functions-called-once -fno-default-inline -fno-optimize-sibling-calls -fno-omit-frame-pointer -g
LDFLAGS = -lm
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
ELF=$(APP)-$(INPUT).elf
CMD = ./$(ELF) data/$(INPUT)

# Targets
help:
	@echo "Usage: make [option] [VARIABLE=value]"
	@echo ""
	@echo "Options:"
	@echo "  all                Build the project (default)"
	@echo "  clean              Remove build artifacts"
	@echo "  help               Show this help message"
	@echo "  run                Run the built application"
	@echo "  exectime-time      Run the app N times and measure ms time using /usr/bin/time"
	@echo "  exectime-perf      Run the app N times and measure time using perf stat"
	@echo "  profiling-perf     Profile the app and generate a call graph using perf"
	@echo
	@echo ""
	@echo "Settable variables:"
	@echo "  APP                Application name           Options: disparity, localization, mser, multi-ncut, pca, sift, stitch, svm, texture-synthesis, tracking (default: $(DEFAULT_APP))"
	@echo "  INPUT              Input dataset              Options: sim, sim_fast, (other benchmark-dependent inputs) (default: $(DEFAULT_INPUT))"
	@echo "  TARGET             Target architecture        Options: x86_64, arm (default: $(DEFAULT_TARGET))"
	@echo "  N                  Number of runs for timing  Options: N > 0 (default: $(DEFAULT_N))"
	@echo "  X86_64_COMPILER    Compiler for x86_64        (default: $(DEFAULT_X86_64_COMPILER))"
	@echo "  ARM_COMPILER       Compiler for ARM           (default: $(DEFAULT_ARM_COMPILER))"
	@echo ""
	@echo "Examples:"
	@echo "  make APP=disparity INPUT=sim_fast TARGET=x86"
	@echo "  make exectime-time N=100 APP=disparity INPUT=sim_fast TARGET=x86"
	@echo "  make clean"

all: $(ELF)

$(ELF):
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(ELF) $(SRC) -D$(INPUT) -DGENERATE_OUTPUT -DCHECK

run: $(ELF)
	@echo "Running $(CMD)"
	@$(CMD)

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



profiling-perf: $(ELF)
	@echo "Running perf on: $(CMD)"
	@sudo perf record -F 72000 -g --call-graph dwarf --all-user -- $(CMD)
	@sudo perf script > out.perf
	@profiling/stackcollapse-perf.pl out.perf > out.folded
	@profiling/flamegraph.pl --title "CPU Flamegraph" --countname=Samples --width 1800 --colors hot --minwidth 0.5 out.folded > $(APP)_flamegraph.svg
	@echo "Flamegraph generated: $(APP)_flamegraph.svg"
	@sudo perf report --stdio --sort symbol --percent-limit 0 > perf-flat.txt
	@python3 profiling/perf2csv.py $(APP)_profile.csv
	@echo "CSV profile generated: perf-profile.csv"
	@rm -f out.perf out.folded perf.data perf.data.old perf-flat.txt

clean:
	rm -f $(ELF) out.perf out.folded perf.data perf.data.old perf-flat.txt $(OBJ) *.svg *.csv

