prefix?=/usr/local

texter_cflags:=$(CFLAGS)
texter_ldflags:=$(LDFLAGS)
texter_dynamic_libs:=-ltermbox
texter_static_libs:=libs/termbox/src/libtermbox.a
texter_ldlibs:=-lm $(LDLIBS)
texter_objects:=$(patsubst %.c,%.o,$(wildcard *.c))
texter_libs_deps:=
texter_static_var:=
texter_ldlibs:=$(texter_static_libs) $(texter_ldlibs)
texter_cflags:=-Ilibs/termbox/src -Ilibs $(texter_cflags)
texter_libs_deps:=$(texter_static_libs)

all: texter

texter: $(texter_libs_deps) $(texter_objects)
	$(CC) $(texter_cflags) $(texter_objects) $(texter_ldflags) $(texter_ldlibs) -o bin/texter

$(texter_objects): %.o: %.c
	$(CC) -c $(texter_cflags) $< -o $@

$(texter_libs_deps):
	$(MAKE) -C libs

sloc:
	find . -name '*.c' -or -name '*.h' | \
    grep -Pv '(termbox|test|ut|lua)' | \
    xargs -rn1 cat | \
    wc -l

install: texter
	install -D -v -m 755 bin/texter $(DESTDIR)$(prefix)/bin/texter

uninstall:
	rm -r $(DESTDIR)$(prefix)/bin/texter

clean:
	rm -f texter $(texter_objects) $(texter_libs_deps) 
	$(MAKE) -C libs clean

.PHONY: all sloc install uninstall clean
