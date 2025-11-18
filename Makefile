# Makefile for Bubble Bursting in Elasto-Viscoplastic Medium
#
# This project uses the Basilisk framework for computational fluid dynamics
# See: http://basilisk.fr

# Compiler and flags
# Note: Basilisk uses 'qcc' wrapper around gcc
QCC = qcc
CFLAGS = -O2 -Wall
BASILISK_FLAGS = -grid=multigrid -D_MPI=0

# Directories
SRC_DIR = 01_code
TOOLS_DIR = 04_graphical_abstract
BUILD_DIR = build

# Source files
MAIN_SRC = $(SRC_DIR)/burst_evp.c
RESTORE_DATA_SRC = $(TOOLS_DIR)/restore_data.c
RESTORE_INDEX_SRC = $(TOOLS_DIR)/restore_index.c

# Header files
HEADERS = $(SRC_DIR)/log-conform-EVP.h \
          $(SRC_DIR)/log-conform-EVP-data.h \
          $(SRC_DIR)/saramito-EVP.h \
          $(SRC_DIR)/fene-p-EVP.h \
          $(SRC_DIR)/adapt_wavelet_limited.h

# Output executables
BURST_EVP = burst_evp
RESTORE_DATA = restore_data
RESTORE_INDEX = restore_index

# Default parameters for testing
J ?= 0.5
DEB ?= 0.5

# Default target
.PHONY: all
all: check-basilisk $(BURST_EVP)

# Check if Basilisk is installed
.PHONY: check-basilisk
check-basilisk:
	@command -v $(QCC) >/dev/null 2>&1 || { \
		echo "ERROR: Basilisk qcc compiler not found!"; \
		echo ""; \
		echo "Basilisk framework is required to compile this project."; \
		echo "Please install Basilisk from: http://basilisk.fr/src/INSTALL"; \
		echo ""; \
		echo "After installation, ensure 'qcc' is in your PATH."; \
		exit 1; \
	}
	@echo "Basilisk found: $$(which $(QCC))"

# Main simulation executable
$(BURST_EVP): $(MAIN_SRC) $(HEADERS)
	@echo "Building main simulation: $(BURST_EVP)"
	@mkdir -p $(BUILD_DIR)
	cd $(SRC_DIR) && $(QCC) $(CFLAGS) -o ../$(BUILD_DIR)/$(BURST_EVP) burst_evp.c -lm
	@echo "Built: $(BUILD_DIR)/$(BURST_EVP)"

# Post-processing tools
.PHONY: tools
tools: $(RESTORE_DATA) $(RESTORE_INDEX)

$(RESTORE_DATA): $(RESTORE_DATA_SRC) $(HEADERS)
	@echo "Building data restoration tool: $(RESTORE_DATA)"
	@mkdir -p $(BUILD_DIR)
	cd $(SRC_DIR) && $(QCC) $(CFLAGS) -o ../$(BUILD_DIR)/$(RESTORE_DATA) ../$(RESTORE_DATA_SRC) -lm
	@echo "Built: $(BUILD_DIR)/$(RESTORE_DATA)"

$(RESTORE_INDEX): $(RESTORE_INDEX_SRC) $(HEADERS)
	@echo "Building index calculation tool: $(RESTORE_INDEX)"
	@mkdir -p $(BUILD_DIR)
	cd $(SRC_DIR) && $(QCC) $(CFLAGS) -o ../$(BUILD_DIR)/$(RESTORE_INDEX) ../$(RESTORE_INDEX_SRC) -lm
	@echo "Built: $(BUILD_DIR)/$(RESTORE_INDEX)"

# Run simulation with default parameters
.PHONY: run
run: $(BURST_EVP)
	@echo "Running simulation with J=$(J), Deb=$(DEB)"
	cd $(BUILD_DIR) && ./$(BURST_EVP) $(J) $(DEB)

# Test build (compile only, don't run)
.PHONY: test
test: all tools
	@echo "All components built successfully!"

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)
	rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*.s $(SRC_DIR)/_burst_evp.c
	rm -f $(TOOLS_DIR)/*.o $(TOOLS_DIR)/*.s
	rm -f *.o *.s _*.c
	@echo "Clean complete"

# Deep clean (including simulation outputs)
.PHONY: distclean
distclean: clean
	@echo "Removing simulation output files..."
	rm -f dump* out log-* core.*
	rm -f *.dat *.ppm *.mp4
	@echo "Deep clean complete"

# Help target
.PHONY: help
help:
	@echo "Bubble Bursting in Elasto-Viscoplastic Medium - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Build main simulation (burst_evp)"
	@echo "  make all          - Same as 'make'"
	@echo "  make tools        - Build post-processing tools (restore_data, restore_index)"
	@echo "  make test         - Build all components (simulation + tools)"
	@echo "  make run          - Build and run simulation with default parameters"
	@echo "                      Use: make run J=0.5 DEB=0.5"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make distclean    - Remove build artifacts and simulation outputs"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Parameters for 'make run':"
	@echo "  J    - Plastocapillary number (default: 0.5)"
	@echo "  DEB  - Deborah number (default: 0.5)"
	@echo ""
	@echo "Requirements:"
	@echo "  - Basilisk framework (http://basilisk.fr)"
	@echo "  - GCC compiler with OpenMP support"
	@echo ""
	@echo "Example usage:"
	@echo "  make              # Build simulation"
	@echo "  make run J=1.0 DEB=1.0  # Run with custom parameters"
	@echo "  make tools        # Build post-processing utilities"

# Install (copy to standard location - optional)
.PHONY: install
install: all
	@echo "Install target not yet implemented"
	@echo "Executables are in: $(BUILD_DIR)/"

.DEFAULT_GOAL := all
