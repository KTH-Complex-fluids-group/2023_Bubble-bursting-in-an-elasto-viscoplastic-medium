# Contributing to Bubble Bursting Simulation

Thank you for your interest in this research code! This repository contains the implementation for the paper "Bursting bubble in an elastoviscoplastic medium" published in the Journal of Fluid Mechanics.

## Using This Code

This repository is primarily intended for:
- Reproducing results from the published paper
- Understanding the implementation details
- Adapting the methods for related research problems
- Educational purposes

## Getting Help

### Questions About the Code

If you have questions about:
- **Installation issues** - Check the README.md prerequisites section
- **Running simulations** - See the Quick Start guide in README.md
- **Post-processing** - Refer to `04_graphical_abstract/README.md`
- **Physics/methods** - Consult the published paper (DOI: 10.1017/jfm.2024.1073)

### Reporting Issues

If you encounter problems:

1. **Check existing documentation** first:
   - README.md - Main documentation
   - 04_graphical_abstract/README.md - Post-processing tools
   - Makefile help: `make help`

2. **For Basilisk-related issues**:
   - Visit [Basilisk documentation](http://basilisk.fr/)
   - Check [Basilisk forum](http://basilisk.fr/sandbox/)

3. **For code-specific issues**:
   - Contact the authors (see README.md for contact information)
   - Include:
     - Your system configuration (OS, compiler version)
     - Basilisk version
     - Full error message
     - Steps to reproduce the problem

## Code Modifications

### For Your Own Research

You are welcome to modify this code for your research! The code is licensed under GPL v3.0.

**Please ensure you:**
1. Cite the original paper in your publications
2. Comply with the GPL v3.0 license terms
3. Document your modifications clearly

### Suggesting Improvements

If you find bugs or have suggestions for improvements:
1. Contact the authors directly (see README.md)
2. Provide:
   - Clear description of the issue or suggestion
   - Minimal working example if reporting a bug
   - Proposed solution if you have one

## Development Setup

If you want to modify the code:

### Build System

```bash
# Build main simulation
make

# Build post-processing tools
make tools

# Build everything
make test

# Clean build artifacts
make clean
```

### Code Structure

```
01_code/
├── burst_evp.c              # Main simulation - start here
├── log-conform-EVP.h        # Core physics implementation
├── saramito-EVP.h           # Constitutive model
├── adapt_wavelet_limited.h  # Mesh adaptation
└── *.h                      # Supporting headers
```

### Testing Your Modifications

1. **Quick test** (fast, low resolution):
```bash
make run J=0.5 DEB=0.5
# Check that it runs without errors
```

2. **Compare with baseline**:
   - Run a case from the paper
   - Compare your results with published videos/plots in `02_videos/` and `03_supplementary_plots/`

3. **Post-process your results**:
```bash
make tools
./build/restore_data dump-0.9500 data-0.9500 0.5 0.5 0.5
```

### Code Style

This code follows Basilisk C conventions:
- Event-driven structure
- Basilisk-specific syntax (foreach loops, etc.)
- Mathematical variable names matching paper notation
- Extensive inline documentation

When modifying:
- Keep variable names consistent with the paper
- Add comments explaining physics/numerical choices
- Document any changes to parameters or methods

## Parameter Studies

To run parameter sweeps:

```bash
# Example: vary Plastocapillary number J
for J in 0.5 1.0 2.0; do
    ./build/burst_evp $J 0.5
    # Move outputs to organized directories
    mkdir -p results/J_${J}
    mv dump-* results/J_${J}/
done
```

**Recommended parameters for exploration:**
- J (Plastocapillary): 0.1 to 10.0
- Deb (Deborah): 0.1 to 10.0
- Bond: Fixed at 0.001 (modify in burst_evp.c to change)
- B (solvent ratio): Fixed at 0.5 (modify to change)

## Computational Resources

**Typical resource requirements:**
- Memory: 2-8 GB depending on max refinement level
- CPU: Parallelizes with OpenMP (set `OMP_NUM_THREADS`)
- Time: 2-12 hours per simulation (depends on parameters and resolution)
- Storage: ~1-5 GB per simulation (dump files)

**Recommended:**
- Use multi-core system with OpenMP
- Adequate disk space for dump files
- Linux or macOS (Basilisk runs best on Unix-like systems)

## Citation

If you use or modify this code, please cite:

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

## License

This code is distributed under the GNU General Public License v3.0. See LICENSE file for details.

This means:
- ✅ You can use it for research and education
- ✅ You can modify it for your needs
- ✅ You can distribute your modifications
- ⚠️ Modified versions must also be GPL v3.0
- ⚠️ You must cite the original work

## Contact

For questions, collaboration opportunities, or additional data:

- **Arivazhagan G B**: argb@mech.kth.se
- **Vatsal Sanjay**: vatsalsanjay@gmail.com
- **Outi Tammisola**: outi@mech.kth.se

See README.md for full contact information and links.

## Acknowledgments

If you build upon this work, consider acknowledging:
- The Basilisk framework (http://basilisk.fr)
- The original publication (DOI: 10.1017/jfm.2024.1073)
- Any supporting funding agencies mentioned in the paper
