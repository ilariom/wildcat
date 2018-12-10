TARGET_EXEC ?= ../bin/wildcat_app

BUILD_DIR ?= ../obj
SRC_DIRS ?= .

SRCS := $(shell find $(SRC_DIRS) -name "*.cc")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d) /usr/ /usr/local/
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
SDL_DEPS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
CXX_STD := --std=c++14

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(CXX_STD) $(OBJS) -g -o $@ $(LDFLAGS) $(SDL_DEPS)

# c++ source
$(BUILD_DIR)/%.cc.o: %.cc
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CXX_STD) $(CPPFLAGS) $(CXXFLAGS) -c $< -g -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p