# SPICEy - A Lightweight Circuit Simulator ⚡

[![Build Status](https://img.shields.io/github/actions/workflow/status/yourusername/circuit-simulator/build.yml?style=flat-square)](https://github.com/yourusername/circuit-simulator/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://opensource.org/licenses/MIT)
[![Code Coverage](https://img.shields.io/badge/Coverage-90%25-brightgreen?style=flat-square)]()

A high-performance circuit simulator written in C, inspired by SPICE. Designed for educational use and embedded applications.

**Key Features**:
- ✅ DC Analysis (Resistors, Voltage/Current Sources)
- 🚧 Transient Analysis (Capacitors/Inductors coming soon!)
- 📊 Web Interface (Planned)
- 🔍 Modified Nodal Analysis (MNA) with sparse matrix support
- 🧪 Comprehensive test suite with 90%+ coverage

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Netlist Format](#netlist-format)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## Installation 💻

### Prerequisites
- GCC ≥ 9.0
- Make
- Python 3 (for testing)

### Build from Source
```bash
git clone https://github.com/yourusername/circuit-simulator.git
cd circuit-simulator
make