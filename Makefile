SRC_DIR ?= src
SRC = $(SRC_DIR)/

OUT_DIR ?= out
CC_VERSION := $(shell $(CC) -dumpversion)
MACHINE_INFO := $(shell $(CC) -dumpmachine)
OUT = $(OUT_DIR)/$(CC)-$(CC_VERSION)-$(MACHINE_INFO)/
DEPS = $(OUT)dependency-files/

LIB_DIR = lib
LIB_BASE_NAME = linked-list
LIB = $(OUT)$(LIB_DIR)/
LIB_PREFIX = $(LIB)lib
SO_TARGET = $(LIB_PREFIX)$(LIB_BASE_NAME).so
A_TARGET = $(LIB_PREFIX)$(LIB_BASE_NAME).a

EXE_DIR = $(OUT)exe
EXE = $(EXE_DIR)/

EXAMPLE_DIR ?= example
EXAMPLE = $(EXAMPLE_DIR)/

SOURCES := $(shell find '$(SRC)' -name '*.c')
OBJECTS = $(subst $(SRC),$(OUT),$(SOURCES:.c=.o))
DEPENDENCY_FILES = $(subst $(SRC),$(DEPS),$(SOURCES:.c=.deps))

EXAMPLE_SOURCES := $(shell find '$(EXAMPLE)' -name '*.c')
EXAMPLE_EXECUTABLES = $(subst $(EXAMPLE),$(EXE),$(EXAMPLE_SOURCES:.c=))

# {{{ Command and building flags ##############################################

CC_OUTPUT_OPTION = -o $@
CC ?= gcc
CFLAGS += -Wall -std=c11
CFLAGS += -g
CPPFLAGS += -I$(SRC_DIR)
#TARGET_ARCH

MK_OUT_DIRS = \
    [ -e '$(dir $@)' ] || \
        { mkdir -p '$(dir $@)' && \
            echo '*** INFO: Creating directory: "$(dir $@)" ***'; }

# }}} Command and building flags ##############################################

# {{{ Generic building rules ##################################################

$(DEPS)%.deps: $(SRC)%.c
	@$(MK_OUT_DIRS)
	$(CC) $(CPPFLAGS) -MM $(CC_OUTPUT_OPTION) $<

$(OUT)%.o: $(SRC)%.c
	@$(MK_OUT_DIRS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $(CC_OUTPUT_OPTION) $<

$(EXE)%: $(EXAMPLE)%.c
	@$(MK_OUT_DIRS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) $< $(LOADLIBES) $(LDLIBS) $(CC_OUTPUT_OPTION)

# }}} Generic building rules ##################################################

all: build
.PHONY: all

build: build-so build-a
.PHONY: build

build-so: CFLAGS += -fPIC
build-so: LDFLAGS += -shared
build-so: $(SO_TARGET)
.PHONY: build-so

$(SO_TARGET): $(OBJECTS)
	@$(MK_OUT_DIRS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) $^ $(LOADLIBES) $(LDLIBS) $(CC_OUTPUT_OPTION)

build-a: $(A_TARGET)
.PHONY: build-a

$(A_TARGET): $(OBJECTS)
	@$(MK_OUT_DIRS)
	$(AR) $(ARFLAGS) $@ $^

examples: build-examples
.PHONY: examples

build-examples: build build-example-executables
.PHONY: build-example-executables

# Uncomment following two lines to link with library instead of inlining:
#build-example-executables: CPPFLAGS += -L$(LIB) -DNO_INLINE
#build-example-executables: LDLIBS += -l$(LIB_BASE_NAME)
build-example-executables: $(EXAMPLE_EXECUTABLES)
.PHONY: build-examples

clean: clean-objects clean-dependency-files
.PHONY: clean
NO_DEPS_TARGETS += clean

clean-objects:
	rm -f $(OBJECTS)
.PHONY: clean-objects
NO_DEPS_TARGETS += clean-objects

clean-dependency-files:
	rm -f $(DEPENDENCY_FILES)
.PHONY: clean-dependency-files
NO_DEPS_TARGETS += clean-dependency-files

distclean dist-clean:
	rm -fr $(OUT_DIR)
.PHONY: distclean dist-clean
NO_DEPS_TARGETS += distclean dist-clean

INCLUDE_DEPENDENCIES = Y

# If $(MAKECMDGOALS) is empty, then we are calling default target which
# requires dependency files to be included.
ifneq ($(MAKECMDGOALS),)
ifeq ($(filter-out $(NO_DEPS_TARGETS),$(MAKECMDGOALS)),)
INCLUDE_DEPENDENCIES = N
endif
endif

ifeq ($(INCLUDE_DEPENDENCIES),Y)
-include $(DEPENDENCY_FILES)
endif
