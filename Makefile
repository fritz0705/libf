CC := $(CROSS)gcc
LD := $(CC)
AR := $(CROSS)ar
CTAGS := $(CROSS)ctags

VERSION :=\"0.3-dev\"

override CFLAGS := -fPIC -Wall -Wextra -Wno-unused-parameter -std=gnu99 $(CFLAGS)
override CPPFLAGS := -I ./include -DVERSION=$(VERSION) $(CPPFLAGS)
override LDFLAGS := $(LDFLAGS)

DESTDIR :=/
PREFIX :=/usr/local
VERSION :=0.1

.PHONY: all clean install tags static shared

all: static shared

objects :=

objects += src/libf.o
objects += src/list.o src/list_iterate.o
objects += src/hash.o
objects += src/fnv.o
objects += src/fnv_str.o src/str.o src/str_fmt.o src/str_io.o src/str_cmp.o src/str_build.o
objects += src/sock.o

tags:
	$(CTAGS) -R

static: libf.a

shared: libf.so

libf.so: $(objects)
	$(LD) $(LDFLAGS) -shared -o $@ $^

libf.a: $(objects)
	$(AR) rs $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

clean:
	rm -f libf.so libf.a
	find src/ -name '*.o' -delete

install: libf.so libf.a
	install libf.a libf.so $(DESTDIR)$(PREFIX)/lib/
	cp -r include/./ $(DESTDIR)$(PREFIX)/include/
