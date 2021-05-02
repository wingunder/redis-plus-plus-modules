SUBDIRS = test

all: lib/libredis++.so

hiredis:
	git submodule update --init

lib/libhiredis.so: hiredis
	PREFIX=.. $(MAKE) -C hiredis install
	touch lib/libhiredis.so

build-redis-plus-plus:
	mkdir -p $@

build-redis-plus-plus/Makefile: build-redis-plus-plus lib/libhiredis.so
	cmake -DCMAKE_SYSTEM_PREFIX_PATH=. -DCMAKE_INSTALL_PREFIX=. -DHIREDIS_HEADER=include -DHIREDIS_LIB=lib/libhiredis.so -S redis-plus-plus -B $<

lib/libredis++.so: build-redis-plus-plus/Makefile
	$(MAKE) -C build-redis-plus-plus install
	touch lib/libredis++.so

clean-local:
	rm -rf build-redis-plus-plus include/hiredis include/redis-plus-plus lib share

test/test_redismodules: lib/libredis++.so
	$(MAKE) -C test test_redismodules

test: test/test_redismodules
	$(MAKE) -C test test_redismodules
	LD_LIBRARY_PATH=./lib ./test/test_redismodules

.PHONY: test
