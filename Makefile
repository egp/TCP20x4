# Makefile v7
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror

SRC_DIR := src
TEST_DIR := tests/host
BUILD_DIR := build

CORE_SRC := $(SRC_DIR)/TCP20x4Core.cpp
CORE_HDR := $(SRC_DIR)/TCP20x4Core.h $(SRC_DIR)/TCP20x4Status.h

CONTROLLER_SRC := $(SRC_DIR)/TCP20x4Controller.cpp
CONTROLLER_HDR := $(SRC_DIR)/TCP20x4Controller.h $(SRC_DIR)/TCP20x4Device.h

TRANSPORT_SRC := $(SRC_DIR)/TCP20x4Pcf8574Transport.cpp
TRANSPORT_HDR := $(SRC_DIR)/TCP20x4Pcf8574Transport.h $(SRC_DIR)/TCP20x4Pcf8574Config.h $(SRC_DIR)/TCP20x4PinMap.h

TEST_BINS := \
	$(BUILD_DIR)/test_WB_tcp20x4_core_initial_state \
	$(BUILD_DIR)/test_WB_tcp20x4_core_write_line \
	$(BUILD_DIR)/test_WB_tcp20x4_core_clear \
	$(BUILD_DIR)/test_WB_tcp20x4_core_display_state \
	$(BUILD_DIR)/test_BB_tcp20x4_controller_api \
	$(BUILD_DIR)/test_WB_tcp20x4_pcf8574_transport

.PHONY: all test clean check-files

all: test

check-files:
	@test -f $(CORE_SRC)
	@test -f $(CONTROLLER_SRC)
	@test -f $(TRANSPORT_SRC)
	@test -f $(TEST_DIR)/test_WB_tcp20x4_core_initial_state.cpp
	@test -f $(TEST_DIR)/test_WB_tcp20x4_core_write_line.cpp
	@test -f $(TEST_DIR)/test_WB_tcp20x4_core_clear.cpp
	@test -f $(TEST_DIR)/test_WB_tcp20x4_core_display_state.cpp
	@test -f $(TEST_DIR)/test_BB_tcp20x4_controller_api.cpp
	@test -f $(TEST_DIR)/test_WB_tcp20x4_pcf8574_transport.cpp

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/test_WB_tcp20x4_core_initial_state: check-files \
	$(TEST_DIR)/test_WB_tcp20x4_core_initial_state.cpp \
	$(CORE_SRC) $(CORE_HDR) | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -I $(SRC_DIR) \
		$(TEST_DIR)/test_WB_tcp20x4_core_initial_state.cpp \
		$(CORE_SRC) \
		-o $(BUILD_DIR)/test_WB_tcp20x4_core_initial_state

$(BUILD_DIR)/test_WB_tcp20x4_core_write_line: check-files \
	$(TEST_DIR)/test_WB_tcp20x4_core_write_line.cpp \
	$(CORE_SRC) $(CORE_HDR) | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -I $(SRC_DIR) \
		$(TEST_DIR)/test_WB_tcp20x4_core_write_line.cpp \
		$(CORE_SRC) \
		-o $(BUILD_DIR)/test_WB_tcp20x4_core_write_line

$(BUILD_DIR)/test_WB_tcp20x4_core_clear: check-files \
	$(TEST_DIR)/test_WB_tcp20x4_core_clear.cpp \
	$(CORE_SRC) $(CORE_HDR) | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -I $(SRC_DIR) \
		$(TEST_DIR)/test_WB_tcp20x4_core_clear.cpp \
		$(CORE_SRC) \
		-o $(BUILD_DIR)/test_WB_tcp20x4_core_clear

$(BUILD_DIR)/test_WB_tcp20x4_core_display_state: check-files \
	$(TEST_DIR)/test_WB_tcp20x4_core_display_state.cpp \
	$(CORE_SRC) $(CORE_HDR) | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -I $(SRC_DIR) \
		$(TEST_DIR)/test_WB_tcp20x4_core_display_state.cpp \
		$(CORE_SRC) \
		-o $(BUILD_DIR)/test_WB_tcp20x4_core_display_state

$(BUILD_DIR)/test_BB_tcp20x4_controller_api: check-files \
	$(TEST_DIR)/test_BB_tcp20x4_controller_api.cpp \
	$(CORE_SRC) $(CORE_HDR) $(CONTROLLER_SRC) $(CONTROLLER_HDR) | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -I $(SRC_DIR) \
		$(TEST_DIR)/test_BB_tcp20x4_controller_api.cpp \
		$(CORE_SRC) \
		$(CONTROLLER_SRC) \
		-o $(BUILD_DIR)/test_BB_tcp20x4_controller_api

$(BUILD_DIR)/test_WB_tcp20x4_pcf8574_transport: check-files \
	$(TEST_DIR)/test_WB_tcp20x4_pcf8574_transport.cpp \
	$(TRANSPORT_SRC) $(TRANSPORT_HDR) | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -I $(SRC_DIR) \
		$(TEST_DIR)/test_WB_tcp20x4_pcf8574_transport.cpp \
		$(TRANSPORT_SRC) \
		-o $(BUILD_DIR)/test_WB_tcp20x4_pcf8574_transport

test: $(TEST_BINS)
	@./$(BUILD_DIR)/test_WB_tcp20x4_core_initial_state
	@./$(BUILD_DIR)/test_WB_tcp20x4_core_write_line
	@./$(BUILD_DIR)/test_WB_tcp20x4_core_clear
	@./$(BUILD_DIR)/test_WB_tcp20x4_core_display_state
	@./$(BUILD_DIR)/test_BB_tcp20x4_controller_api
	@./$(BUILD_DIR)/test_WB_tcp20x4_pcf8574_transport

clean:
	@rm -rf $(BUILD_DIR)
# Makefile v7