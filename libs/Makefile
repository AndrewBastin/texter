termbox_cflags:=-std=c99 -Wall -Wextra -pedantic -Wno-unused-result -g -O3 -D_XOPEN_SOURCE $(CFLAGS)
termbox_objects:=$(patsubst termbox/src/%.c,termbox/src/%.o,$(wildcard termbox/src/*.c))

all: load_submodules termbox/src/libtermbox.a

load_submodules:
	git submodule update --init

termbox/src/libtermbox.a: $(termbox_objects)
	$(AR) rcs $@ $(termbox_objects)

clean:
	rm -f termbox/src/libtermbox.a $(termbox_objects)

.PHONY: all clean
