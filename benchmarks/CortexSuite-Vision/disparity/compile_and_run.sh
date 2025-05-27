INPUT_DIR=sim_fast
gcc *.c -lm -o disparity.elf
./disparity.elf data/$INPUT_DIR