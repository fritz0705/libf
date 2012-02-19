CC := gcc
LD := $(CC)
AR := ar

CFLAGS := -fPIC -Wall -Wextra -std=gnu99 $(CFLAGS)
CPPFLAGS := -I ./include $(CPPFLAGS)
LDFLAGS := $(LDFLAGS)

.PHONY: all clean

all: libf.so libf.a

objects := src/list.o src/hash.o

libf.so: $(objects)
	$(LD) -shared -o $@ $^

libf.a: $(objects)
	$(AR) rs $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

clean:
	rm -f libf.so libf.a
	find src/ -name '*.o' -delete
