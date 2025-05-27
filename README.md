# 🌌 SkyScan

## Overview

SkyScan processes sighting reports containing apparent speed and brightness data alongside known aircraft signature values to identify potential matches. The program offers both linear and binary search algorithms for efficiency optimization and provides detailed timing analysis for performance evaluation.

## 🚀 Features

- **C++11 compliant implementation** with modern standards
- **High-resolution timing** via `<chrono>` library
- **Google-style C++ code formatting** for readability
- **Modular, object-oriented design** for maintainability
- **Dataset generation support** for testing
- **Flexible search algorithms** (linear vs binary search)
- **Performance benchmarking** with microsecond precision

## 📁 File Structure

```
├── Makefile                 # Build configuration
├── README.md               # This file
├── report.pdf              # Performance analysis report
├── sighting_search.cc      # Main application source
├── create_dataset          # Dataset generator executable
├── create_dataset.cc       # Dataset generator source (optional)
├── sightings_X.dat         # Generated sightings data files
├── signatures_X.dat        # Generated aircraft signature files
└── res_X.dat              # Output files with match results
```

## 🛠️ How to Compile

From the project root directory:

```bash
# Compile the main application
make

# Clean build artifacts
make clean
```

The Makefile uses the following compilation command:
```bash
g++ -std=c++11 -Wall -Werror sighting_search.cc -o sighting_search
```

## 📈 How to Run

### Basic Usage

```bash
./sighting_search <sighting_file.dat> <signature_file.dat> <result_file.dat>
```

### Example

```bash
./sighting_search sightings_1.dat signatures_1.dat res_1.dat
```

### Interactive Prompts

When you run the program, you'll be prompted to choose a search method:

```
Choice of search method ([l]inear, [b]inary)?
```

- **Linear Search (`l`)**: Best for smaller datasets or when data is unsorted
- **Binary Search (`b`)**: More efficient for larger datasets (automatically sorts data first)

Invalid inputs will prompt again until a valid selection is made.

### Sample Output

```bash
$ ./sighting_search sightings_1.dat signatures_1.dat res_1.dat
Choice of search method ([l]inear, [b]inary)?
l
CPU time: 12.556 microseconds

$ cat res_1.dat
2
```

## 🧬 Dataset Generation

Use the provided `create_dataset` binary to generate randomized test files for evaluation:

### Command Syntax

```bash
./create_dataset <#sightings> <#signatures> <suffix>
```

### Example

```bash
./create_dataset 100 25 test1
```

This generates:
- `sightings_test1.dat` (100 sighting records)
- `signatures_test1.dat` (25 aircraft signature records)

## 📊 Performance Analysis

The program outputs execution time in microseconds for performance evaluation. For detailed analysis of time complexity and efficiency trade-offs between linear and binary search approaches, including charts and performance benchmarks, see the included `report.pdf`.

## 🔧 Technical Details

- **Language**: C++11
- **Timing Resolution**: Microseconds
- **Search Algorithms**: Linear O(n) and Binary O(log n)
- **Memory Management**: Object-oriented design with proper resource handling
- **Code Style**: Google C++ Style Guide compliant

## 📄 Documentation

For comprehensive performance analysis, algorithmic comparisons, and benchmark results, refer to the detailed technical report: [report.pdf](https://github.com/user-attachments/files/20465205/report.pdf)

---

*SkyScan helps distinguish between conventional aircraft and unidentified aerial phenomena through efficient data analysis and pattern matching.*
