CC := $(CROSS)gcc
LD := $(CC)
AR := $(CROSS)ar
CTAGS := $(CROSS)ctags

CFLAGS := -fPIC -Wall -Wextra -std=gnu99 $(CFLAGS)
CPPFLAGS := -I ./include $(CPPFLAGS)
LDFLAGS := $(LDFLAGS)

DESTDIR :=/
PREFIX :=$(DESTDIR)usr/local

.PHONY: all clean install tags static shared

all: static shared

objects := src/list.o src/hash.o src/str.o

tags:
	$(CTAGS) -R

static: libf.a

shared: libf.so

libf.so: $(objects)
	$(LD) -shared -o $@ $^

libf.a: $(objects)
	$(AR) rs $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

clean:
	rm -f libf.so libf.a
	find src/ -name '*.o' -delete

install: libf.so libf.a
	install libf.a libf.so $(PREFIX)/lib/
	cp -r include/./ $(PREFIX)/include/
