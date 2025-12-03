#!/bin/bash

edges_file="$1"
edges_output_file="edges_output.txt"
vertices_file="$2"
vertices_output_file="vertices_output.txt"


# ------------------------------------
# Convert edges format
# ------------------------------------
# Read all non-empty lines into an array
mapfile -t lines < <(grep . "$edges_file")

# Clear output file
> "$edges_output_file"

count=${#lines[@]}

for ((i=0; i<count; i++)); do
    line="${lines[$i]}"

    # Extract first 3 chars, uppercase
    code=$(echo "${line:0:3}" | tr '[:lower:]' '[:upper:]')

    # Generate random X and Y
    rand_x=$(( RANDOM % 500 ))
    rand_y=$(( RANDOM % 500 ))

    # Determine ending symbol (comma or semicolon)
    if [[ $i -eq $((count - 1)) ]]; then
        end=";"
    else
        end=","
    fi

    # Write formatted output
    echo "('$code', '$line', $rand_x, $rand_y)$end" >> "$edges_output_file"
done

echo "Edges Done! Generated $edges_output_file"




mapfile -t rows < "$vertices_file"
count=${#rows[@]}

# Clear output file
> "$vertices_output_file"

# ------------------------------------
# Convert vertices format
# ------------------------------------
for ((i=0; i<count; i++)); do
    IFS=',' read -r a b c d <<< "${rows[$i]}"

    # Last line ends with semicolon
    [[ $i -eq $((count - 1)) ]] && end=";" || end=","

    printf "(%d, %d, %d, %d)%s\n" "$((a+1))" "$((b+1))" "$c" "$d" "$end" >> "$vertices_output_file"
done

echo "Vertics Done! Generated $vertices_output_file"



# ------------------------------------
# Generate seed.sql output
# ------------------------------------
template_file="seed_template.sql"
output_file="./db/seed_output.sql"

> "$output_file"

while IFS= read -r line; do
    case "$line" in
        "----Please input airports----")
            cat "$edges_output_file" >> "$output_file"
            ;;
        "----Please input routes----")
            cat "$vertices_output_file" >> "$output_file"
            ;;
        *)
            echo "$line" >> "$output_file"
            ;;
    esac
done < "$template_file"

echo "SQLite Seed Done! Generated $output_file"


# ------------------------------------
# Remote the meta data
# ------------------------------------

rm "$edges_output_file"
rm "$vertices_output_file"