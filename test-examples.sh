#!/usr/bin/env bash

# configurable top-level folders (space-separated)
EXAMPLE_DIRS=("Basics" "Audio" "Advanced")

# how many seconds to let each example run
RUN_DURATION=3

# build directory
BUILD_DIR="build-test-examples"

# color + emoji helpers
GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"
CHECK="${GREEN}✅${RESET}"
CROSS="${RED}❌${RESET}"

# clean build dir
# rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
rm -rf logs
mkdir -p logs

echo "--- Testing selected example folders: ${EXAMPLE_DIRS[*]}"

for CATEGORY in "${EXAMPLE_DIRS[@]}"; do
    if [ ! -d "$CATEGORY" ]; then
        echo -e "${CROSS} ${CATEGORY} (not found)"
        continue
    fi

    echo "--- $CATEGORY"
    for EXAMPLE in "$CATEGORY"/*; do
        if [ -d "$EXAMPLE" ] && [ -f "$EXAMPLE/CMakeLists.txt" ]; then
            REL_PATH="$EXAMPLE"
            BUILD_SUBDIR="$BUILD_DIR/$REL_PATH"
            mkdir -p "$BUILD_SUBDIR"
            cmake -S "$EXAMPLE" -B "$BUILD_SUBDIR" &> /dev/null
            if cmake --build "$BUILD_SUBDIR" &> /dev/null; then
                # find the executable (assume only one .cpp file producing one binary)
                EXECUTABLE=$(find "$BUILD_SUBDIR" -maxdepth 1 -type f -perm +111 | head -n 1)
                if [ -x "$EXECUTABLE" ]; then
                    LOG_FILE="logs/$(echo "$REL_PATH" | tr / -).log"
                    
                    nohup "$EXECUTABLE" >> "$LOG_FILE" 2>&1 &
                    PID=$!
                    sleep "$RUN_DURATION"
                    kill "$PID" &> /dev/null
                    wait "$PID"
                    EXIT_CODE=$?
                    
                    if [ "$EXIT_CODE" -eq 0 ]; then
                        echo -e "--- $CHECK $REL_PATH"
                    else
                        echo -e "--- $CROSS $REL_PATH (runtime failed: exit code $EXIT_CODE)"
                    fi
                else
                    echo -e "--- $CROSS $REL_PATH (no executable)"
                fi
            else
                echo -e "--- $CROSS $REL_PATH (build failed)"
            fi
        fi
    done
done
