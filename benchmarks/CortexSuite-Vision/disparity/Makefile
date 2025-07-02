# Benchmark-specific definitions
APP = disparity
DEFAULT_INPUT = sim_fast

# General rules for any CortexSuite-Vision benchmark
CC = clang
CFLAGS = -w -O3
LDFLAGS = -lm
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
INPUT ?= $(DEFAULT_INPUT)
ELF=$(APP)-$(INPUT).elf

# Targets
all: $(ELF)

$(ELF):
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(ELF) $(SRC) -D$(INPUT) -DGENERATE_OUTPUT -DCHECK

run: $(ELF)
	@perf stat -e task-clock ./$(ELF) data/$(INPUT)

clean:
	rm -f $(ELF)
