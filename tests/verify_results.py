import json
import sys
import re

def parse_output(output_file):
    voltages = {}
    with open(output_file, 'r') as f:
        content = f.read()

    # Check for errors
    error_patterns = {
        4: r"zero resistance",
        6: r"floating",
        5: r"singular matrix"
    }
    for code, pattern in error_patterns.items():
        if re.search(pattern, content, re.IGNORECASE):
            return {"error_code": code}

    # Parse voltages
    voltage_pattern = re.compile(r"V(\d+)\s+=\s+([0-9.]+)\s+V")
    matches = voltage_pattern.findall(content)
    for node, voltage in matches:
        voltages[node] = float(voltage)

    return voltages

def main():
    if len(sys.argv) != 3:
        print("Usage: python verify_results.py <output_file> <expected_file>")
        sys.exit(1)

    output_file = sys.argv[1]
    expected_file = sys.argv[2]

    actual = parse_output(output_file)
    with open(expected_file, 'r') as f:
        expected = json.load(f)

    if "error_code" in expected:
        if actual.get("error_code") == expected["error_code"]:
            sys.exit(0)
        else:
            print(f"Expected error {expected['error_code']}, got {actual.get('error_code', 'none')}")
            sys.exit(1)
    else:
        tolerance = expected.get("tolerance", 0.001)
        errors = []
        for node, value in expected["nodes"].items():
            actual_value = actual.get(node)
            if actual_value is None or abs(actual_value - value) > tolerance:
                errors.append(f"Node {node}: Expected {value}, Got {actual_value}")
        if errors:
            print("\n".join(errors))
            sys.exit(1)
        else:
            sys.exit(0)

if __name__ == "__main__":
    main()