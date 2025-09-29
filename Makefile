CXX := clang++
CXXFLAGS := -O2 -Wall -std=c++17

SRC_DIR := src
OUT_DIR := output

.PHONY: run

TARGETS := $(filter-out run,$(MAKECMDGOALS))

clean:
	rm -r ./$(OUT_DIR)/

% : $(OUT_DIR)/%
	@echo "RUNNING $<"
	@./$<

$(OUT_DIR)/% : $(SRC_DIR)/%.cpp
	@echo COMPILING $<
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

.PRECIOUS: $(OUT_DIR)/%