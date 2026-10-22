# RocketMod - Rocket Launchers-Only Plugin
# Copyright 2004-2026
# https://github.com/thecybermind/rocketmod_qmm/
# 3-clause BSD license: https://opensource.org/license/bsd-3-clause
# Created By: Kevin Masterson < k.m.masterson@gmail.com >

BIN_32 := rocketmod_qmm
BIN_64 := rocketmod_qmm_x86_64

CC := g++

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=%.o)

CPPFLAGS := -MMD -MP -I ./include -isystem ../qmm_sdks -isystem ../qmm2/include
CFLAGS   := -Wall -pipe -fPIC
LDFLAGS  := -shared -fPIC
LDLIBS   :=

REL_CPPFLAGS := $(CPPFLAGS) -DNDEBUG
DBG_CPPFLAGS := $(CPPFLAGS) -D_DEBUG

REL_CFLAGS_32 := $(CFLAGS) -m32 -O2 -falign-functions=16 -fno-strict-aliasing -fstrength-reduce 
REL_CFLAGS_64 := $(CFLAGS) -O2 -falign-functions=16 -fno-strict-aliasing -fstrength-reduce 
DBG_CFLAGS_32 := $(CFLAGS) -m32 -g -pg
DBG_CFLAGS_64 := $(CFLAGS) -g -pg

REL_LDFLAGS_32 := $(LDFLAGS) -m32
REL_LDFLAGS_64 := $(LDFLAGS)
DBG_LDFLAGS_32 := $(LDFLAGS) -m32 -g -pg
DBG_LDFLAGS_64 := $(LDFLAGS) -g -pg

.PHONY: help all clean release debug release32 debug32 release64 debug64 $(addprefix game-,$(GAMES)) $(addprefix release-,$(GAMES)) $(addprefix debug-,$(GAMES))

help:
	@echo make targets:
	@echo all: release debug
	@echo release: release32 release64
	@echo release32: [32-bit release builds]
	@echo release64: [64-bit release builds]
	@echo debug: debug32 debug64
	@echo debug32: [32-bit debug builds]
	@echo debug64: [64-bit debug builds]
	@echo game-[GAME]: release-[GAME] debug-[GAME]
	@echo release-[GAME]: release32-[GAME] release64-[GAME]
	@echo debug-[GAME]: debug32-[GAME] debug64-[GAME]
	@echo release32-[GAME]: [32-bit release build for GAME]
	@echo release64-[GAME]: [64-bit release build for GAME]
	@echo debug32-[GAME]: [32-bit debug build for GAME]
	@echo debug64-[GAME]: [64-bit release build for GAME]

all: release debug
release: release32 release64
release32: release32-Q3A release32-QUAKE2
release64: release64-Q3A release64-QUAKE2
debug: debug32 debug64
debug32: debug32-Q3A debug32-QUAKE2
debug64: debug64-Q3A debug64-QUAKE2
game-Q3A: release-Q3A debug-Q3A
release-Q3A: release32-Q3A release64-Q3A
debug-Q3A: debug32-Q3A debug64-Q3A
release32-Q3A: $(BIN_DIR)/release-Q3A/x86/$(BIN_32)_Q3A.so
debug32-Q3A: $(BIN_DIR)/debug-Q3A/x86/$(BIN_32)_Q3A.so

$(BIN_DIR)/release-Q3A/x86/$(BIN_32)_Q3A.so: $(addprefix $(OBJ_DIR)/release-Q3A/x86/,$(OBJ_FILES))
	mkdir -p $(@D)
	$(CC) $(REL_LDFLAGS_32) -o $@ $(LDLIBS) $^
	
$(BIN_DIR)/debug-Q3A/x86/$(BIN_32)_Q3A.so: $(addprefix $(OBJ_DIR)/debug-Q3A/x86/,$(OBJ_FILES))
	mkdir -p $(@D)
	$(CC) $(DBG_LDFLAGS_32) -o $@ $(LDLIBS) $^

$(OBJ_DIR)/release-Q3A/x86/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(REL_CPPFLAGS) -DGAME_Q3A -DGAME_STR=\"Q3A\" $(REL_CFLAGS_32) -c $< -o $@

$(OBJ_DIR)/debug-Q3A/x86/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(DBG_CPPFLAGS) -DGAME_Q3A -DGAME_STR=\"Q3A\" $(DBG_CFLAGS_32) -c $< -o $@

-include $(addprefix $(OBJ_DIR)/release-Q3A/x86/,$(OBJ_FILES:.o=.d))
-include $(addprefix $(OBJ_DIR)/debug-Q3A/x86/,$(OBJ_FILES:.o=.d))

release64-Q3A: $(BIN_DIR)/release-Q3A/x86_64/$(BIN_64)_Q3A.so
debug64-Q3A: $(BIN_DIR)/debug-Q3A/x86_64/$(BIN_64)_Q3A.so

