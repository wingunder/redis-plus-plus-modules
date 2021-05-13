## [Top-K](https://oss.redislabs.com/redisbloom/TopK_Commands/)

[TOPK.RESERVE](https://oss.redislabs.com/redisbloom/TopK_Commands/#topkreserve)
```C++
    bool reserve(const sw::redis::StringView &key,
                 long long topk,
                 long long width = default_width,
                 long long depth = default_depth,
                 double decay = default_decay);
```
[TOPK.ADD](https://oss.redislabs.com/redisbloom/TopK_Commands/#topkadd)
```C++
    template <typename Input, typename Output>
    void add(const sw::redis::StringView &key, Input first, Input last, Output output);
```
[TOPK.INCRBY](https://oss.redislabs.com/redisbloom/TopK_Commands/#topkincrby)
```C++
    template <typename Input, typename Output>
    void incrby(const sw::redis::StringView &key,
                Input first,
                Input last,
                Output output);
```
[TOPK.QUERY](https://oss.redislabs.com/redisbloom/TopK_Commands/#topkquery)
```C++
    void query(const sw::redis::StringView &key, Input first, Input last, Output output);
```
[TOPK.COUNT](https://oss.redislabs.com/redisbloom/TopK_Commands/#topkcount)
```C++
    void count(const sw::redis::StringView &key, Input first, Input last, Output output);
```
[TOPK.LIST](https://oss.redislabs.com/redisbloom/TopK_Commands/#topklist)
```C++
    void list(const sw::redis::StringView &key, Output output);
```
[TOPK.INFO](https://oss.redislabs.com/redisbloom/TopK_Commands/#topkinfo)
```C++
    void info(const sw::redis::StringView &key, Output &output);
```
