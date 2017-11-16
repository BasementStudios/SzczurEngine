
#
$(info [ Makefile for SzczurEngine project ])
$(info [   version 1.7.5 by PsychoX & <3   ])
#

# Phony declarations
.PHONY: all obj src inc tep run clean info update-compilable

# Space
NULL := 
SPACE := $(NULL) $(NULL)



#
# Target selection/detection
#

# Target normalize function
define normalize_TARGET
    override TARGET := $(shell echo $(TARGET) | tr A-Z a-z)
    PLATFORM := $(findstring win,$(TARGET))$(findstring lin,$(TARGET))
    ARCH     := $(findstring 32,$(TARGET))$(findstring 64,$(TARGET))
    override TARGET:=$(PLATFORM)$(ARCH)
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
INC_DIR := src
SRC_DIR := src
TEP_DIR := src
OBJ_DIR := obj/$(TARGET)
OUT_DIR := out
RUN_DIR := out

# Extensions
SRC_EXT := .cpp
INC_EXT := .hpp
TEP_EXT := .tpp
OBJ_EXT := .o
OUT_EXT := .exe
RUN_EXT := .exe

# Common compiler and linker flags
CXXFLAGS := -std=c++17 -Wall -I$(INC_DIR)
LDFLAGS  :=

# Use static linking
LINKING := static

# Clean entries
CLEAN_FILES := "./obj/*/*.o" "./$(OUT_DIR)/$(OUT_NAME)" "./$(RUN_DIR)/$(RUN_NAME)"
CLEAN_DIRS  := "./obj/*/*"
CLEAN_DIRS_FORCE := no

# Using MXE? and its options
MXE := no
MXE_DIR := /usr/lib/mxe
MXE_PKGS := sfml

# Libraries 
LIB_LIST := SFML BOOST
# 	Directories
#		SFML
LIB_INC_DIR_SFML_32  :=
LIB_BIN_DIR_SFML_32  :=
LIB_INC_DIR_SFML_64  :=
LIB_BIN_DIR_SFML_64  :=
#		Boost
LIB_INC_DIR_BOOST_32 :=
LIB_BIN_DIR_BOOST_32 :=
LIB_INC_DIR_BOOST_64 :=
LIB_BIN_DIR_BOOST_64 :=
#	Flag sets
#		SFML
CXXFLAGS_STATIC_SFML   := -DSFML_STATIC
LDFLAGS_STATIC_SFML    := -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -ljpeg -lopengl32 -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32 -lwinmm -DSFML_STATIC
CXXFLAGS_DYNAMIC_SFML  :=
LDFLAGS_DYNAMIC_SFML   := -lsfml-graphics -lsfml-window -lsfml-system
#		Boost
CXXFLAGS_STATIC_BOOST  :=
LDFLAGS_STATIC_BOOST   :=
CXXFLAGS_DYNAMIC_BOOST :=
LDFLAGS_DYNAMIC_BOOST  :=



#
# Load settings override
#

include ./settings.mk

$(eval $(call normalize_TARGET))



#
# Compiler selection
#

# Linking
override LINKING := $(shell echo $(LINKING) | tr a-z A-Z)
ifeq ($(LINKING),STATIC)
    CXXFLAGS += -static
    LDFLAGS  += -static
endif

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



#
# Library flags selection
#

ifeq ($(and $(findstring $(PLATFORM),win),$(findstring $(MXE),yes),1),1)
	# MXE uses pkg-config
	CXXFLAGS += $(shell $(CROSS)pkg-config --cflags $(MXE_PKGS))
	LDFLAGS  += $(shell $(CROSS)pkg-config --libs $(MXE_PKGS))
else
    define add_lib
		# Add linking dependent flags
        CXXFLAGS += $(CXXFLAGS_$(LINKING)_$(1))
        LDFLAGS  +=  $(LDFLAGS_$(LINKING)_$(1))
        # Add includes/libraries object directories
		ifneq ($(LIB_INC_DIR_$(1)_$(ARCH)),)
            CXXFLAGS += -I$(LIB_INC_DIR_$(1)_$(ARCH))
        endif
        ifneq ($(LIB_BIN_DIR_$(1)_$(ARCH)),)
            LDFLAGS  += -L$(LIB_BIN_DIR_$(1)_$(ARCH))
        endif
    endef
    $(foreach LIB_NAME, $(LIB_LIST), $(eval $(call add_lib,$(LIB_NAME))))
endif



#
# Other 
#

# @warn To dla MrRaiNa ;)
ifeq ($(PLATFORM),lin)
    CROSS :=
endif

# Set tools
CXX := $(CROSS)g++
LD  := $(CROSS)g++
LS  := ls -AdoGh --time-style long-iso
RM  := rm $(if $(CLEAN_FORCE),-rf,)
PRINT := printf

ifeq ($(SLIENT),yes)
    PRINT := :
endif



#
# Files searching
#

# Sources to compile
SOURCES := $(shell find $(SRC_DIR) -type f -name '*$(SRC_EXT)')

# Headers to check updates
HEADERS := $(shell find $(INC_DIR) -type f -name '*$(INC_EXT)')

# Template files to check updates
TEPLATS := $(shell find $(TEP_DIR) -type f -name '*$(TEP_EXT)')

# Objects to be compiled
OBJECTS := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SOURCES:$(SRC_EXT)=$(OBJ_EXT)))

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
	@$(PRINT) "[Linked] -> "
	-@$(LS) $@

# Compiling
obj: $(OBJECTS)
$(OBJECTS): $(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT)
	@mkdir -p `dirname $@`
	@$(PRINT) "[Compiling] $$ "
	$(CXX) -c $< -o $@ $(CXXFLAGS)
	@$(PRINT) "[Compiled] -> "
	-@$(LS) $@

# Preparsing 
src: $(SOURCES)
#$(SOURCES): $(SRC_DIR)/%$(SRC_EXT):
#	@$(PRINT) "[Preparsing] $$ "
inc: $(HEADERS)
#$(HEADERS): $(INC_DIR)/%$(INC_EXT):
#	@$(PRINT) "[Preparsing] $$ "
tep: $(TEPLATS)
#$(TEPLATS): $(TEP_DIR)/%$(TEP_EXT):
#	@$(PRINT) "[Preparsing] $$ "

# Update sources by changes in related headers and templates
update-compilable: 
ifndef COMPILABLE_UPDATED
	@$(PRINT) "[Marking compilable to update] $ "
	# @todo ;f MrRaiN i PsychoX
	#-@chmod +x ./helper.sh
	#-./Update-By-Deps.sh --help work in progress ;)
	$(eval COMPILABLE_UPDATED := yes)
endif

# Run
run: all
	@$(PRINT) "[Running]\n"
	@-cp $(OUT_FILE) $(RUN_FILE)
	@chmod +x $(RUN_FILE)
	cd ./$(RUN_DIR) & ./$(RUN_NAME)$(RUN_EXT) $(PARAMS)

# Clean 
clean: 
	@$(PRINT) "[Cleaning]\n"
ifeq ($(CLEAN_FILES),) 
	@$(PRINT) "Nothing to clean.\n"
else
	-rm $(CLEAN_FILES)
	-rm -d $(CLEAN_DIRS)
endif

info:
	$(info  )
	$(info TARGET=$(TARGET) -> PLATFORM=$(PLATFORM), ARCH=$(ARCH), MXE=$(MXE))
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


