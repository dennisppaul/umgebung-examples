#!/usr/bin/env bash

# configurable top-level folders (space-separated)
EXAMPLE_DIRS=("Basics" "Audio" "Advanced")

# how many seconds to let each example run
RUN_DURATION=3

# build directory (inside test folder)
BUILD_DIR="build-test-examples"

# logs directory (inside test folder)
LOG_DIR="logs"

# root directory where examples live (relative to this script)
EXAMPLES_ROOT=".."

# color + emoji helpers
GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"
CHECK="${GREEN}✅${RESET}"
CROSS="${RED}❌${RESET}"

# clean and prepare build and log directories
mkdir -p "$BUILD_DIR"
rm -rf "$LOG_DIR"
mkdir -p "$LOG_DIR"

echo "--- Testing selected example folders: ${EXAMPLE_DIRS[*]}"

for CATEGORY in "${EXAMPLE_DIRS[@]}"; do
    CATEGORY_PATH="$EXAMPLES_ROOT/$CATEGORY"
    
    if [ ! -d "$CATEGORY_PATH" ]; then
        echo -e "${CROSS} ${CATEGORY} (not found)"
        continue
    fi

    echo "--- $CATEGORY"
    for EXAMPLE in "$CATEGORY_PATH"/*; do
        if [ -d "$EXAMPLE" ] && [ -f "$EXAMPLE/CMakeLists.txt" ]; then
            REL_PATH="${EXAMPLE#$EXAMPLES_ROOT/}"
            BUILD_SUBDIR="$BUILD_DIR/$REL_PATH"
            mkdir -p "$BUILD_SUBDIR"
            
            cmake -S "$EXAMPLE" -B "$BUILD_SUBDIR" &> /dev/null
            if cmake --build "$BUILD_SUBDIR" &> /dev/null; then
                # find the executable (assume only one .cpp file producing one binary)
                
                EXECUTABLE=""
                for f in "$BUILD_SUBDIR"/*; do
                    if [ -f "$f" ] && [ -x "$f" ]; then
                        EXECUTABLE="$f"
                        break
                    fi
                done                
                
                if [ -x "$EXECUTABLE" ]; then
                    LOG_FILE="$LOG_DIR/$(echo "$REL_PATH" | tr / -).log"
                    
                    nohup "$EXECUTABLE" >> "$LOG_FILE" 2>&1 &
                    PID=$!
                    sleep "$RUN_DURATION"
                    kill "$PID" &> /dev/null
                    wait "$PID"
                    EXIT_CODE=$?
                    
                    # consider SIGTERM (143) as successful run too
                    if [ "$EXIT_CODE" -eq 0 ] || [ "$EXIT_CODE" -eq 143 ]; then
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