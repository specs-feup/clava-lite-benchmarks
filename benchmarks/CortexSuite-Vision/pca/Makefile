# Benchmark-specific definitions
APP = pca
DEFAULT_INPUT = R

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
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(ELF) $(SRC)

run: $(ELF)
	@perf stat -e task-clock ./$(ELF) data/sat.trn 4435 37 $(INPUT)

clean:
	rm -f $(ELF)
