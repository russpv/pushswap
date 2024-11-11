#!/bin/bash

unset ARG

# Number of random integers to generate
COUNT=10

# Maximum value for the random integers
MAX_VALUE=100

# Generate random integers and store them in ARG
ARG=""
for (( i=0; i<$COUNT; i++ )); do
    RANDOM_INT=$(( RANDOM % MAX_VALUE ))  # Generates a random integer between 0 and MAX_VALUE - 1
    ARG+="$RANDOM_INT "  # Append the random integer to ARG
done

# Remove trailing space
ARG=$(echo $ARG | xargs)

# Export ARG variable
export ARG

# Print the generated ARG variable
echo "Generated ARG: $ARG"

