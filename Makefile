CC := $(CROSS)gcc
LD := $(CC)
AR := $(CROSS)ar
CTAGS := $(CROSS)ctags

ENV:=hosted

VERSION :=\"0.4-dev\"

override CFLAGS := -fPIC -Wall -Wextra -Wno-unused-parameter -std=gnu99 -g -O2 $(CFLAGS)
override CPPFLAGS := -I ./include -DVERSION=$(VERSION) $(CPPFLAGS)
override LDFLAGS := $(LDFLAGS)

ifeq ($(ENV),freestanding)
override CFLAGS += -ffreestanding -nostdinc
override LDFLAGS += -nostdlib
endif

DESTDIR :=/
PREFIX :=/usr/local

.PHONY: all clean install tags static shared

all: static
ifeq ($(ENV),hosted)
all: shared
endif

objects :=

objects += src/list.o
objects += src/dict.o
ifeq ($(ENV),hosted)
endif

tags:
	$(CTAGS) -R

static: libf.a

shared: libf.so

libf.so: $(objects)
	$(LD) $(LDFLAGS) -shared -o $@ $^

libf.a: $(objects)
	$(AR) rs $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f libf.so libf.a
	find src/ -name '*.o' -exec $(RM) {} \;

install: libf.so libf.a
	install libf.a libf.so $(DESTDIR)$(PREFIX)/lib/
	cp -r include/./ $(DESTDIR)$(PREFIX)/include/

src/list.o: include/f/list.h
src/dict.o: include/f/dict.h include/f/list.h

