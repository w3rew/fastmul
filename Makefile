PYLIB := $(shell pkg-config --cflags --libs python3)
NPLIB := $(shell python -c'import numpy;print(numpy.get_include())')

DEBUG ?= NO
ifeq ($(DEBUG), YES)
DEBUGFLAGS ?= -g
else
DEBUGFLAGS ?=
endif

OPT ?= NO
ifeq ($(OPT), YES)
OPTFLAGS ?= -Ofast -march=native
else
OPTFLAGS ?=
endif

THREADING ?= NO
ifeq ($(THREADING), YES)
THREADFLAGS ?= -fopenmp
else 
THREADFLAGS ?=
endif

.PHONY: all
all:
	clang fastmul.c $(PYLIB) -I$(NPLIB) $(DEBUGFLAGS) $(OPTFLAGS) $(THREADFLAGS) -o fastmul.so -fPIC -shared

