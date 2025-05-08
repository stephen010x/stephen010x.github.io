TARGET := bin
TEXTEN := wasm

TMPDIR := tmp
SRCDIR := src
BINDIR := bin
LIBDIR := lib
GENDIR := src/shaders/gen

# TODO: create a phony target that preprocesses every
# source and header file, and places them in a temp directory
# for you to view and debug
#emcc -E -iquote -I../lib/cglm/include input.h -o input.pp.h


CC := emcc
CFLAGS := -Wno-gnu-variable-sized-type-not-at-end
CFLAGS := $(CFLAGS)
LFLAGS := -sOFFSCREEN_FRAMEBUFFER -sOFFSCREENCANVAS_SUPPORT 
# --proxy-to-worker  -sFETCH -pthread -sPTHREAD_POOL_SIZE=4
#LFLAGS := $(LFLAGS) -sINVOKE_RUN=0 -sEXPORTED_RUNTIME_METHODS=ccall,cwrap
LFLAGS := $(LFLAGS)
BFLAGS := -Wall -Wextra -Wpedantic -Wconversion -Wundef
BFLAGS := $(BFLAGS) -Wno-language-extension-token -Wno-gnu -std=gnu2x
BFLAGS := -Wno-format-security
#-sWASM=2

INCLUDE := -I$(LIBDIR)/cglm/include


DEFAULT := fast


GOAL := $(firstword $(MAKECMDGOALS))
GOAL := $(if $(GOAL),$(GOAL),$(DEFAULT))
# should grab all paths relative to makefile.
# colon equal (:=) removed, so as to grab all generated c files as well
SRCS = $(shell find $(SRCDIR) -name "*.c")
ifneq ($(GOAL),clean)
	OBJS = $(SRCS:%.c=$(TMPDIR)/$(GOAL)/%.o)
	DEPS = $(SRCS:%.c=$(TMPDIR)/$(GOAL)/%.d)
endif
BINTARG := $(BINDIR)/$(TARGET)


#CURRENTDIR := $(shell pwd)


ifeq ($(OS),Windows_NT) 
    $(error Incompatable operating system)
else
    OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
    ifeq ($(OS),Linux)
    	LINUX := LINUX
    	BLFLAGS += -target *-*-linux-gnu
    	#MAKEOSFILE := make/linux.mk
    else
		$(error Incompatable operating system)
    endif
endif









###########################################
#                                         #
#      LINUX                              #
#                                         #
###########################################
ifdef LINUX



all: $(DEFAULT)
fast: _fast $(BINTARG).$(TEXTEN)
debug: _debug $(BINTARG).$(TEXTEN)
release: _release $(BINTARG).$(TEXTEN)


-include $(DEPS)
-include shaders


# TODO: FIX THIX!!!
shaders: $(GENDIR)/shaders.h
	@:


# -sGL_DEBUG=1
_fast:
	$(eval CFLAGS += -DDEBUG_MODE)
	$(eval LFLAGS += -sASSERTIONS=2)
	$(eval BFLAGS += -g -O0 -gsource-map)

# -sGL_DEBUG=1
_debug:
	$(eval CFLAGS += -DDEBUG_MODE)
	$(eval LFLAGS += -sASSERTIONS=2)
	$(eval BFLAGS += -g -Og -gsource-map)

_release: _optimize

# -Oz  ## smaller than -Os, but also slower
#  -sMODULARIZE
_optimize:
	$(eval LFLAGS += -Wl,--gc-sections -s -flto --closure 1)
	$(eval BFLAGS += -Os -g0)


# $(basename $@).html
$(BINTARG).$(TEXTEN): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(BFLAGS) $(LFLAGS) -o $@ -o $(basename $@).js $^

$(TMPDIR)/$(GOAL)/%.o: %.c $(TMPDIR)/$(GOAL)/%.d
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE) $(BFLAGS) $(CFLAGS) -c $< -o $@

$(TMPDIR)/$(GOAL)/%.d: %.c
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE) -MM -MT $(patsubst %.d,%.o,$@) -MF $@ $<


$(GENDIR)/shaders.h:
	python3 src/shaders/shader_to_c.py


clean:
	rm -rf $(TMPDIR)
	rm -rf $(BINDIR)
	rm -rf src/shaders/gen

# ./serve.sh & google-chrome --enable-features=SharedArrayBuffer 0.0.0.0:8000
#chrome:
#	./serve.sh & google-chrome 0.0.0.0:8000


endif





.PHONY: all fast debug release static dynamic clean test chrome shaders
.PHONY: _build _fast _debug _release _dynamic _static _optimize
