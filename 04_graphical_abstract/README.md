# Post-Processing and Visualization Tools

This directory contains tools for extracting, analyzing, and visualizing simulation data from the bubble bursting simulations.

## Overview

The post-processing workflow consists of:
1. **Data extraction** - Extract field data from simulation dumps
2. **Index calculation** - Calculate flow topology indices
3. **Visualization** - Create plots and 3D renderings using Python and Blender

## Tools

### C Programs (Data Extraction)

#### restore_data.c

Extracts field data from simulation dump files and outputs them in a format suitable for visualization.

**Purpose**: Exports velocity fields, volume fraction, stress tensors, and other quantities to a structured text file.

**Output Fields**:
- `f` - Volume fraction (VOF for interface tracking)
- `u.x, u.y` - Velocity components
- `g.x, g.y` - Geometric quantities
- `D2` - Logarithm of strain rate magnitude
- `rhov` - Density field
- `tau_p.x.x, tau_p.y.x, tau_p.y.y, tau_qq` - Polymeric stress tensor components
- `mupv, lambdav, tau0v` - Material properties
- `trA` - Trace of conformation tensor
- `solidreg` - Yielded/unyielded region indicator

**Usage**:
```bash
# First, build the tool
make tools  # from project root

# Run data extraction
./build/restore_data <input_dump> <output_file> <B> <J> <Deb>
```

**Parameters**:
- `input_dump` - Path to simulation dump file (e.g., "dump-0.9500")
- `output_file` - Output data file name (e.g., "data-0.9500")
- `B` - Solvent ratio parameter (typically 0.5)
- `J` - Plastocapillary number (e.g., 0.5, 1.0, 2.0)
- `Deb` - Deborah number (e.g., 0.5, 1.0, 2.0)

**Example**:
```bash
./build/restore_data dump-0.9500 data-0.9500 0.5 1.0 0.5
```

#### restore_index.c

Calculates flow topology indices from simulation dumps, specifically computing strain rate and vorticity measures to classify flow regions.

**Purpose**: Computes the flow topology index F2 = (|D| - |Ω|)/(|D| + |Ω|) where D is strain rate and Ω is vorticity.

**Output Fields**:
- `f` - Volume fraction
- `D2` - Squared strain rate magnitude
- `O2` - Squared vorticity magnitude
- `F2` - Flow topology index (-1 to 1, negative=rotation-dominated, positive=strain-dominated)
- `solidreg` - Yielded/unyielded region indicator

**Usage**:
```bash
# Build the tool
make tools  # from project root

# Run index calculation
./build/restore_index <input_dump> <output_file> <B> <J> <Deb>
```

**Example**:
```bash
./build/restore_index dump-0.9500 index-0.9500 0.5 1.0 0.5
```

### Python Scripts (Visualization)

#### python_script.py

Creates 2D visualizations and plots of the extracted data.

**Requirements**:
```bash
pip install numpy matplotlib scipy
```

**Expected Input Files**:
- `data-0.9500` - Field data from restore_data
- `interface-0.95.dat` - Interface position data
- `index-0.9500` - Flow topology index from restore_index

**Functionality**:
- Plots velocity fields
- Visualizes interface position
- Creates contour plots of stress and strain
- Generates flow topology visualizations

**Usage**:
```bash
python python_script.py
```

**Note**: You may need to modify the script to specify input file paths and configure plot parameters.

#### blender_script.py

Blender script for creating 3D renderings and visualizations of the bubble bursting process.

**Requirements**:
- Blender 2.8+ with Python API
- Data files from restore_data or restore_index

**Usage**:
```bash
blender --background --python blender_script.py
```

**Note**: This script is intended to be run within Blender's Python environment. You may need to adjust paths and rendering settings within the script.

## Workflow Example

Here's a complete workflow for post-processing simulation results:

### 1. Run Simulation

```bash
# Build and run simulation
make
./build/burst_evp 1.0 0.5  # J=1.0, Deb=0.5

# Simulation will create dump files like: dump-0.0000, dump-0.0050, ..., dump-0.9500, ...
```

### 2. Extract Data

```bash
# Build post-processing tools
make tools

# Extract field data at t=0.95
./build/restore_data dump-0.9500 data-0.9500 0.5 1.0 0.5

# Calculate flow topology index
./build/restore_index dump-0.9500 index-0.9500 0.5 1.0 0.5
```

### 3. Visualize

```bash
# Create 2D plots
python python_script.py

# Create 3D rendering (if using Blender)
blender --background --python blender_script.py
```

## Data Format

The output files from `restore_data` and `restore_index` are in Basilisk's field format:
- Binary format optimized for large datasets
- Can be read with Basilisk's input functions or custom parsers
- Contains regular grid data with specified resolution (1024 points)
- Box domain: x ∈ [-4, 4], y ∈ [0, 8]

## Understanding the Output

### Strain Rate (D2)

The strain rate magnitude is computed as:
```
D = sqrt(D11² + D22² + D33² + 2*D13²)
```

where Dij are components of the rate-of-strain tensor in axisymmetric coordinates.

In the output, `D2` is stored as log10(D) for better visualization of multiple orders of magnitude.

### Flow Topology Index (F2)

The flow topology index F2 distinguishes between different flow regimes:
- F2 ≈ +1: Pure extensional flow (strain-dominated)
- F2 ≈ 0: Shear flow (equal strain and rotation)
- F2 ≈ -1: Pure rotational flow (vorticity-dominated)

This index is particularly useful for identifying:
- Extensional zones near the bubble cap
- Vortical structures in the wake
- Shear bands in yielded regions

### Yield Indicator (solidreg)

The `solidreg` field marks material state:
- `solidreg = +1`: Yielded region (material flowing)
- `solidreg = -1`: Unyielded region (solid-like behavior)

This is determined by comparing the stress against the yield stress threshold.

## Troubleshooting

### "Cannot open dump file"

Ensure the simulation has run and created dump files. Check that the file path is correct relative to your working directory.

### "qcc: command not found" when building tools

Basilisk framework is not installed or not in PATH. See main README for installation instructions.

### Python visualization errors

Ensure all required Python packages are installed:
```bash
pip install numpy matplotlib scipy
```

Check that data files exist and are in the correct location expected by the Python scripts.

## References

For more details on the simulation method and physics, see:
- Main README.md
- Published paper: Journal of Fluid Mechanics (2024), DOI: 10.1017/jfm.2024.1073
- Basilisk documentation: http://basilisk.fr/src/examples/

## Notes

- The restore scripts recreate the full simulation state but only output selected fields
- Output resolution is set to 1024 points for high quality visualization
- The computational domain used is [-4,4] × [0,8] matching the simulation setup
- All scripts assume axisymmetric geometry with axis at y=0
