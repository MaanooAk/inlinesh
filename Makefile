
INSTALL_PATH = /usr/local

rebuild: clean inlinesh

clean:
	rm -f inlinesh

inlinesh:
	gcc --std=gnu11 -O3 inlinesh.c -o inlinesh


install: rebuild
	mkdir -p $(INSTALL_PATH)/bin
	cp -f inlinesh $(INSTALL_PATH)/bin
	chmod 755 $(INSTALL_PATH)/bin/inlinesh

uninstall:
	rm -f $(INSTALL_PATH)/bin/inlinesh

help:
	@echo "usage: sudo make [inlinesh|clean|install|uninstall]"
