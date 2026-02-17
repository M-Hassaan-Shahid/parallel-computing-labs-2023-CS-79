# PDCL Lab - Parallel Computing Performance Analysis

**Author:** M. Hassaan Shahid  
**Registration No:** 2023-CS-79  
**Course:** PDCL  
**Professor:** Sir Waqas

## Overview

This repository contains implementations and performance analysis of parallel counting algorithms using different parallelization techniques in C (pthreads) and Python (threading, multiprocessing).

## Project Structure

```
.
├── count_seq.py              # Python sequential implementation
├── count_threads.py          # Python threading implementation
├── count_mp.py               # Python multiprocessing implementation
├── count_pthread.c           # C pthread implementation
├── search_pthread.c          # C pthread search with early termination
├── lab_report.tex            # Complete LaTeX report
├── generate_correct_graph.py # Graph generation script
├── run_complete_experiments.sh # Automated experiment runner
└── results/                  # Experimental results and graphs
```

## Quick Start

### 1. Run Experiments
```bash
# Make script executable
chmod +x run_complete_experiments.sh

# Run all experiments
./run_complete_experiments.sh
```

### 2. Generate Graphs
```bash
# Install matplotlib (if needed)
sudo apt-get install python3-matplotlib

# Generate speedup graphs
python3 generate_correct_graph.py
```

### 3. Compile Report
```bash
# Install LaTeX (if needed)
sudo apt-get install texlive-full

# Compile report
pdflatex lab_report.tex
pdflatex lab_report.tex  # Run twice for TOC

# View PDF
xdg-open lab_report.pdf
```

## Manual Compilation

### C Programs
```bash
# Compile with optimizations
gcc -O2 -pthread -o count_pthread count_pthread.c
gcc -O2 -pthread -o search_pthread search_pthread.c

# Run
./count_pthread
./search_pthread
```

### Python Programs
```bash
python3 count_seq.py
python3 count_threads.py
python3 count_mp.py
```

## Key Results

| Implementation | Threads | Time (s) | Speedup |
|----------------|---------|----------|---------|
| C Sequential | 1 | 3.12 | 1.00× |
| C Pthreads | 4 | 0.16 | 19.50× |
| Python Sequential | 1 | 71.49 | 1.00× |
| Python Threading | 4 | 7.12 | 10.04× |
| Python Multiprocessing | 4 | 3.41 | 20.96× |

### Key Findings
- C achieved super-linear speedup (19.50× with 4 threads)
- Python multiprocessing successfully bypassed GIL (20.96× speedup)
- C is 22.9× faster than Python (sequential comparison)
- Python threading showed 10.04× speedup despite GIL

## System Specifications

- **CPU:** Intel Core i5-8265U @ 1.60GHz
- **Cores:** 4 physical cores, 8 logical (with Hyper-Threading)
- **OS:** Linux (Ubuntu 24.04)
- **Compiler:** GCC with -O2 optimization
- **Python:** 3.12

## Dependencies

- GCC with pthread support
- Python 3.6+
- matplotlib (for graph generation)
- LaTeX distribution (for report compilation)

## Installation

```bash
# Ubuntu/Debian
sudo apt-get install build-essential python3-matplotlib texlive-full

# Verify installation
gcc --version
python3 --version
pdflatex --version
```

## Documentation

- `lab_report.pdf` - Complete lab report with analysis
- `README.md` - This file
- Source code comments - Inline documentation

## License

This project is for educational purposes as part of the PDCL course.

## Contact

M. Hassaan Shahid - 2023-CS-79
