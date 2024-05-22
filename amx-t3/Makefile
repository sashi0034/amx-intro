CXX = icpc
CXXFLAGS = -std=gnu++17 -S -fsource-asm -masm=intel -O3 -xCORE-AVX512 -qopt-zmm-usage=high
CXXFLAGS_BIN = -std=gnu++17 -O3 -xCORE-AVX512 -qopt-zmm-usage=high
SRC_DIR = src
BUILD_DIR = build
ASM_DIR = $(BUILD_DIR)/asm
BIN_DIR = $(BUILD_DIR)/bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
ASM_FILES = $(SOURCES:$(SRC_DIR)/%.cpp=$(ASM_DIR)/%.asm)
BINARIES = $(BIN_DIR)/amx-t3

all: $(ASM_DIR) $(ASM_FILES) $(BINARIES)

bin: $(BIN_DIR) $(BINARIES)

$(ASM_DIR):
	mkdir -p $(ASM_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(ASM_DIR)/%.asm: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR)/amx-t3: $(BIN_DIR) $(SOURCES)
	$(CXX) $(CXXFLAGS_BIN) $(SOURCES) -o $@

clean:
	rm -f $(ASM_DIR)/*.asm
	rm -f $(BIN_DIR)/*
