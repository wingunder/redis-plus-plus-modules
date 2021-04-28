SUBDIRS = test

#all-local: lib/libhiredis.a lib/libredis++.a

hiredis redis-plus-plus:
	git submodule init
	git submodule update

build:
	mkdir -p $@

build/Makefile: build lib/libhiredis.so
	cmake -DCMAKE_SYSTEM_PREFIX_PATH=. -DCMAKE_INSTALL_PREFIX=. -DHIREDIS_HEADER=include -DHIREDIS_LIB=lib/libhiredis.so -S redis-plus-plus -B $<

lib/libredis++.a lib/libredis++.so: lib/libhiredis.a build/Makefile
	$(MAKE) -C build install

lib/libhiredis.a lib/libhiredis.so:
	PREFIX=.. $(MAKE) -C hiredis install

clean-local:
	rm -rf build include lib share

test:
	LD_LIBRARY_PATH=./lib ./test/test_redisbloom

.PRECIOUS: build

.PHONY: build test
