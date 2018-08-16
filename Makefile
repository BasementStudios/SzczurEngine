
#
# Intro
#

$(info [ PsychoX' Makefile - version 2.6.0 ])

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
    SYSTEM_PLATFORM := win
    PLATFORM := win
else
    SYSTEM_PLATFORM := lin
    PLATFORM := lin
endif

# Architecture detection
ARCH_DETECT := $(shell uname -m)
ifeq ($(findstring 64,$(ARCH_DETECT)),)
    SYSTEM_ARCH := 32
    ARCH := 32
else
    SYSTEM_ARCH := 64
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
CXXFLAGS := -Wall
LD       := g++
LDFLAGS  := -Wall

# Language
CXXFLAGS += -std=c++17 

# Using MXE? and its options
MXE := no
MXE_DIR := /usr/lib/mxe

# Packages
PKG_CONFIG := pkg-config

# Arch tags
ARCH_32 := i686
ARCH_64 := x86_64

# Use static linking
LINKING := static
CXXFLAGS_STATIC  := -static
CXXFLAGS_DYNAMIC :=
 LDFLAGS_STATIC  := -static
 LDFLAGS_DYNAMIC :=

# Threads model
THREADS := posix

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
LIB_LIST := BOOST IMGUI GLAD SFML LUA SOL2 JSON GLM OPENAL FFMPEG_AV
ifeq ($(TARGET_PLATFORM),win)
    LIB_LIST += WINDOWS
endif
#   Boost
 PKG_CONFIG_NAME_BOOST  := boost
 CXXFLAGS_STATIC_BOOST  :=
  LDFLAGS_STATIC_BOOST  :=
CXXFLAGS_DYNAMIC_BOOST  :=
 LDFLAGS_DYNAMIC_BOOST  :=
#   ImGUI 
 PKG_CONFIG_NAME_IMGUI  := imgui
 CXXFLAGS_STATIC_IMGUI  := 
  LDFLAGS_STATIC_IMGUI  := -limgui 
CXXFLAGS_DYNAMIC_IMGUI  := 
 LDFLAGS_DYNAMIC_IMGUI  := -limgui
#   GLAD
 PKG_CONFIG_NAME_GLAD   := glad
 CXXFLAGS_STATIC_GLAD   :=
  LDFLAGS_STATIC_GLAD   := -lglad
CXXFLAGS_DYNAMIC_GLAD   :=
 LDFLAGS_DYNAMIC_GLAD   := -lglad
#   SFML
 PKG_CONFIG_NAME_SFML   := sfml
 CXXFLAGS_STATIC_SFML   := -DSFML_STATIC
  LDFLAGS_STATIC_SFML   := -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -ljpeg -lopengl32 -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32 -lwinmm -DSFML_STATIC
CXXFLAGS_DYNAMIC_SFML   :=
 LDFLAGS_DYNAMIC_SFML   := -lsfml-audio   -lsfml-graphics   -lsfml-window   -lsfml-system   -lopengl32 -lfreetype -ljpeg -lopengl32 -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32 -lwinmm
#   Lua
 PKG_CONFIG_NAME_LUA    := lua
 CXXFLAGS_STATIC_LUA    :=
  LDFLAGS_STATIC_LUA    := -llua
CXXFLAGS_DYNAMIC_LUA    :=
 LDFLAGS_DYNAMIC_LUA    := -llua
#   Sol2
 PKG_CONFIG_NAME_SOL2   := sol2
#   JSON
 PKG_CONFIG_NAME_JSON   := json
#   GLM
 PKG_CONFIG_NAME_GLM   	:= glm
#   OpenAL
 PKG_CONFIG_NAME_OPENAL	:= openal
#   FFMPEG/libavformat
 PKG_CONFIG_NAME_FFMPEG_AV := libavformat
  LDFLAGS_STATIC_FFMPEG_AV := -lavformat -lavcodec -lavutil -lswresample
  LDFLAGS_SHARED_FFMPEG_AV := -lavformat -lavcodec -lavutil -lswresample
  LDFLAGS += -lswscale
#   Windows
 LDFLAGS_STATIC_WINDOWS := -lcomdlg32
 LDFLAGS_SHARED_WINDOWS := -lcomdlg32

LIB_SUPPRESS_WARNING := yes

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

# Threads mode
override THREADS := $(shell echo $(THREADS) | tr a-z A-Z)

# Cross-compilling
ifeq ($(CROSS),)
    # Detect cross-compilation
    ifneq ($(TARGET_PLATFORM),$(SYSTEM_PLATFORM))
        CROSS := yes
    endif
    ifneq ($(TARGET_ARCH),$(SYSTEM_ARCH))
        CROSS := yes
    endif
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
    # -> Linux...
    else ifeq ($(TARGET_PLATFORM),lin)
        # Select Linux GNU
        CROSS := $(CROSS)-linux-gnu
    # -> Invaild platform
    else
        $(error "Target platform not selected propertly... TARGET_PLATFORM=$(TARGET_PLATFORM)")
    endif
    
    # MXE support
    ifeq ($(TARGET_PLATFORM),win)
        ifeq ($(MXE),yes)
            # Add MXE as cross-target compiling env
            MXE_BIN_DIR := $(MXE_DIR)/usr/bin
            PATH := $(PATH):$(MXE_BIN_DIR)
            # Make shell use the specified in the makefile path
            SHELL = env PATH='$(PATH)' /bin/bash
            # Select static/shared compilator
            ifeq ($(LINKING),STATIC)
                CROSS := $(CROSS).static
            else
                CROSS := $(CROSS).shared
            endif
            # Select threads mode
            ifeq ($(THREADS),POSIX)
                CROSS := $(CROSS).posix
            endif
        endif
    endif
    
    # Compiler 
    # -> Clang
    ifeq ($(findstring clang,$(CXX)),clang)
        # Passing target information via argument instead of whole toolchain prefix
        CXXFLAGS += -target $(CROSS)
         LDFLAGS += -target $(CROSS)
        CROSS :=
    else
    # -> Other 
        CROSS := $(CROSS)-
    endif
    
    # If true 32 bit compiler not present, use universal
    ifeq ($(shell bash -c "command -v $(CROSS)$(CXX)"),)
        CROSS := $(subst i686,x86_64,$(CROSS))
        $(warning "Using universal compiler... CROSS=$(CROSS)")
    endif
    
    # If no selective compiler, just try use 'g++'...
    ifeq ($(shell bash -c "command -v $(CROSS)$(CXX)"),)
        CROSS :=
        $(warning "Using standard system compiler... ($(CXX))")
    endif
else
    CROSS :=
endif

# Disabling pkg-config to force use stupid basic system
ifeq ($(PKG_CONFIG),no)
    PKG_CONFIG :=
endif
ifeq ($(shell bash -c "command -v $(CROSS)$(PKG_CONFIG)"),)
    PKG_CONFIG :=
    #$(warning "A pkg-config not found, using only basic method to provide libraries...")
endif

# Select compiler, linker and other tools
CXX := $(CROSS)$(CXX)
LD  := $(CROSS)$(LD)
PKG_CONFIG := $(CROSS)$(PKG_CONFIG)



#
# Library flags selection
#

ifeq ($(LIB_SUPPRESS_WARNING),yes)
    LIB_INCLUDE_PREFIX := -isystem${ }
else
    LIB_INCLUDE_PREFIX := -I
endif
define add_lib
    $(eval TMP_CXXFLAGS :=)
    $(eval TMP_LDFLAGS  :=)
    
    # Detect pkg-config package
    $(if $(PKG_CONFIG),
        $(eval TMP_CXXFLAGS := $(shell $(PKG_CONFIG) --cflags $(PKG_CONFIG_NAME_$(1)) 2>/dev/null))
        $(eval TMP_LDFLAGS  := $(shell $(PKG_CONFIG) --libs   $(PKG_CONFIG_NAME_$(1)) 2>/dev/null))
    )
    
    $(if $(TMP_CXXFLAGS)$(TMP_LDFLAGS),
        # Use package flags
        $(eval CXXFLAGS += $(TMP_CXXFLAGS))
        $(eval LDFLAGS  += $(TMP_LDFLAGS))
    ,
        # Use primitive method...
        $(eval CXXFLAGS += $(CXXFLAGS_$(LINKING)_$(1)))
        $(eval LDFLAGS  +=  $(LDFLAGS_$(LINKING)_$(1)))
        $(if $(INC_DIR_$(1)_$(TARGET_ARCH)),
            $(eval CXXFLAGS += $(LIB_INCLUDE_PREFIX)$(INC_DIR_$(1)_$(TARGET_ARCH)))
        )
        $(if $(LIB_DIR_$(1)_$(TARGET_ARCH)),
            $(eval LDFLAGS  += -L$(LIB_DIR_$(1)_$(TARGET_ARCH)))
        )
    )
endef
Q := $(foreach LIB_NAME, $(LIB_LIST), $(call add_lib,$(LIB_NAME)))



#
# Other 
#

# Replacements for illegal path characters
COLON_REPLACEMENT := _c0loN
PATHBACK_REPLACEMENT := _B4ckP4th

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
OBJECTS := $(subst ..,$(PATHBACK_REPLACEMENT),$(subst :,$(COLON_REPLACEMENT),$(patsubst %,$(OBJ_DIR)/%$(OBJ_EXT),$(SOURCES))))

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
$(eval SOURCE := $(subst $(COLON_REPLACEMENT),:,$(subst $(PATHBACK_REPLACEMENT),..,$(patsubst $(OBJ_DIR)/%$(OBJ_EXT),%,$(OBJECT)))))
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
	@echo "SYSTEM_PLATFORM=$(SYSTEM_PLATFORM) SYSTEM_ARCH=$(SYSTEM_ARCH)"
	@echo "TARGET_PLATFORM=$(TARGET_PLATFORM) TARGET_ARCH=$(TARGET_ARCH)"
	@echo "MXE=$(MXE) MXE_DIR=$(MXE_DIR)"
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
	@echo "$(subst $(SPACE),\n,$(SOURCES))"
	@echo ""
	@echo "Headers:"
	@echo "$(subst $(SPACE),\n,$(HEADERS))"
	@echo ""
	@echo "Templates:"
	@echo "$(subst $(SPACE),\n,$(TEMPLATES))"
	@echo ""