$(BIN_DIR)/release-Q3A/x86_64/$(BIN_64)_Q3A.so: $(addprefix $(OBJ_DIR)/release-Q3A/x86_64/,$(OBJ_FILES))
	mkdir -p $(@D)
	$(CC) $(REL_LDFLAGS_64) -o $@ $(LDLIBS) $^	

$(BIN_DIR)/debug-Q3A/x86_64/$(BIN_64)_Q3A.so: $(addprefix $(OBJ_DIR)/debug-$(1)/x86_64/,$(OBJ_FILES))
	mkdir -p $(@D)
	$(CC) $(DBG_LDFLAGS_64) -o $@ $(LDLIBS) $^

$(OBJ_DIR)/release-Q3A/x86_64/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(REL_CPPFLAGS) -DGAME_Q3A -DGAME_STR=\"Q3A\" $(REL_CFLAGS_64) -c $< -o $@

$(OBJ_DIR)/debug-Q3A/x86_64/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(DBG_CPPFLAGS) -DGAME_Q3A -DGAME_STR=\"Q3A\" $(DBG_CFLAGS_64) -c $< -o $@

-include $(addprefix $(OBJ_DIR)/release-Q3A/x86_64/,$(OBJ_FILES:.o=.d))
-include $(addprefix $(OBJ_DIR)/debug-Q3A/x86_64/,$(OBJ_FILES:.o=.d))

game-QUAKE2: release-QUAKE2 debug-QUAKE2
release-QUAKE2: release32-QUAKE2 release64-QUAKE2
debug-QUAKE2: debug32-QUAKE2 debug64-QUAKE2
release32-QUAKE2: $(BIN_DIR)/release-QUAKE2/x86/$(BIN_32)_QUAKE2.so
debug32-QUAKE2: $(BIN_DIR)/debug-QUAKE2/x86/$(BIN_32)_QUAKE2.so

$(BIN_DIR)/release-QUAKE2/x86/$(BIN_32)_QUAKE2.so: $(addprefix $(OBJ_DIR)/release-QUAKE2/x86/,$(OBJ_FILES))
	mkdir -p $(@D)
	$(CC) $(REL_LDFLAGS_32) -o $@ $(LDLIBS) $^
	
$(BIN_DIR)/debug-QUAKE2/x86/$(BIN_32)_QUAKE2.so: $(addprefix $(OBJ_DIR)/debug-QUAKE2/x86/,$(OBJ_FILES))
	mkdir -p $(@D)
	$(CC) $(DBG_LDFLAGS_32) -o $@ $(LDLIBS) $^

$(OBJ_DIR)/release-QUAKE2/x86/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(REL_CPPFLAGS) -DGAME_QUAKE2 -DGAME_STR=\"QUAKE2\" $(REL_CFLAGS_32) -c $< -o $@

$(OBJ_DIR)/debug-QUAKE2/x86/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(DBG_CPPFLAGS) -DGAME_QUAKE2 -DGAME_STR=\"QUAKE2\" $(DBG_CFLAGS_32) -c $< -o $@

-include $(addprefix $(OBJ_DIR)/release-QUAKE2/x86/,$(OBJ_FILES:.o=.d))
-include $(addprefix $(OBJ_DIR)/debug-QUAKE2/x86/,$(OBJ_FILES:.o=.d))

release64-QUAKE2: $(BIN_DIR)/release-QUAKE2/x86_64/$(BIN_64)_QUAKE2.so
debug64-QUAKE2: $(BIN_DIR)/debug-QUAKE2/x86_64/$(BIN_64)_QUAKE2.so

$(BIN_DIR)/release-QUAKE2/x86_64/$(BIN_64)_QUAKE2.so: $(addprefix $(OBJ_DIR)/release-QUAKE2/x86_64/,$(OBJ_FILES))
	mkdir -p $(@D)
	$(CC) $(REL_LDFLAGS_64) -o $@ $(LDLIBS) $^	

$(BIN_DIR)/debug-QUAKE2/x86_64/$(BIN_64)_QUAKE2.so: $(addprefix $(OBJ_DIR)/debug-$(1)/x86_64/,$(OBJ_FILES))
	mkdir -p $(@D)
	$(CC) $(DBG_LDFLAGS_64) -o $@ $(LDLIBS) $^

$(OBJ_DIR)/release-QUAKE2/x86_64/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(REL_CPPFLAGS) -DGAME_QUAKE2 -DGAME_STR=\"QUAKE2\" $(REL_CFLAGS_64) -c $< -o $@

$(OBJ_DIR)/debug-QUAKE2/x86_64/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(DBG_CPPFLAGS) -DGAME_QUAKE2 -DGAME_STR=\"QUAKE2\" $(DBG_CFLAGS_64) -c $< -o $@

-include $(addprefix $(OBJ_DIR)/release-QUAKE2/x86_64/,$(OBJ_FILES:.o=.d))
-include $(addprefix $(OBJ_DIR)/debug-QUAKE2/x86_64/,$(OBJ_FILES:.o=.d))

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)
