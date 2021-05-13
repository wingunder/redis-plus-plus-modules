## [CuckooFilter](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/)

[CF.RESERVE](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfreserve)
```C++
    void reserve(const sw::redis::StringView &key,
                 long long capacity,
                 long long bucketsize,
                 long long max_iterations = default_max_iterations,
                 long long expansion = default_expansion_rate);
```
[CF.ADD](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfadd)
```C++
    long long add(const sw::redis::StringView &key,
                  const sw::redis::StringView &item);
```
[CF.ADDNX](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfaddnx)
```C++
    long long addnx(const sw::redis::StringView &key,
                    const sw::redis::StringView &item);
```
[CF.INSERT](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfinsert)
```C++
    template <typename Input, typename Output>
    void insert(const sw::redis::StringView &key,
                  long long capacity,
                  bool nocreate,
                  Input first,
                  Input last,
                  Output &result);
```
[CF.INSERTNX](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfinsertnx)
```C++
    void insertnx(const sw::redis::StringView &key,
                  long long capacity,
                  bool nocreate,
                  Input first,
                  Input last,
                  Output &result);
```
[CF.EXISTS](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cf)
```C++
    long long exists(const sw::redis::StringView &key,
                     const sw::redis::StringView &item);
```
[CF.DEL](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfdel)
```C++
    long long del(const sw::redis::StringView &key,
                  const sw::redis::StringView &item);
```
[CF.COUNT](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfcount)
```C++
    template <typename Input, typename Output>
    long long count(const sw::redis::StringView &key,
                    const sw::redis::StringView &item);
```
[CF.SCANDUMP](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfscandump)
```C++
    long long scandump(const sw::redis::StringView &key,
                       long long iter,
                       std::pair<long long, std::vector<unsigned char>>& result);
```
[CF.LOADCHUNK](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfloadchunk)
```C++
    void loadchunk(const sw::redis::StringView &key,
                   const std::pair<long long, std::vector<unsigned char>>& payload);
```
[CF.INFO](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/#cfinfo)
```C++
    template <typename Output>
    void info(const sw::redis::StringView &key, Output &output);
```
