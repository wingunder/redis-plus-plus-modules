# redis-plus-plus-modules

- [Introduction](#introduction)
- [Motivation](#motivation)
- [Design](#design)
- [Building](#building)
- [Quickstart](#quickstart)
- [APIs](#apis)
- [TODO](#todo)
- [Conclusion](#conclusion)
- [Contributing](#contributing)
- [Authors](#authors)
- [Copyright](#copyright)
- [History](#history)
- [Links](#links)

## Introduction

This library aims to supply a C++ interface to all major/popular Redis modules.
It uses [redis-plus-plus](https://github.com/sewenew/redis-plus-plus), which
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

First of all you'll need to have access to a Redis database with the
module(s) loaded, that you'd like to program against. The fastest way
to get started with this is to simply head off to
[redismod](https://github.com/RedisLabsModules/redismod) and follow
their instructions to get a docker image installed and running, which
will supply this.

Another option is to use a docker image. The [RedisBllom Quickstart
Guide](https://oss.redislabs.com/redisbloom/Quick_Start/)
suggests the following:

```console
docker run -p 6379:6379 --name redis-redisbloom redislabs/rebloom:latest
```

Once your Redis database is set up and running, you can run the
`MODULE LIST` command on it:
```console
$ redis-cli MODULE LIST
1) 1) "name"
   2) "bf"
   3) "ver"
   4) (integer) 20205
```
In the above example, only the BloomFilter module is loaded.

Not much more, but look at the `./test` directory for some hints.

## APIs

### RedisBloom

  - [BloomFilter](https://oss.redislabs.com/redisbloom/Bloom_Commands/)
  - [CuckooFilter](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/)
  - [Count-Min-Sketch](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/)
  - [Top-K](https://oss.redislabs.com/redisbloom/TopK_Commands/)

## TODO

  - Add CuckooFilter, Count-Min-Sketch and Top-K
  - Add a directory with some examples
  - Add doxygen output.
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

The initial version of *redis-plus-plus-modules* was written by [wingunder](https://github.com/wingunder).
Some ideas and code originate from
[sewenew](https://github.com/sewenew) and his amazing
[redis-plus-plus](https://github.com/sewenew/redis-plus-plus) library.

## Copyright

This software is copyrighted under [Apache Licence V2.0](https://www.apache.org/licenses/LICENSE-2.0).


## History

This project was started during the RedisConf 2021 hackathlon.

## Links

   - [Redis](https://redis.io/)
   - [redis-plus-plus](https://github.com/sewenew/redis-plus-plus)
   - [hiredis](https://github.com/redis/hiredis)
   - [RedisBloom](https://oss.redislabs.com/redisbloom/Bloom_Commands/)
