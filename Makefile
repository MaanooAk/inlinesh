
INSTALL_PATH = /usr/local

default: test

inlinesh: inlinesh.c
	cc --std=gnu11 -O3 inlinesh.c -o inlinesh


TEST_FILES = $(wildcard tests/*)

test: tests/expected tests/.testout
	diff --color=always tests/expected tests/.testout

tests/.testout: inlinesh $(TEST_FILES)
	cd tests; cat test* | inlinesh > .testout


clean:
	rm -f inlinesh /tests/.testout


install:
	mkdir -p $(INSTALL_PATH)/bin
	cp -f inlinesh $(INSTALL_PATH)/bin
	chmod 755 $(INSTALL_PATH)/bin/inlinesh

uninstall:
	rm -f $(INSTALL_PATH)/bin/inlinesh

help:
	@echo "usage: sudo make [inlinesh|clean|install|uninstall]"
