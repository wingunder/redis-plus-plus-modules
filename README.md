# redis-plus-plus-modules

- [Introduction](#introduction)
- [Motivation](#motivation)
- [Design](#design)
- [Building](#building)
- [Quickstart](#quickstart)
- [TODO](#todo)
- [Conclusion](#conclusion)
- [Contributing](#contributing)
- [Authors](#authors)
- [History](#history)
- [Links](#links)

## Introduction

This library aims to supply a C++ interface to all major/popular Redis modules.
It derives from [redis-plus-plus](https://github.com/sewenew/redis-plus-plus), which
in turn uses [hiredis](https://github.com/redis/hiredis).

## Motivation

After using [redis-plus-plus](https://github.com/sewenew/redis-plus-plus)
for a while, one simply gets addicted to the ease of having proper
interfaces to Redis commands. These interfaces save a lot of
boiler-plate code and therefore a lot of time, and that's why I
decided to write this library.

## Design

The goal of this library is to use
[redis-plus-plus](https://github.com/sewenew/redis-plus-plus) and
[hiredis](https://github.com/redis/hiredis) with no modification.
These two projects are included as git submodules and are built as
part of the final redis-plus-plus-modules library.

## Building

GNU/Automake is used as building tool and g++ is used for building.

```shell
git clone https://github.com/wingunder/redis-plus-plus-modules.git
cd
./bootstrap.sh
./configure
make -j8
```

Tests for RedisBloom can be done by running:
```shell
LD_LIBRARY_PATH=./lib ./test/test_redisbloom
```

## Quickstart

Nothing yet, but look at the `./test` directory for some hints.

## TODO

  - Finish the BloomFilter (SCANDUMP and LOADCHUNK are still missing)
  - Add `Quickstart` content
  - Add API docs.
  - Add Cuckoo, Count-Min-Sketch and Top-K
  - Install procedure and at least a Debian package
  - Add APIs for: RedisJSON, RediSearch, RedisTimeSeries, RedisGraph,
    RedisAI and RedisGears
  - Add the hackathlon video

## Conclusion

Not yet, but it will be something like:
   - Making this library was pretty easy and it should be trivial to
   extend it with future Redis modules.
   - It's not done yet. (Registered TM) :)

## Contributing

In principle, contributions are welcome, however this project is in a
seriously heavy development phase and everything can still change. So,
at the moment I'll not be accepting any pull requests. But feel free
to open issues and reviews.

## Authors

*redis-plus-plus-modules* was initially written by [wingunder](https://github.com/wingunder).
Some ideas and code originate from
[sewenew](https://github.com/sewenew) and his amazing
[redis-plus-plus](https://github.com/sewenew/redis-plus-plus) library.

## History

This project was started during the RedisConf 2021 hackathlon.

## Links

   - [Redis](https://redis.io/)
   - [redis-plus-plus](https://github.com/sewenew/redis-plus-plus)
   - [hiredis](https://github.com/redis/hiredis)
   - [RedisBloom](https://oss.redislabs.com/redisbloom/Bloom_Commands/)
