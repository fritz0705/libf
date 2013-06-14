ifeq ($(CC),cc)
ifneq ($(shell which clang),)
HAS_CLANG=yes
CC := clang
else
CC := gcc
endif
endif

ifeq ($(LD),ld)
LD := $(CC)
endif

AR ?= ar

VERSION :=\"1.0-dev\"

override CFLAGS := -fPIC -Wall -Wextra -pedantic -std=c99\
	-O$(if $(OPTIMIZATION),$(OPTIMIZATION),3) $(CFLAGS)
override CPPFLAGS := -I ./include -DVERSION=$(VERSION) $(CPPFLAGS)
override LDFLAGS := $(LDFLAGS)

ifeq ($(DEBUG),1)
override CFLAGS += -O0 -g
endif

ifeq ($(PROFILE),1)
override CC := gcc
override LD := gcc
override LDFLAGS += -pg
override CFLAGS += -pg
endif

DESTDIR :=/
PREFIX :=/usr/local

.PHONY: all clean install tags static shared

all: static shared

SRCS = $(sort $(wildcard src/*/*.c))
HEADERS = $(sort $(wildcard include/*/*.h src/*.h))
OBJS = $(SRCS:.c=.o)

static: libf.a

shared: libf.so

test: test.o libf.a
	$(LD) $(LDFLAGS) -o $@ $^

libf.so: $(OBJS)
	$(LD) $(LDFLAGS) -shared -o $@ $^

libf.a: $(OBJS)
	$(AR) rs $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) libf.so
	$(RM) libf.a
	$(RM) test
	$(RM) $(OBJS)

install: libf.so libf.a
	install libf.a libf.so $(DESTDIR)$(PREFIX)/lib/
	cp -r include/./ $(DESTDIR)$(PREFIX)/include/

