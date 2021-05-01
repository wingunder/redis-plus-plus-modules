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

The following Redis Modules are fully implemented:

  - [BloomFilter](https://oss.redislabs.com/redisbloom/Bloom_Commands/)
  - [CuckooFilter](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/)
  - [Count-Min-Sketch](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/)
  - [Top-K](https://oss.redislabs.com/redisbloom/TopK_Commands/)
  - [RedisJSON](https://oss.redislabs.com/redisjson/commands/)

The following Redis Modules are in the process of being implemented:

  - [RedisGRAPH](https://oss.redislabs.com/redisgraph/commands/)

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

Tests can be performed by running:
```shell
make test
```

## Quickstart

First of all you'll need to have access to a Redis database with the
module(s) loaded, that you'd like to program against. The fastest way
to get started with this is to simply head off to
[redismod](https://github.com/RedisLabsModules/redismod) and follow
their instructions to get a docker image installed and running, which
will supply this.

The following commands, adapted from the [RedisLabs redismod
docker page](https://hub.docker.com/r/redislabs/redismod) should start
a docker image that contains all relevant modules:

```console
$ docker pull redislabs/redismod
$ docker run -p 6379:6379 redislabs/redismod
```

Once your Redis database is set up and running, you can run the
`MODULE LIST` command on it:
```console
$ redis-cli MODULE LIST
1) 1) "name"
   2) "ai"
   3) "ver"
   4) (integer) 10002
2) 1) "name"
   2) "ReJSON"
   3) "ver"
   4) (integer) 10007
3) 1) "name"
   2) "bf"
   3) "ver"
   4) (integer) 20205
4) 1) "name"
   2) "search"
   3) "ver"
   4) (integer) 20006
5) 1) "name"
   2) "timeseries"
   3) "ver"
   4) (integer) 10408
6) 1) "name"
   2) "rg"
   3) "ver"
   4) (integer) 10006
7) 1) "name"
   2) "graph"
   3) "ver"
   4) (integer) 20402
```
In the above example, 7 modules are available. The 3rd module,
BloomFilter (bf), supplies BloomFilter (BF), CuckooFilter (CF),
Count-Min-Sketch (CMS) and Top-K (TOPK).

The `./test` directory contains test cases for all implemented API
calls. In order to run the tests, simply run:

```shell
make test
```

## APIs

### RedisBloom API Documentation

  - [BloomFilter](https://oss.redislabs.com/redisbloom/Bloom_Commands/)
  - [CuckooFilter](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/)
  - [Count-Min-Sketch](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/)
  - [Top-K](https://oss.redislabs.com/redisbloom/TopK_Commands/)
  - [RedisJSON](https://oss.redislabs.com/redisjson/commands/)
  - [RedisGRAPH](https://oss.redislabs.com/redisgraph/commands/)

## TODO

  - Finish the RedisGraph implementation
  - Add a directory with some examples
  - Add doxygen output
  - Install procedure and at least a Debian package
  - Add APIs for: RediSearch, RedisTimeSeries, RedisAI and RedisGears
  - Add the hackathlon video

## Contributing

In principle, contributions are welcome, however this project is in a
seriously heavy development phase and everything can still change. So,
at the moment I'll not be accepting any pull requests. But feel free
to open issues and reviews.

## Authors

The initial version of *redis-plus-plus-modules* was written by
[wingunder](https://github.com/wingunder).
Some ideas and code originate from
[sewenew](https://github.com/sewenew) and his amazing
[redis-plus-plus](https://github.com/sewenew/redis-plus-plus) library.

## Copyright

This software is copyrighted under [Apache Licence
V2.0](https://www.apache.org/licenses/LICENSE-2.0).


## History

This project was started during the RedisConf 2021 hackathlon.

## Links

   - [Redis](https://redis.io/)
   - [redis-plus-plus](https://github.com/sewenew/redis-plus-plus)
   - [hiredis](https://github.com/redis/hiredis)
   - [RedisBloom](https://oss.redislabs.com/redisbloom/Bloom_Commands/)
