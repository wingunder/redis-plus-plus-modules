## [Count-Min-Sketch](https://oss.redislabs.com/redisbloom/Cuckoo_Commands/)

[CMS.INITBYDIM](https://oss.redislabs.com/redisbloom/CountMinSketch_Commands/#cmsinitbydim)
```C++
    void initbydim(const sw::redis::StringView &key,
                   long long width,
                   long long depth);
```
[CMS.INITBYPROB](https://oss.redislabs.com/redisbloom/CountMinSketch_Commands/#cmsinitbyprob)
```C++
    void initbyprob(const sw::redis::StringView &key,
                    double error,
                    double probability);
```
[CMS.INCRBY](https://oss.redislabs.com/redisbloom/CountMinSketch_Commands/#cmsinrcby)
```C++
    template <typename Input>
    void incrby(const sw::redis::StringView &key,
                Input first,
                Input last);

    template <typename Input, typename Output>
    void incrby(const sw::redis::StringView &key,
                Input first,
                Input last,
                Output output);
```
[CMS.QUERY](https://oss.redislabs.com/redisbloom/CountMinSketch_Commands/#cmsquery)
```C++
    template <typename Input, typename Output>
    void query(const sw::redis::StringView &key, 
               Input first, 
               Input last, 
               Output output);
```
[CMS.MERGE](https://oss.redislabs.com/redisbloom/CountMinSketch_Commands/#cmsmerge)
```C++
    void merge(const sw::redis::StringView &key, Input first, Input last);
```
[CMS.INFO](https://oss.redislabs.com/redisbloom/CountMinSketch_Commands/#cmsinfo)
```C++
    void info(const sw::redis::StringView &key, Output &output);
```
