#!/bin/bash
set -e

# Absolute paths to avoid issues with spaces
TEST_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SIMULATOR="${TEST_ROOT}/../simulator"
TEST_DIR="${TEST_ROOT}/test_cases"

# Debug: Print all test files
echo -e "\nTest files found:"
ls -1 "${TEST_DIR}"/*.net || true  # Continue even if no files

# Clean previous outputs
rm -f "${TEST_DIR}"/*.output

# Iterate over all test files explicitly
for netlist in "${TEST_DIR}"/*.net; do
    test_name=$(basename "${netlist}" .net)
    expected_file="${TEST_DIR}/${test_name}.expected"
    output_file="${TEST_DIR}/${test_name}.output"

    echo -e "\n=== Running test: ${test_name} ==="

    # Run simulator and suppress debug output
    "${SIMULATOR}" "${netlist}" --quiet > "${output_file}" 2>&1 || true

    # Verify results
    if python3 "${TEST_ROOT}/verify_results.py" "${output_file}" "${expected_file}"; then
        echo "✅ PASS: ${test_name}"
    else
        echo "❌ FAIL: ${test_name}"
    fi

    # echo ""
done

echo -e "\n=== All tests completed ==="
exit 0