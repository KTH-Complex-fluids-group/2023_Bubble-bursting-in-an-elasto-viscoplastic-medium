# Bubble Bursting in an Elastoviscoplastic Medium

This repository contains the code and supplementary material for simulating bubble bursting dynamics in an elastoviscoplastic medium. The simulation is implemented in 2D+axi (axisymmetric) configuration using the Basilisk framework.

## Publication

This work has been published in the Journal of Fluid Mechanics. If you use this code or data in your research, please cite:

```bibtex
@article{balasubramanian2024bursting, 
    title={Bursting bubble in an elastoviscoplastic medium}, 
    volume={1001}, 
    DOI={10.1017/jfm.2024.1073}, 
    journal={Journal of Fluid Mechanics}, 
    author={Balasubramanian, A. G. and Sanjay, V. and Jalaal, M. and Vinuesa, R. and Tammisola, O.}, 
    year={2024}, 
    pages={A9}
}
```

The article can be found at: 

[![](https://img.shields.io/badge/Journal%20of%20Fluid%20Mechanics-10.1017/jfm.2024.1073-blue)](https://doi.org/10.1017/jfm.2024.1073)
[![](https://img.shields.io/badge/arXiv-4b4b4b?style=flat&logo=arxiv&link=https://arxiv.org/pdf/2409.14897.pdf)](https://arxiv.org/pdf/2409.14897.pdf)


## Background

The simulation models the bursting dynamics of a bubble in an elasto-viscoplastic medium, which exhibits both elastic and viscoplastic behavior. The physics includes:
- Two-phase flow with surface tension
- Elasto-viscoplastic rheology
- Axisymmetric geometry
- Log-conformation formulation for numerical stability

## Repository Structure

```
├── 01_code/                      # Source code and headers
│   ├── burst_evp.c              # Main simulation driver
│   ├── log-conform-EVP.h        # Log-conformation method for EVP materials
│   ├── log-conform-EVP-data.h   # Header for data restoration tools
│   ├── saramito-EVP.h           # Saramito EVP constitutive model
│   ├── fene-p-EVP.h             # FENE-P model implementation
│   ├── adapt_wavelet_limited.h  # Adaptive mesh refinement
│   └── Bo0.0010.dat             # Initial bubble shape (3.5 MB)
├── 02_videos/                    # Simulation videos (66 files, 67 MB)
├── 03_supplementary_plots/       # Analysis plots (79 files, 35 MB)
├── 04_graphical_abstract/        # Post-processing tools
│   ├── restore_data.c           # Data extraction utility
│   ├── restore_index.c          # Flow topology calculator
│   ├── python_script.py         # 2D visualization
│   ├── blender_script.py        # 3D rendering
│   └── README.md                # Tools documentation
├── Makefile                      # Build system
├── README.md                     # This file
└── LICENSE                       # GPL v3.0 license
```

## Implementation

### Key Files

**Main Simulation:**
- `01_code/burst_evp.c` - Main simulation file implementing two-phase flow physics
  - Command-line arguments: J (Plastocapillary number) and Deb (Deborah number)
  - Fixed parameters: Bond=0.001, B=0.5 (solvent ratio)
  - Outputs: Snapshots every 0.005 time units

**Physics Implementation:**
- `01_code/log-conform-EVP.h` - Log-conformation method for elasto-viscoplastic materials
  - Handles High Weissenberg Number Problem
  - Implements yielded/unyielded region tracking
- `01_code/saramito-EVP.h` - Saramito's EVP constitutive model
  - Defines stress and relaxation functions
  - Yield criterion: τ_D vs τ_0

**Numerical Methods:**
- `01_code/adapt_wavelet_limited.h` - Adaptive mesh refinement with spatial limits
  - Wavelet-based error estimation
  - Region-specific refinement control

**Post-processing:**
- `01_code/log-conform-EVP-data.h` - Header for data restoration utilities
- `01_code/fene-p-EVP.h` - FENE-P model (alternative to Saramito)

### Key Parameters
- `Bond`: Bond number (ratio of gravitational to surface tension forces)
- `J`: Dimensionless yield stress
- `Deb`: Deborah number (ratio of relaxation time to observation time)
- `B`: Solvent to total viscosity ratio
- `tmax`: Maximum simulation time
- `DT_MAX`: Maximum time step

### Numerical Methods
- Grid: Adaptive mesh refinement with levels from `LEVEL` (8) to `MAXlevel` (11)
- Time integration: Centered Navier-Stokes solver
- Interface tracking: Volume-of-Fluid method with tension
- Error tolerances:
  - Fraction error (fErr): 1e-3
  - Curvature error (KErr): 1e-4
  - Velocity error (VelErr): 1e-2
  - Vorticity error (OmegaErr): 1e-3

## Prerequisites

**Required:**
- **Basilisk Framework**: Install from [http://basilisk.fr/src/INSTALL](http://basilisk.fr/src/INSTALL)
  - This provides the `qcc` compiler and numerical solvers
  - Version: Latest stable release recommended
- **GCC Compiler**: With OpenMP support (usually included)
  - Most Linux distributions: `sudo apt-get install gcc`
  - macOS: Install via Xcode Command Line Tools

**Optional (for post-processing):**
- **Python 3.x** with packages: `numpy`, `matplotlib`, `scipy`
  - Install: `pip install numpy matplotlib scipy`
- **Blender 2.8+** (for 3D visualizations)

## Quick Start

### 1. Installation Check

Verify Basilisk is installed:
```bash
which qcc  # Should show path to qcc compiler
qcc --version  # Check it works
```

If `qcc` is not found, install Basilisk following the instructions at [http://basilisk.fr/src/INSTALL](http://basilisk.fr/src/INSTALL).

### 2. Build

Using the provided Makefile (recommended):
```bash
# Build main simulation
make

# Build all components including post-processing tools
make test

# See all options
make help
```

Or compile manually:
```bash
cd 01_code
qcc -O2 -Wall -fopenmp burst_evp.c -o burst_evp -lm
```

### 3. Run

```bash
# Set number of threads (optional, for parallel execution)
export OMP_NUM_THREADS=4

# Run with Plastocapillary number J=1.0 and Deborah number Deb=0.5
./build/burst_evp 1.0 0.5

# Or using make
make run J=1.0 DEB=0.5
```

## Getting Started

### Initial Configuration

The simulation requires an initial shape file that defines the bubble geometry:
- **File**: `01_code/Bo0.0010.dat` - Initial bubble shape for Bond number 0.001
- **Location**: Already included in the repository (3.5 MB, 109,426 points)
- **Format**: Text file with r, z coordinates defining bubble interface
- **Source**: For details on how this shape is obtained, see [VatsalSy/Bursting-Bubble-In-a-Viscoplastic-Medium](https://github.com/VatsalSy/Bursting-Bubble-In-a-Viscoplastic-Medium)

### Running Simulations

**Method 1: Using Makefile (Recommended)**
```bash
# Build the simulation
make

# Run with custom parameters
make run J=1.0 DEB=0.5

# The simulation will run and create output files in the current directory
```

**Method 2: Manual Compilation**
```bash
cd 01_code

# Compile with OpenMP support
qcc -O2 -Wall -fopenmp burst_evp.c -o burst_evp -lm

# Set number of OpenMP threads for parallel execution
export OMP_NUM_THREADS=4

# Run with Plastocapillary (J) and Deborah (Deb) numbers
./burst_evp 1.0 0.5  # Example: J=1.0, Deb=0.5
```

### Build System

The repository includes a comprehensive Makefile with the following targets:

| Command | Description |
|---------|-------------|
| `make` or `make all` | Build main simulation executable |
| `make tools` | Build post-processing utilities (restore_data, restore_index) |
| `make test` | Build all components |
| `make run J=X DEB=Y` | Build and run simulation with parameters |
| `make clean` | Remove build artifacts |
| `make distclean` | Remove builds and simulation outputs |
| `make help` | Show detailed help |

All executables are built in the `build/` directory.

## Outputs

### Output Files

The simulation generates several output files:
- `intermediate/snapshot-*.dat`: Simulation state at regular intervals
- `timestep.txt`: Time stepping information
- `log`: Contains kinetic energy and other diagnostic data
- `01_pp/png/`: Directory for PNG output files
- `01_pp/pdf/`: Directory for PDF output files

## Visualization and Post-processing

### Post-processing Tools

The `04_graphical_abstract/` directory contains tools for extracting and visualizing simulation data:

**C Programs** (require Basilisk):
- `restore_data.c` - Extract field data from simulation dumps
- `restore_index.c` - Calculate flow topology indices

**Python Scripts**:
- `python_script.py` - Create 2D visualizations
- `blender_script.py` - Generate 3D renderings

**Build and use:**
```bash
# Build post-processing tools
make tools

# Extract data from a simulation dump file
./build/restore_data dump-0.9500 data-0.9500 0.5 1.0 0.5

# Calculate flow topology index
./build/restore_index dump-0.9500 index-0.9500 0.5 1.0 0.5

# Visualize (requires Python packages)
cd 04_graphical_abstract
python python_script.py
```

For detailed documentation on post-processing tools, see [`04_graphical_abstract/README.md`](04_graphical_abstract/README.md).

### Supplementary Materials

The repository includes published supplementary materials:
- **`02_videos/`**: 66 videos showing bubble dynamics for different parameter combinations
- **`03_supplementary_plots/`**: 79 plots analyzing flow topology, energy distribution, and dynamics
- **`04_graphical_abstract/`**: Tools and scripts for creating publication-quality visualizations


## Contact

If you need some additional data that might be of interest to you, please don't hesitate to contact us at:\
``Arivazhagan G B`` [![](https://img.shields.io/badge/Mail-blue?style=flat&logo=microsoftoutlook&link=mailto:argb@mech.kth.se)](mailto:argb@mech.kth.se) [![](https://img.shields.io/badge/Scholar-4b4b4b?style=flat&logo=googlescholar&link=https://scholar.google.com/citations?user=xyheRZ8AAAAJ&hl=en)](https://scholar.google.com/citations?user=xyheRZ8AAAAJ&hl=en) [![](https://img.shields.io/badge/LinkedIn-blue?style=flat&logo=linkedin&link=https://www.linkedin.com/in/arivazhagan-geetha-balasubramanian-648b8567/)](https://www.linkedin.com/in/arivazhagan-geetha-balasubramanian-648b8567/)\
``Vatsal Sanjay`` [![](https://img.shields.io/badge/Mail-blue?style=flat&logo=microsoftoutlook&link=mailto:vatsalsanjay@gmail.com)](mailto:vatsalsanjay@gmail.com) [![](https://img.shields.io/badge/Scholar-4b4b4b?style=flat&logo=googlescholar&link=https://scholar.google.com/citations?user=67aQviYAAAAJ&hl=en&oi=ao)](https://scholar.google.com/citations?user=67aQviYAAAAJ&hl=en&oi=ao) [![](https://img.shields.io/badge/LinkedIn-blue?style=flat&logo=linkedin&link=https://www.linkedin.com/in/vatsalsanjay/)](https://www.linkedin.com/in/vatsalsanjay/)\
``Outi Tammisola`` [![](https://img.shields.io/badge/Mail-blue?style=flat&logo=microsoftoutlook&link=mailto:outi@mech.kth.se)](mailto:outi@mech.kth.se) [![](https://img.shields.io/badge/Scholar-4b4b4b?style=flat&logo=googlescholar&link=https://scholar.google.com/citations?user=XSKb9YAAAAAJ&hl=en&oi=ao)](https://scholar.google.com/citations?user=XSKb9YAAAAAJ&hl=en&oi=ao) [![](https://img.shields.io/badge/LinkedIn-blue?style=flat&logo=linkedin&link=https://www.linkedin.com/in/outi-tammisola-8b2b6511/)](https://www.linkedin.com/in/outi-tammisola-8b2b6511/)
