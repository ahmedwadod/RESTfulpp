#!/bin/sh

cmake -Bbuild .
cmake --build build
for file in build/tests/*; do
	if [ -x "$file" ] && [ "$file" != "build/tests/CMakeFiles" ]; then
		# Execute the file
		./"$file"
	fi
done
