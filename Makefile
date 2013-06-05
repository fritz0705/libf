CC := $(CROSS)gcc
LD := $(CC)
AR := $(CROSS)ar
CTAGS := $(CROSS)ctags

VERSION :=\"1.0-dev\"

override CFLAGS := -fPIC -Wall -Wextra -pedantic -std=gnu11 -O3 $(CFLAGS)
override CPPFLAGS := -I ./include -DVERSION=$(VERSION) $(CPPFLAGS)
override LDFLAGS := $(LDFLAGS)

ifeq ($(DEBUG),1)
override CFLAGS += -O0 -g
endif

DESTDIR :=/
PREFIX :=/usr/local

.PHONY: all clean install tags static shared

all: static shared

SRCS = $(sort $(wildcard src/*/*.c))
OBJS = $(SRCS:.c=.o)

tags:
	$(CTAGS) -R

static: libf.a

shared: libf.so

libf.so: $(OBJS)
	$(LD) $(LDFLAGS) -shared -o $@ $^

libf.a: $(OBJS)
	$(AR) rs $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) libf.so
	$(RM) libf.a
	find src/ -name '*.o' -delete

install: libf.so libf.a
	install libf.a libf.so $(DESTDIR)$(PREFIX)/lib/
	cp -r include/./ $(DESTDIR)$(PREFIX)/include/

