/**
## Log-conformation EVP header for data restoration tools

This is a minimal header file for post-processing and data restoration tools.
It includes the necessary components from the main log-conform-EVP implementation
without the full event system, making it suitable for data extraction utilities.

For the full implementation, see log-conform-EVP.h
*/

#include "log-conform-EVP.h"

/**
This header simply includes the main log-conform-EVP.h file, which contains
all the necessary definitions for:
- tau_p (polymeric stress tensor)
- tau_qq (additional stress component for axisymmetric cases)
- trA (trace of conformation tensor)
- solidreg (field marking yielded/unyielded regions)
- lambda, mup, tau0 (material parameters)

These are used by the restore utilities to properly interpret simulation data.
*/
