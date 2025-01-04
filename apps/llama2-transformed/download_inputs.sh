#!/bin/bash
if ! command -v wget &> /dev/null; then
    echo "Error: wget is not installed."
    echo "Please install wget and try again."
    exit 1
fi
wget -O stories260K.bin https://huggingface.co/karpathy/tinyllamas/resolve/main/stories260K/stories260K.bin
wget -O stories15M.bin https://huggingface.co/karpathy/tinyllamas/resolve/main/stories15M.bin
wget -O stories42M.bin https://huggingface.co/karpathy/tinyllamas/resolve/main/stories42M.binstories42M.bin
wget -O stories110M.bin https://huggingface.co/karpathy/tinyllamas/resolve/main/stories110M.bin
