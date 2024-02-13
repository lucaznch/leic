#!/bin/bash

# Color codes
GREEN='\033[0;32m'  # Green for PASS
RED='\033[0;31m'    # Red for FAIL
NC='\033[0m'        # No color (reset)

# Get the directory of the script
script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

input_dir="$script_dir"
output_dir="$script_dir"
result_dir="$script_dir"

# Iterate over input files
for input_file in "$input_dir"/*.jobs; do
    # Extract the filename (without extension)
    filename=$(basename -- "$input_file")
    filename_without_extension="${filename%.*}"

    # Define corresponding output and result files
    output_file="$output_dir/$filename_without_extension.out"
    result_file="$result_dir/$filename_without_extension.result"

    # Check if the output file exists
    if [ ! -f "$output_file" ]; then
        echo -e "${RED}Error: Output file not found for $input_file.${NC}"
        continue
    fi

    # Check if the result file exists
    if [ ! -f "$result_file" ]; then
        echo -e "${RED}Error: Result file not found for $input_file.${NC}"
        continue
    fi

    # Run diff and print the result with color
    diff_output=$(diff "$output_file" "$result_file")
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}PASS:${NC} $input_file"
    else
        echo -e "${RED}FAIL:${NC} $input_file"
        echo "$diff_output"
    fi
done
