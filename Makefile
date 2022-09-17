PYLIB := $(shell pkg-config --cflags --libs python3)
NPLIB := $(shell python -c'import numpy;print(numpy.get_include())')

DEBUG ?= NO
ifeq ($(DEBUG), YES)
DEBUGFLAGS := -g
else
DEBUGFLAGS := ""
endif

.PHONY: all
all:
	clang fastmul.c $(PYLIB) -I$(NPLIB) $(DEBUGFLAGS) -o fastmul.so -fPIC -shared

