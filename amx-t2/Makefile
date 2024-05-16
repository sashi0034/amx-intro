# Makefile to generate assembly files

CXX = icpc
CXXFLAGS = -O3 -std=gnu++17 -S -fsource-asm -masm=intel
SRC_DIR = src
BUILD_DIR = build
ASM_DIR = $(BUILD_DIR)/asm

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
ASM_FILES = $(SOURCES:$(SRC_DIR)/%.cpp=$(ASM_DIR)/%.asm)

all: $(ASM_DIR) $(ASM_FILES)

$(ASM_DIR):
	mkdir -p $(ASM_DIR)

$(ASM_DIR)/%.asm: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(ASM_DIR)/*.asm
