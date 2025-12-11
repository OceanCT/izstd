#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
IZSTD_TESTS_DIR="$(dirname "$SCRIPT_DIR")"
TESTS_DIR="$(dirname "$IZSTD_TESTS_DIR")"
IZSTD_ROOT="$(dirname "$TESTS_DIR")"

DATASETS_DIR="$TESTS_DIR/izstd-tests/datasets/simple/"
ZSTD_BIN="$IZSTD_ROOT/programs/zstd"

OUTPUT_CSV="$SCRIPT_DIR/compress_results_simple.csv"
TMP_OUT="/tmp/zstd_tmp_out"

LEVELS="3"
REPEAT=1

# ============================================
# Build
# ============================================
echo "===[1] Building izstd...==="
cd "$IZSTD_ROOT"
make -j$(nproc)

if [[ ! -f "$ZSTD_BIN" ]]; then
    echo "❌ Build failed!"
    exit 1
fi

# ============================================
# CSV
# ============================================
echo "dataset,file,level,orig_size,comp_size,ratio,avg_time_ms,avg_throughput_MB_s" \
    > "$OUTPUT_CSV"


# ============================================
# Tests
# ============================================
echo "===[2] Running compression tests...==="

for dataset in "$DATASETS_DIR"/ ; do
    dataset_name=$(basename "$dataset")

    for file in "$dataset"/*; do
        [[ "$file" == *.zip ]] && continue
        [[ ! -f "$file" ]] && continue

        filename=$(basename "$file")
        orig_size=$(stat -c%s "$file")

        echo "--- Testing $dataset_name / $filename (size=$orig_size bytes) ---"

        for lvl in $LEVELS; do

            total_time_ms=0
            comp_size=0  

            echo "  Level $lvl: running $REPEAT repetitions..."

            for ((i = 1; i <= REPEAT; i++)); do

                # =============================================
                # move file to /dev/null to avoid caching effects
                # =============================================
                cat "$file" > /dev/null

                start_ns=$(date +%s%N)
                "$ZSTD_BIN" -$lvl -q -f "$file" -o "$TMP_OUT"
                end_ns=$(date +%s%N)

                elapsed_ms=$(( (end_ns - start_ns) / 1000000 ))
                total_time_ms=$(( total_time_ms + elapsed_ms ))

                comp_size=$(stat -c%s "$TMP_OUT")
            done

            avg_time_ms=$(awk "BEGIN { printf \"%.3f\", $total_time_ms / $REPEAT }")

            ratio=$(awk "BEGIN { printf \"%.5f\", $orig_size / $comp_size }")

            avg_throughput=$(awk "BEGIN {
                printf \"%.3f\", ($orig_size/1000000) / ($avg_time_ms/1000)
            }")

            echo "$dataset_name,$filename,$lvl,$orig_size,$comp_size,$ratio,$avg_time_ms,$avg_throughput" \
                >> "$OUTPUT_CSV"

            echo "    → avg_time=${avg_time_ms} ms, avg_throughput=${avg_throughput} MB/s, ratio=${ratio}"

        done
    done
done


echo "==============================================="
echo " Compression test finished."
echo " CSV Results: $OUTPUT_CSV"
echo "==============================================="
