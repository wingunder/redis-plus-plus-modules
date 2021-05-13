## [BloomFilter](https://oss.redislabs.com/redisbloom/Bloom_Commands/)

[BF.RESERVE](https://oss.redislabs.com/redisbloom/Bloom_Commands/#bfreserve)
```C++
    void reserve(const sw::redis::StringView &key,
                 double error_rate,
                 long long capacity,
                 bool nonscaling,
                 long long expansion = default_expansion_rate);
```
[BF.ADD](https://oss.redislabs.com/redisbloom/Bloom_Commands/#bfadd)
```C++
    long long add(const sw::redis::StringView &key,
                  const sw::redis::StringView &item);
```
[BF.MADD](https://oss.redislabs.com/redisbloom/Bloom_Commands/#bfmadd)
```C++
    template <typename Input, typename Output>
    void madd(const std::string& key,
              Input first,
              Input last,
              Output &result);
```
[BF.INSERT](https://oss.redislabs.com/redisbloom/Bloom_Commands/#bfinsert)
```C++
    template <typename Input, typename Output>
    void insert(const sw::redis::StringView &key,
                double error_rate,
                long long capacity,
                bool nonscaling,
                bool nocreate,
                Input first,
                Input last,
                Output &result,
                long long expansion = default_expansion_rate);
```
[BF.EXISTS](https://oss.redislabs.com/redisbloom/Bloom_Commands/#bfexists)
```C++
    long long exists(const sw::redis::StringView &key,
                     const sw::redis::StringView &item);
```
[BF.MEXISTS](https://oss.redislabs.com/redisbloom/Bloom_Commands/#bfmexists)
```C++
    template <typename Input, typename Output>
    void mexists(const std::string& key,
                 Input first,
                 Input last,
                 Output &result);
```
[BF.SCANDUMP](https://oss.redislabs.com/redisbloom/Bloom_Commands/#bfscandump)
```C++
    long long scandump(const sw::redis::StringView &key,
                       long long iter,
                       std::pair<long long, std::vector<unsigned char>>& result);
```
[BF.LOADCHUNK](https://oss.redislabs.com/redisbloom/Bloom_Commands/#bfloadchunk)
```C++
    void loadchunk(const sw::redis::StringView &key,
                   const std::pair<long long, std::vector<unsigned char>>& payload);
```
[BF.INFO](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfinfo)
```C++
    template <typename Output>
    void info(const sw::redis::StringView &key, Output &output);
```
