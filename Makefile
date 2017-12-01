
#
# Intro
#

$(info [ PsychoX' Makefile - version 1.8.1 ])

# Phony declarations
.PHONY: all obj src inc tep run clean info update-compilable

# Special variables
NULL := 
SPACE := $(NULL) $(NULL)

# More settings file
SETTINGS_FILE := ./settings.mk



#
# Target selection/detection
#

# Target normalize function
define normalize_TARGET
    # There MUST be so many 'eval's, because Makefile interpreter will try "optimalize" uses of variables...
    $(eval override TARGET := $(shell echo $(TARGET) | tr A-Z a-z))
    $(eval override PLATFORM := $(findstring win,$(TARGET))$(findstring lin,$(TARGET)))
    $(eval override ARCH     := $(findstring 32,$(TARGET))$(findstring 64,$(TARGET)))
    $(eval override TARGET:=$(PLATFORM)$(ARCH))
endef

# Target selection
ifdef TARGET
    $(eval $(call normalize_TARGET))
endif

# Target detection
ifeq ($(PLATFORM),)
    # Platform detection
    PLATFORM_DETECT := $(shell uname -o | tr A-Z a-z)
    ifeq ($(findstring linux,$(PLATFORM_DETECT)),)
        PLATFORM := win
    else
        PLATFORM := lin
    endif
endif
ifeq ($(ARCH),)
    # Architecture detection
    ARCH_DETECT := $(shell uname -m)
    ifeq ($(findstring 64,$(ARCH_DETECT)),)
        ARCH := 32
    else
        ARCH := 64
    endif
endif
override TARGET:=$(PLATFORM)$(ARCH)



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

# Use static linking
LINKING := static
CXXFLAGS_STATIC  := -static
CXXFLAGS_DYNAMIC :=
 LDFLAGS_STATIC  := -static
 LDFLAGS_DYNAMIC :=

# Clean entries
CLEAN_FILES := "./obj/*/*.o" "./$(OUT_DIR)/$(OUT_NAME)" "./$(RUN_DIR)/$(RUN_NAME)"
CLEAN_DIRS  := "./obj/*/*"
CLEAN_DIRS_FORCE := no

# Using MXE? and its options
MXE := no
MXE_DIR := /usr/lib/mxe

# Libraries 
LIB_LIST := SFML BOOST LUA
#   SFML
 CXXFLAGS_STATIC_SFML   := -DSFML_STATIC
  LDFLAGS_STATIC_SFML   := -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -ljpeg -lopengl32 -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32 -lwinmm -DSFML_STATIC
CXXFLAGS_DYNAMIC_SFML   :=
 LDFLAGS_DYNAMIC_SFML   := -lsfml-graphics -lsfml-window -lsfml-system
 MXE_PACKAGENAME_SFML   := sfml
#   Boost
 CXXFLAGS_STATIC_BOOST  :=
  LDFLAGS_STATIC_BOOST  :=
CXXFLAGS_DYNAMIC_BOOST  :=
 LDFLAGS_DYNAMIC_BOOST  :=
 MXE_PACKAGENAME_BOOST  := boost
#  Lua
 CXXFLAGS_STATIC_LUA    := -lliblua
  LDFLAGS_STATIC_LUA    :=
CXXFLAGS_DYNAMIC_LUA    := -lliblua
 LDFLAGS_DYNAMIC_LUA    := 
 MXE_PACKAGENAME_LUA    := lua

# Executables (.so/.dll) @todo kiedyś może to zrobię :D zeby COPY_BIN kopiowalo dll/so ;)
EXECUTABLES_SFML := openal32.dll sfml*

# Header-only libs
HEADER_LIB_LIST := SOL2 JSON
HEADER_INC_SOL2 := 3rd-party/sol2
HEADER_INC_JSON := 3rd-party/json



#
# Load settings override
#

include $(SETTINGS_FILE)



#
# Compiler selection
#

# Linking
override LINKING := $(shell echo $(LINKING) | tr a-z A-Z)
CXXFLAGS += $(CXXFLAGS_$(LINKING))
LDFLAGS  += $( LDFLAGS_$(LINKING))

# Architecture 
# -> 32 bit
ifeq ($(ARCH),32)
    CROSS := i686
# -> 64 bit 
else ifeq ($(ARCH),64)
    CROSS := x86_64
# -> Invaild architecture
else
    $(error "Target architecture not selected propertly... ARCH=$(ARCH)")
endif
CXXFLAGS += -m$(ARCH)
LDFLAGS  += -m$(ARCH)

# Platform
# -> Windows...
ifeq ($(PLATFORM),win)
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
    else
        # Other MinGW on Windows?
        #...
    endif
# -> Linux...
else ifeq ($(PLATFORM),lin)
    # Select Linux GNU
    CROSS := $(CROSS)-linux-gnu
# -> Invaild platform
else
    $(error "Target platform not selected propertly... PLATFORM=$(PLATFORM)")
endif
CROSS := $(CROSS)-

# If true 32 bit compiler not present, use universal
ifeq ($(shell bash -c "command -v $(CROSS)g++"),)
    CROSS := $(subst i686,x86_64,$(CROSS))
endif

# If no selective compiler, just try use 'g++'...
ifeq ($(shell bash -c "command -v $(CROSS)g++"),)
    CROSS :=
endif



#
# Library flags selection
#

# Compiled libs
define add_lib
    ifeq ($(and $(findstring $(PLATFORM),win),$(findstring $(MXE),yes),$(MXE_PACKAGENAME_$(1)),1),1)
        # Use MXE packages
        MXE_PKGS += $(MXE_PACKAGENAME_$(1))
    else
        # Add linking dependent flags
        CXXFLAGS += $(CXXFLAGS_$(LINKING)_$(1))
        LDFLAGS  +=  $(LDFLAGS_$(LINKING)_$(1))
        # Add includes/libraries object directories
        ifneq ($(INC_DIR_$(1)_$(ARCH)),)
            CXXFLAGS += -I$(INC_DIR_$(1)_$(ARCH))
        endif
        ifneq ($(LIB_DIR_$(1)_$(ARCH)),)
            LDFLAGS  += -L$(LIB_DIR_$(1)_$(ARCH))
        endif
    endif
endef
$(foreach LIB_NAME, $(LIB_LIST), $(eval $(call add_lib,$(LIB_NAME))))

# MXE uses pkg-config
ifeq ($(and $(findstring $(PLATFORM),win),$(findstring $(MXE),yes),1),1)
    CXXFLAGS += $(shell $(CROSS)pkg-config --cflags $(MXE_PKGS))
    LDFLAGS  += $(shell $(CROSS)pkg-config --libs $(MXE_PKGS))
endif

# Header-only libs
$(foreach LIB_NAME, $(HEADER_LIB_LIST), $(eval CXXFLAGS += -I$(HEADER_INC_$(LIB_NAME))))



#
# Other 
#

# @warn To dla MrRaiNa ;)
ifneq ($(MRRAIN),)
    CROSS :=
endif

# Set tools
CXX := $(CROSS)$(CXX)
LD  := $(CROSS)$(LD)
LS  := ls -AdoGh --time-style long-iso
RM  := rm $(if $(CLEAN_FORCE),-rf,)
PRINT := printf
FUCKING_SLIENT :=1>&2 2>/dev/null || :

ifeq ($(SLIENT),yes)
    PRINT := :
endif

# Other FORCE
ifeq ($(FORCE),yes)
    CLEAN_FORCE := yes
endif

# Adding headers and templates directories to search paths
CXXFLAGS += -I$(subst $(SPACE), -I,$(INC_DIRS))
CXXFLAGS += -I$(subst $(SPACE), -I,$(TEP_DIRS))



#
# Files searching
#

# Sources to compile
SOURCES := $(shell find $(SRC_DIRS) -type f -name '*$(subst $(SPACE),' -or -name '*,$(SRC_EXTS))')

# Headers to check updates
HEADERS := $(shell find $(INC_DIRS) -type f -name '*$(subst $(SPACE),' -or -name '*,$(INC_EXTS))')

# Template files to check updates
TEPLATS := $(shell find $(TEP_DIRS) -type f -name '*$(subst $(SPACE),' -or -name '*,$(TEP_EXTS))')

# Objects to be compiled @warn problem jestli SRC_DIR ma '..'...?
OBJECTS := $(patsubst %,$(OBJ_DIR)/%$(OBJ_EXT),$(SOURCES))

# Output file to be linked to
OUT_FILE := $(OUT_DIR)/$(OUT_NAME)$(OUT_EXT)

# Runtime file to run for tests
RUN_FILE := $(RUN_DIR)/$(RUN_NAME)$(RUN_EXT)

# Files to clean out
CLEAN_FILES := $(shell find . -type f -path $(subst $(SPACE), -or -path ,$(CLEAN_FILES)))

# Folders to clean out
CLEAN_DIRS  := $(shell find . -type d -path $(subst $(SPACE), -or -path ,$(CLEAN_DIRS)))



#
# Recipes
#

# Standard options
defualt: all
nothing: 
all: lnk

# Linking 
lnk: $(OUT_FILE)
$(OUT_FILE): $(OBJECTS)
	@mkdir -p `dirname $@`
	@$(PRINT) "[Linking] $$ "
	$(LD) $(OBJECTS) -o $@ $(LDFLAGS)
	@$(PRINT) "[Linked]\033[1m -> " 
	-@$(LS) $@
	@$(PRINT) "\033[0m"

# Compiling
obj: $(OBJECTS)
$(OBJECTS): $(OBJ_DIR)/%$(OBJ_EXT): %
	@mkdir -p `dirname $@`
	@$(PRINT) "[Compiling] $$ "
	$(CXX) -c $< -o $@ $(CXXFLAGS)
	@$(PRINT) "[Compiled]\033[1m -> "
	-@$(LS) $@
	@$(PRINT) "\033[0m"

# Preparsing
src: $(SOURCES)
#$(SOURCES):
#	@$(PRINT) "[Preparsing] $$ "
inc: $(HEADERS)
#$(HEADERS):
#	@$(PRINT) "[Preparsing] $$ "
tep: $(TEPLATS)
#$(TEPLATS):
#	@$(PRINT) "[Preparsing] $$ "

# Update sources by changes in related headers and templates
update-compilable: 
ifndef COMPILABLE_UPDATED
	@$(PRINT) "[Marking compilable to update] $ "
	# @todo WIP. PsychoX
	#-@chmod +x ./Update-By-Deps.sh
	#-./Update-By-Deps.sh --help work in progress ;)
	$(eval COMPILABLE_UPDATED := yes)
endif

# Compile and run the output executable
run: all
	@$(PRINT) "[Running]\n"
	@-cp $(OUT_FILE) $(RUN_FILE) $(FUCKING_SLIENT)
	@chmod +x $(RUN_FILE)
	cd ./$(RUN_DIR) ; ./$(RUN_NAME)$(RUN_EXT) $(PARAMS)

# Cleaning the compile environment
clean: 
	@$(PRINT) "[Cleaning]\n"
ifeq ($(or $(CLEAN_FILES),$(CLEAN_DIRS),$(CLEAN_FORCE)),) 
	@$(PRINT) "Nothing to clean.\n"
else
	-$(RM) $(CLEAN_FILES) || :
	-$(RM) -dr $(CLEAN_DIRS) 2> /dev/null || :
endif

# Informations (for debug) 
info:
	$(info  )
	$(info TARGET=$(TARGET) -> PLATFORM=$(PLATFORM), ARCH=$(ARCH), MXE=$(MXE))
	$(info  )
	$(info PATH=$(PATH))
	$(info  )
	$(info Compilator: )
	$(info $(shell $(CXX) --version | head -n 1))
	$(info Flags: )
	$(info $(CXXFLAGS))
	$(info  )
	$(info Linker: )
	$(info $(shell $(LD) --version | head -n 1))
	$(info Flags: )
	$(info $(LDFLAGS))
	$(info  )
	$(info SOURCES = $(SOURCES))
	$(info  )
	$(info HEADERS = $(HEADERS))
	$(info  )
	$(info TEPLATS = $(TEPLATS))
	$(info  )
	$(info OBJECTS = $(OBJECTS))
	$(info  )


