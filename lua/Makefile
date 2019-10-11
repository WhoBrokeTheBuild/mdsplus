CFLAGS  ?= -g -Wall -fPIC -I/usr/local/mdsplus/include
LDFLAGS ?=
LDLIBS  ?=

LUA ?= lua

CFLAGS  += $(shell pkg-config --cflags $(LUA))
LDFLAGS += $(shell pkg-config --libs-only-L $(LUA))
LDLIBS  += $(shell pkg-config --libs-only-l $(LUA))

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
TARGET = MDSplus.so

all: $(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET): $(OBJECTS)
	$(CC) -shared -o $@ $^ $(LDLIBS)

test:
	$(LUA) test.lua