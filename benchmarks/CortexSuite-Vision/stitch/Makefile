# Benchmark-specific definitions
APP = stitch
DEFAULT_INPUT = sim_fast
N = 10

# General rules for any CortexSuite-Vision benchmark
CC = clang
CFLAGS = -w -O3 -fno-inline -fno-inline-functions -fno-inline-functions-called-once -fno-default-inline -fno-optimize-sibling-calls -fno-omit-frame-pointer -g
LDFLAGS = -lm
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
INPUT ?= $(DEFAULT_INPUT)
ELF=$(APP)-$(INPUT).elf
CMD = ./$(ELF) data/$(INPUT)

# Targets
all: $(ELF)

$(ELF):
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(ELF) $(SRC) -D$(INPUT) -DGENERATE_OUTPUT -DCHECK

run: $(ELF)
	@echo "Running $(CMD)"
	@$(CMD)

run-perf: $(ELF)
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
clean:
	rm -f $(ELF)

