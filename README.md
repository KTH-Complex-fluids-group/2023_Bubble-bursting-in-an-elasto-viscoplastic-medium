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

## Implementation

### Key Files
- `01_code/burst_evp.c`: Main simulation file implementing the physics
- `01_code/log-conform-EVP.h`: Implementation of log-conformation method for viscoelastic models
- `01_code/saramito-EVP.h`: Implementation of Saramito's elasto-viscoplastic model
- `01_code/adapt_wavelet_limited.h`: Adaptive mesh refinement implementation

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

## Getting Started

### Prerequisites
- Install the [Basilisk framework](http://basilisk.fr/src/INSTALL)
- Ensure you have a C compiler with OpenMP support
- Python 3.x for post-processing

### Initial Configuration
The simulation requires an initial shape file that defines the bubble geometry:
- `Bo0.0010.dat`: Initial bubble shape for Bond number 0.001
- Place this file in the same directory as `burst_evp.c`
- For details of how this shape is obtained, please refer to repo: [VatsalSy/Bursting-Bubble-In-a-Viscoplastic-Medium](https://github.com/VatsalSy/Bursting-Bubble-In-a-Viscoplastic-Medium). 

### Running Simulations

1. Compile the code with OpenMP support:
```bash
# Compile the code
qcc -O2 -Wall -disable-dimensions -fopenmp burst_evp.c -o burst_evp -lm

# Set the number of OpenMP threads
export OMP_NUM_THREADS=4

# Run the executable with Plastocapillary (J) and Deborah (Deb) numbers
./burst_evp 1.0 0.5  # Example: J=1.0, Deb=0.5
```

## Outputs

### Output Files

The simulation generates several output files:
- `intermediate/snapshot-*.dat`: Simulation state at regular intervals
- `timestep.txt`: Time stepping information
- `log`: Contains kinetic energy and other diagnostic data
- `01_pp/png/`: Directory for PNG output files
- `01_pp/pdf/`: Directory for PDF output files

## Visualization and Post-processing

The repository includes supplementary materials:
- `02_videos/`: Contains 66 videos showing bubble dynamics
- `03_supplementary_plots/`: 79 plots analyzing various aspects
- `04_graphical_abstract/`: Visual summary of key findings


## Contact

If you need some additional data that might be of interest to you, please don't hesitate to contact us at:\
``Arivazhagan G B`` [![](https://img.shields.io/badge/Mail-blue?style=flat&logo=microsoftoutlook&link=mailto:argb@mech.kth.se)](mailto:argb@mech.kth.se) [![](https://img.shields.io/badge/Scholar-4b4b4b?style=flat&logo=googlescholar&link=https://scholar.google.com/citations?user=xyheRZ8AAAAJ&hl=en)](https://scholar.google.com/citations?user=xyheRZ8AAAAJ&hl=en) [![](https://img.shields.io/badge/LinkedIn-blue?style=flat&logo=linkedin&link=https://www.linkedin.com/in/arivazhagan-geetha-balasubramanian-648b8567/)](https://www.linkedin.com/in/arivazhagan-geetha-balasubramanian-648b8567/)\
``Vatsal Sanjay`` [![](https://img.shields.io/badge/Mail-blue?style=flat&logo=microsoftoutlook&link=mailto:vatsalsanjay@gmail.com)](mailto:vatsalsanjay@gmail.com) [![](https://img.shields.io/badge/Scholar-4b4b4b?style=flat&logo=googlescholar&link=https://scholar.google.com/citations?user=67aQviYAAAAJ&hl=en&oi=ao)](https://scholar.google.com/citations?user=67aQviYAAAAJ&hl=en&oi=ao) [![](https://img.shields.io/badge/LinkedIn-blue?style=flat&logo=linkedin&link=https://www.linkedin.com/in/vatsalsanjay/)](https://www.linkedin.com/in/vatsalsanjay/)\
``Outi Tammisola`` [![](https://img.shields.io/badge/Mail-blue?style=flat&logo=microsoftoutlook&link=mailto:outi@mech.kth.se)](mailto:outi@mech.kth.se) [![](https://img.shields.io/badge/Scholar-4b4b4b?style=flat&logo=googlescholar&link=https://scholar.google.com/citations?user=XSKb9YAAAAAJ&hl=en&oi=ao)](https://scholar.google.com/citations?user=XSKb9YAAAAAJ&hl=en&oi=ao) [![](https://img.shields.io/badge/LinkedIn-blue?style=flat&logo=linkedin&link=https://www.linkedin.com/in/outi-tammisola-8b2b6511/)](https://www.linkedin.com/in/outi-tammisola-8b2b6511/)
