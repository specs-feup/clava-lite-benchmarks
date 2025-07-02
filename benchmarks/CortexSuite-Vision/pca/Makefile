# Benchmark-specific definitions
APP = pca
DEFAULT_INPUT = sat.trn 4435 37 R

# General rules for any CortexSuite-Vision benchmark
CC = clang
CFLAGS = -w -O3
LDFLAGS = -lm
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
INPUT ?= $(DEFAULT_INPUT)
INPUT_NAME="${INPUT// /_}"
ELF=$(APP)-$(INPUT_NAME).elf

# Targets
all: $(ELF)

$(ELF):
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(ELF) $(SRC) -D$(INPUT) -DGENERATE_OUTPUT -DCHECK

run: $(ELF)
	@perf stat -e task-clock ./$(ELF) data/$(INPUT)

clean:
	rm -f $(ELF)
