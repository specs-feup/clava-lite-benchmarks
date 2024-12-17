#!/bin/bash
if ! command -v wget &> /dev/null; then
    echo "Error: wget is not installed."
    echo "Please install wget and try again."
    exit 1
fi
wget https://huggingface.co/karpathy/tinyllamas/blob/main/stories260K/stories260K.bin
wget https://huggingface.co/karpathy/tinyllamas/resolve/main/stories15M.bin
wget https://huggingface.co/karpathy/tinyllamas/resolve/main/stories42M.bin
wget https://huggingface.co/karpathy/tinyllamas/resolve/main/stories110M.bin
