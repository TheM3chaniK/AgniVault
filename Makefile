# ────────────────────────────────────────────────
# Compiler and flags
# ────────────────────────────────────────────────
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -Iinclude
LDFLAGS := -lsodium -lsqlite3 -lyaml-cpp

ifeq ($(DEBUG),1)
    CXXFLAGS += -g -DDEBUG -O0
endif


# ────────────────────────────────────────────────
# Directories
# ────────────────────────────────────────────────
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# Output binary
TARGET := $(BIN_DIR)/agniv

# ────────────────────────────────────────────────
# Source and object files
# ────────────────────────────────────────────────
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# ────────────────────────────────────────────────
# Build rules
# ────────────────────────────────────────────────
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "[+] Linking $(TARGET)"
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "[+] Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ────────────────────────────────────────────────
# Utility targets
# ────────────────────────────────────────────────
run: $(TARGET)
	@echo "[+] Running AgniVault..."
	@$(TARGET)

clean:
	@echo "[+] Cleaning build files..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

rebuild: clean all

# ────────────────────────────────────────────────
# End of Makefile
# ────────────────────────────────────────────────

