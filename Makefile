
#
# Intro
#

$(info [ PsychoX' Makefile - version 2.4.0 ])

# Special variables
.SUFFIXES:
NULL :=
SPACE := ${NULL} ${NULL}
${SPACE} := ${SPACE}
TAB := ${NULL}	${NULL}
${TAB} := ${TAB}
define \n


endef
NEWLINE := ${\n}

# More settings file
SETTINGS_FILE := ./settings.mk

# Platform detection
PLATFORM_DETECT := $(shell uname -o | tr A-Z a-z)
ifeq ($(findstring linux,$(PLATFORM_DETECT)),)
    PLATFORM := win
else
    PLATFORM := lin
endif

# Architecture detection
ARCH_DETECT := $(shell uname -m)
ifeq ($(findstring 64,$(ARCH_DETECT)),)
    ARCH := 32
else
    ARCH := 64
endif



#
# Target selection/detection
#

# Target normalize function
define normalize_target
    # There MUST be so many 'eval's, because Makefile interpreter will try "optimalize" uses of variables...
    $(eval override TARGET := $(shell echo $(TARGET) | tr A-Z a-z))
    $(eval override TARGET_PLATFORM := $(findstring win,$(TARGET))$(findstring lin,$(TARGET)))
    $(eval override TARGET_ARCH     := $(findstring 32,$(TARGET))$(findstring 64,$(TARGET)))
    $(eval override TARGET:=$(TARGET_PLATFORM)$(TARGET_ARCH))
endef

# Try to get target from settings file if no selected
ifndef TARGET
    ifneq ("$(wildcard $(SETTINGS_FILE))","")
        TARGET := $(shell grep -Po '^(?:TARGET[ ]*[:]?=[ ]*)\K.*$$' $(SETTINGS_FILE))
    endif
endif

# Try to use system platfrom and arch if no selected
ifndef TARGET
    TARGET:=$(PLATFORM)$(ARCH)
endif

# Target normalization
ifdef TARGET
    $(eval $(call normalize_target))
endif



#
# Default configuration
#

# Output/runtime file name
OUT_NAME := Szczur
RUN_NAME := Szczur

# Run params
PARAMS := 

# Folders structure
INC_DIRS := src
SRC_DIRS := src
TEP_DIRS := src
OBJ_DIR := obj/$(TARGET)
OUT_DIR := out
RUN_DIR := out

# Extensions
SRC_EXTS := .cpp .c
INC_EXTS := .hpp .h
TEP_EXTS := .tpp .t
OBJ_EXT := .o
OUT_EXT := .exe
RUN_EXT := .exe

# Compiler and linker 
CXX      := g++ 
CXXFLAGS := -std=c++17 -Wall
LD       := g++
LDFLAGS  :=

# Using MXE? and its options
MXE := no
MXE_DIR := /usr/lib/mxe

# Arch tags
ARCH_32 := i686
ARCH_64 := x86_64

# Use static linking
LINKING := static
CXXFLAGS_STATIC  := -static
CXXFLAGS_DYNAMIC :=
 LDFLAGS_STATIC  := -static
 LDFLAGS_DYNAMIC :=

# Optimalization
OPTIMALIZE := no
CXXFLAGS_OPTIMALIZATION := -flto -ffat-lto-objects -O3
 LDFLAGS_OPTIMALIZATION := -flto -ffat-lto-objects -O3

# Debbuging
DEBUGGER := none

# Cleaning
ifeq ($(MAKECMDGOALS),clean)
    CLEAN_FILES_CMD := rm 
    CLEAN_FILES := "./$(OBJ_DIR)/*/*$(OBJ_EXT)" "./$(OUT_DIR)/$(OUT_NAME)$(OUT_EXT)"
    CLEAN_DIRS_CMD := rm -dr 
    CLEAN_DIRS  := "./$(OBJ_DIR)/*"
endif

# Other tools
LS := ls -AdoGh --time-style long-iso
MKDIR = mkdir -p 

# Libraries 
LIB_LIST := IMGUI SFML BOOST LUA
#   SFML
 CXXFLAGS_STATIC_SFML   := -DSFML_STATIC
  LDFLAGS_STATIC_SFML   := -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -ljpeg -lopengl32 -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32 -lwinmm -DSFML_STATIC
CXXFLAGS_DYNAMIC_SFML   :=
 LDFLAGS_DYNAMIC_SFML   := -lsfml-audio   -lsfml-graphics   -lsfml-window   -lsfml-system   -lopengl32 -lfreetype -ljpeg -lopengl32 -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32 -lwinmm
 MXE_PACKAGENAME_SFML   := sfml
#   Boost
 CXXFLAGS_STATIC_BOOST  :=
  LDFLAGS_STATIC_BOOST  :=
CXXFLAGS_DYNAMIC_BOOST  :=
 LDFLAGS_DYNAMIC_BOOST  :=
 MXE_PACKAGENAME_BOOST  := boost
#  Lua
 CXXFLAGS_STATIC_LUA    :=
  LDFLAGS_STATIC_LUA    := -llua
CXXFLAGS_DYNAMIC_LUA    :=
 LDFLAGS_DYNAMIC_LUA    := -llua
 MXE_PACKAGENAME_LUA    := lua
#   ImGUI 
 CXXFLAGS_STATIC_IMGUI  := 
  LDFLAGS_STATIC_IMGUI  := -limgui 
CXXFLAGS_DYNAMIC_IMGUI  := 
 LDFLAGS_DYNAMIC_IMGUI  := -limgui

# Dynamic libraries (.so/.dll) @todo kiedyś może to zrobię :D zeby COPY_BIN kopiowalo dll/so ;)
FILES_DYNAMIC_SFML := openal32.dll sfml*

# Header-only libs
HEADER_LIB_LIST := SOL2 JSON GLM
HEADER_INC_SOL2 := 3rd-party/sol2
HEADER_INC_JSON := 3rd-party/json
HEADER_INC_SOL2 := 3rd-party/glm

# Print definitions
COLORS := no

# Markers to grep code with
MARKERS := @todo @warn @err @debug

# Dependencies finding
DEPFLAGS := -MT $$@ -MMD -MP -MF 
DEP_EXT := .d
DEP_TMP_UPDATE_SUFFIX := .tmp



#
# Load settings override
#

# Definition to overwrite if we want add more rules in settings
define more_rules
more_rules:
endef

# Include
ifneq ("$(wildcard $(SETTINGS_FILE))","")
    include $(SETTINGS_FILE)
endif



#
# Cleaning related script
#

ifeq ($(MAKECMDGOALS),clean)

# Files and folders to clean out
CLEAN_FILES := $(shell find . -type f -path $(subst $(SPACE), -or -path ,$(CLEAN_FILES)))
CLEAN_DIRS  := $(shell find . -type d -path $(subst $(SPACE), -or -path ,$(CLEAN_DIRS)) | tac)

# Cleaning the compile environment
.PHONY: clean
clean: 
	$(inform_cleaning)
ifeq ($(or $(CLEAN_FILES),$(CLEAN_DIRS),$(CLEAN_FORCE)),) 
	@echo "Nothing to clean."
else
	$(V)-$(CLEAN_FILES_CMD) $(CLEAN_FILES) $(SLIENT) || :
	$(V)-$(CLEAN_DIRS_CMD) $(CLEAN_DIRS) || :
	@echo "Cleaned."
endif

endif



#
# Compiler selection
#

# Linking
override LINKING := $(shell echo $(LINKING) | tr a-z A-Z)
CXXFLAGS += $(CXXFLAGS_$(LINKING))
LDFLAGS  += $( LDFLAGS_$(LINKING))

# Default cross compiler
ifeq ($(CROSS),)
    CROSS := yes
endif
ifeq ($(CROSS),yes)
    # Architecture 
    CROSS := $(ARCH_$(TARGET_ARCH))
    ifeq ($(CROSS),)
        $(error "Target architecture not selected propertly... TARGET_ARCH=$(TARGET_ARCH)")
    endif

    # Platform
    # -> Windows...
    ifeq ($(TARGET_PLATFORM),win)
        # Select MINGW-W64
        CROSS := $(CROSS)-w64-mingw32
        # MXE support
        ifeq ($(MXE),yes)
            # Add MXE as cross-target compiling env
            MXE_BIN_DIR := $(MXE_DIR)/usr/bin
            PATH := $(PATH):$(MXE_BIN_DIR)
            # Make shell use the specified in the makefile path
            SHELL = env PATH='$(PATH)' /bin/bash
            # Select static/shared compilator.
            ifeq ($(LINKING),STATIC)
                CROSS := $(CROSS).static
            else
                CROSS := $(CROSS).shared
            endif
        endif
    # -> Linux...
    else ifeq ($(TARGET_PLATFORM),lin)
        # Select Linux GNU
        CROSS := $(CROSS)-linux-gnu
    # -> Invaild platform
    else
        $(error "Target platform not selected propertly... TARGET_PLATFORM=$(TARGET_PLATFORM)")
    endif
    CROSS := $(CROSS)-

    # If true 32 bit compiler not present, use universal
    ifeq ($(shell bash -c "command -v $(CROSS)$(CXX)"),)
        CROSS := $(subst i686,x86_64,$(CROSS))
        $(warning "Using universal compiler... CROSS=$(CROSS)")
    endif
    
    # If no selective compiler, just try use 'g++'...
    ifeq ($(shell bash -c "command -v $(CROSS)$(CXX)"),)
        CROSS :=
        $(warning "Using standard system compiler... (i.e. g++)")
    endif
endif

# Use default system compiler
ifeq ($(CROSS),no)
    CROSS :=
endif
ifeq ($(CROSS),none)
    CROSS :=
endif

# Finally select compiler and linker
CXX := $(CROSS)$(CXX)
LD  := $(CROSS)$(LD)



#
# Library flags selection
#

# Compiled libs
define add_lib
    ifeq ($(and $(findstring $(TARGET_PLATFORM),win),$(findstring $(MXE),yes),$(MXE_PACKAGENAME_$(1)),1),1)
        # Use MXE packages
        MXE_PKGS += $(MXE_PACKAGENAME_$(1))
    else
        # Add linking dependent flags
        CXXFLAGS += $(CXXFLAGS_$(LINKING)_$(1))
        LDFLAGS  +=  $(LDFLAGS_$(LINKING)_$(1))
        # Add includes/libraries object directories
        ifneq ($(INC_DIR_$(1)_$(TARGET_ARCH)),)
            CXXFLAGS += -I$(INC_DIR_$(1)_$(TARGET_ARCH))
        endif
        ifneq ($(LIB_DIR_$(1)_$(TARGET_ARCH)),)
            LDFLAGS  += -L$(LIB_DIR_$(1)_$(TARGET_ARCH))
        endif
    endif
endef
$(foreach LIB_NAME, $(LIB_LIST), $(eval $(call add_lib,$(LIB_NAME))))

# MXE uses pkg-config
ifeq ($(and $(findstring $(TARGET_PLATFORM),win),$(findstring $(MXE),yes),$(LIB_LIST),1),1)
    CXXFLAGS += $(shell $(CROSS)pkg-config --cflags $(MXE_PKGS))
    LDFLAGS  += $(shell $(CROSS)pkg-config --libs $(MXE_PKGS))
endif

# Header-only libs
$(foreach LIB_NAME, $(HEADER_LIB_LIST), $(eval CXXFLAGS += -I$(HEADER_INC_$(LIB_NAME))))



#
# Other 
#

# Colon replacement
COLON_REPLACEMENT := _c0loN

# Selecting target machine architecture
CXXFLAGS += -m$(TARGET_ARCH)
LDFLAGS  += -m$(TARGET_ARCH)

# Adding headers and templates directories to search paths
CXXFLAGS += -I$(subst $(SPACE), -I,$(INC_DIRS))
CXXFLAGS += -I$(subst $(SPACE), -I,$(TEP_DIRS))

# Adding optimalization flags
ifeq ($(OPTIMALIZE),yes)
    CXXFLAGS += $(CXXFLAGS_OPTIMALIZATION)
     LDFLAGS +=  $(LDFLAGS_OPTIMALIZATION)
endif

# Adding debugger flags
ifeq (DEBUGGER,ggdb)
    CXXFLAGS += -ggdb
     LDFLAGS += -ggdb
endif

# Normalize paths 
ifeq ($(PLATFORM),win)
    SRC_DIRS := $(shell echo $(SRC_DIRS) | grep -v '\.\.') $(shell echo $(SRC_DIRS) | grep '\.\.' | xargs -r -L1 realpath | sed -e 's/./:\//3' -e 's/^.//') 
    INC_DIRS := $(shell echo $(INC_DIRS) | grep -v '\.\.') $(shell echo $(INC_DIRS) | grep '\.\.' | xargs -r -L1 realpath | sed -e 's/./:\//3' -e 's/^.//')
    TEP_DIRS := $(shell echo $(TEP_DIRS) | grep -v '\.\.') $(shell echo $(TEP_DIRS) | grep '\.\.' | xargs -r -L1 realpath | sed -e 's/./:\//3' -e 's/^.//')
else
    SRC_DIRS := $(shell echo $(SRC_DIRS) | grep -v '\.\.') $(shell echo $(SRC_DIRS) | grep '\.\.' | xargs -r -L1 realpath) 
    INC_DIRS := $(shell echo $(INC_DIRS) | grep -v '\.\.') $(shell echo $(INC_DIRS) | grep '\.\.' | xargs -r -L1 realpath)
    TEP_DIRS := $(shell echo $(TEP_DIRS) | grep -v '\.\.') $(shell echo $(TEP_DIRS) | grep '\.\.' | xargs -r -L1 realpath)
endif



#
# Informing and colors
#

# Verbose flag
ifeq ($(or $(V),$(VERBOSE)),yes)
    override V :=
else
    override V := @
endif

# Markers grepping function
ifndef grep_markers
    grep_markers := @xd(){ grep -i -C1 '$(subst $(SPACE),\|,$(MARKERS))' $$1 || :; }; xd
endif

# Informations 
ifeq ($(COLORS),yes)
    ifndef inform_executable
        inform_executable   := @printf "\033[32m[Executable] %s\033[0m \n"
    endif
    ifndef inform_object
        inform_object       := @printf "\033[33m[Object] %s\033[0m \n" 
    endif
    ifndef inform_source
        inform_source       := @printf "\033[34m[Source] %s\033[0m \n"
    endif
    ifndef inform_header
        inform_header       := @printf "\033[36m[Header] %s\033[0m \n"
    endif
    ifndef inform_template
        inform_template     := @printf "\033[35m[Template] %s\033[0m \n"
    endif
    ifndef inform_running
        inform_running      := @printf "\033[37m[Running] \033[0m \n"
    endif
    ifndef inform_cleaning
        inform_cleaning     := @printf "\033[37m[Cleaning] \033[0m \n"
    endif
else
    ifndef inform_executable
        inform_executable   := @printf "[Executable] %s \n"
    endif
    ifndef inform_object
        inform_object       := @printf "[Object] %s \n" 
    endif
    ifndef inform_source
        inform_source       := @printf "[Source] %s \n"
    endif
    ifndef inform_header
        inform_header       := @printf "[Header] %s \n"
    endif
    ifndef inform_template
        inform_template     := @printf "[Template] %s \n"
    endif
    ifndef inform_running
        inform_running      := @printf "[Running] \n"
    endif
    ifndef inform_cleaning
        inform_cleaning     := @printf "[Cleaning] \n"
    endif
endif

ifeq ($(COLORS),yes)
    CXXFLAGS += -fdiagnostics-color=always
     LDFLAGS += -fdiagnostics-color=always
endif



#
# Files searching
#

# Output file to be linked to
OUT_FILE := $(OUT_DIR)/$(OUT_NAME)$(OUT_EXT)

# Runtime file to run for tests
RUN_FILE := $(RUN_DIR)/$(RUN_NAME)$(RUN_EXT)

# List of sources, headers and templates
SOURCES   := $(shell find $(SRC_DIRS) -type f -name '*$(subst $(SPACE),' -or -name '*,$(SRC_EXTS))')
HEADERS   := $(shell find $(INC_DIRS) -type f -name '*$(subst $(SPACE),' -or -name '*,$(INC_EXTS))')
TEMPLATES := $(shell find $(TEP_DIRS) -type f -name '*$(subst $(SPACE),' -or -name '*,$(TEP_EXTS))')

# List of objects to be linked
OBJECTS := $(subst :,$(COLON_REPLACEMENT),$(patsubst %,$(OBJ_DIR)/%$(OBJ_EXT),$(SOURCES)))

# List of dependencies files
DEPENDENCIES := $(patsubst %$(OBJ_EXT),%$(DEP_EXT),$(OBJECTS))



#
# Compile recipes
#

# Standard options
.PHONY: default nothing all
defualt: all
nothing: 
all: exe more_rules

# Linking 
.PHONY: exe
exe: $(OUT_FILE)
$(OUT_FILE): $(OBJECTS)
	$(inform_executable) $@
	$(V)$(MKDIR) `dirname $@`
	$(V)$(LD) $(OBJECTS) -o $@ $(LDFLAGS)
	$(V)$(LS) $@

# Compiling
.PHONY: obj
obj: $(OBJECTS)
define recipe
$(eval OBJECT := $(1))
$(eval SOURCE := $(subst $(COLON_REPLACEMENT),:,$(patsubst $(OBJ_DIR)/%$(OBJ_EXT),%,$(OBJECT))))
$(eval DEPENDENT := $(patsubst %$(OBJ_EXT),%$(DEP_EXT),$(OBJECT)))
$(OBJECT): $(SOURCE) $(DEPENDENT)
	$(inform_object) $$@
	$(V)$(MKDIR) `dirname $$@`
	$(V)$(CXX) -c $$< -o $$@ $(CXXFLAGS) $(DEPFLAGS) $(DEPENDENT)$(DEP_TMP_UPDATE_SUFFIX)
	$(V)mv -f $(DEPENDENT)$(DEP_TMP_UPDATE_SUFFIX) $(DEPENDENT) && touch $$@
	$(V)$(LS) $$@
endef
$(foreach _, $(OBJECTS), $(eval $(call recipe,$(_))))



#
# Dependencies recipes
#

# Include deps maps
$(DEPENDENCIES): ;
.PRECIOUS: $(DEPENDENCIES)
ifneq ($(DEPENDENCIES),)
    include $(wildcard $(DEPENDENCIES))
endif



#
# Code check
#

# @warn przynajmniej narazie xd ;f
#ifneq ($(MARKERS),)
ifeq ($(findstring code,$(MAKECMDGOALS)),code)

# Sources
.PHONY: src $(SOURCES)
src: $(SOURCES)
$(SOURCES):
	$(inform_source) $@
	$(grep_markers) $@

# Headers
.PHONY: inc $(HEADERS)
inc: $(HEADERS)
$(HEADERS):
	$(inform_header) $@
	$(grep_markers) $@

# Templates
.PHONY: tep $(TEMPLATES)
tep: $(TEMPLATES)
$(TEMPLATES):
	$(inform_template) $@
	$(grep_markers) $@

# Code 
.PHONY: code
code: src inc tep

endif



#
# Other recipes 
#

.PHONY: more_rules
$(eval $(call more_rules))

# Compile and run the output executable
.PHONY: run
run: all
	$(inform_running)
	$(V)-cp $(OUT_FILE) $(RUN_FILE) 1>&2 2>/dev/null || :
	$(V)chmod +x $(RUN_FILE)
	$(V)-cd ./$(RUN_DIR) ; ./$(RUN_NAME)$(RUN_EXT) $(PARAMS)

# Informations (for debug) 
.PHONY: info echo
info: echo 
echo:
	@echo ""
	@echo "PLATFORM=$(PLATFORM), ARCH=$(ARCH), TARGET=$(TARGET), MXE=$(MXE)"
	@echo ""
	@echo "PATH=$(PATH)"
	@echo ""
	@echo "Compilator:"
	@echo "$(shell $(CXX) --version | head -n 1)"
	@echo "Flags:"
	@echo "$(CXXFLAGS)"
	@echo ""
	@echo "Linker:"
	@echo "$(shell $(LD) --version | head -n 1)"
	@echo "Flags:"
	@echo "$(LDFLAGS)"
	@echo ""
	@echo "Sources:"
	@echo -e "$(subst $(SPACE),\n,$(SOURCES))"
	@echo ""
	@echo "Headers:"
	@echo -e "$(subst $(SPACE),\n,$(HEADERS))"
	@echo ""
	@echo "Templates:"
	@echo -e "$(subst $(SPACE),\n,$(TEMPLATES))"
	@echo ""


