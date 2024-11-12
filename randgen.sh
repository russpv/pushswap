#!/bin/zsh

# Check if a command-line argument is provided for duplicates
ALLOW_DUPLICATES=true
if [ "$1" = "--no-duplicates" ]; then
    ALLOW_DUPLICATES=false
fi

# Number of random integers to generate
COUNT=1000

# Maximum value for the random integers
MAX_VALUE=1000000

# Generate random integers and store them in ARG
ARG=""
generated_numbers=()  # Array to keep track of generated numbers

for (( i=0; i<COUNT; )); do
    RANDOM_INT=$(( RANDOM % MAX_VALUE ))  # Generates a random integer between 0 and MAX_VALUE

    if [ "$ALLOW_DUPLICATES" = true ]; then
        # Append the random integer to ARG without checking for duplicates
        ARG+="$RANDOM_INT "
        ((i++))  # Increment only if we are allowing duplicates
    else
        # Check if the number is already generated
        if [[ ! " ${generated_numbers[@]} " =~ " $RANDOM_INT " ]]; then
            ARG+="$RANDOM_INT "
            generated_numbers+=("$RANDOM_INT")  # Add this number to the array
            ((i++))  # Increment only if we add a unique number
        fi
    fi
done

# Remove trailing space
ARG=$(echo $ARG | xargs)

# Export ARG variable
export ARG

# Print the generated ARG variable
echo "Generated ARG: $ARG"
