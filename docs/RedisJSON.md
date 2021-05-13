## [RedisJSON](https://oss.redislabs.com/redisjson/commands/)

[JSON.DEL](https://oss.redislabs.com/redisjson/commands/#jsondel)
```C++
    auto del(const sw::redis::StringView &key);
    auto del(const sw::redis::StringView &key,
             const sw::redis::StringView &path);
```
[JSON.GET](https://oss.redislabs.com/redisjson/commands/#jsonget)
```C++
    template <typename Input>
    auto get(const sw::redis::StringView &key,
             Input first, Input last,
             const sw::redis::StringView &indent = "",
             const sw::redis::StringView &newline = "",
             const sw::redis::StringView &space = "",
             bool noescape = false);
    auto get(const sw::redis::StringView &key,
             const sw::redis::StringView &path,
             const sw::redis::StringView &indent = "",
             const sw::redis::StringView &newline = "",
             const sw::redis::StringView &space = "",
             bool noescape = false);
```
[JSON.MGET](https://oss.redislabs.com/redisjson/commands/#jsonmget)
```C++
    template <typename Input, typename Output>
    void mget(Input first, Input last,
              const std::string& path,
              Output &result);

    template <typename Input, typename Output>
    void mget(Input first, Input last,
              Output &result);
```
[JSON.SET](https://oss.redislabs.com/redisjson/commands/#jsonset)
```C++
    bool set(const sw::redis::StringView &key,
             const sw::redis::StringView &path,
             const sw::redis::StringView &value,
             const DelOpt& opt);

    bool set(const sw::redis::StringView &key,
             const sw::redis::StringView &path,
             long long value,
             const DelOpt& opt);

    bool set(const sw::redis::StringView &key,
             const sw::redis::StringView &path,
             const sw::redis::StringView &value);

    bool set(const sw::redis::StringView &key,
             const sw::redis::StringView &path,
             long long value);
```
[JSON.TYPE](https://oss.redislabs.com/redisjson/commands/#jsontype)
```C++
    auto type(const sw::redis::StringView &key);

    auto type(const sw::redis::StringView &key,
              const sw::redis::StringView &path);
```
[JSON.NUMINCRBY](https://oss.redislabs.com/redisjson/commands/#jsonnumincrby)
```C++
    auto numincrby(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   long long num);
```
[JSON.NUMMULTBY](https://oss.redislabs.com/redisjson/commands/#jsonnummultby)
```C++
    auto nummultby(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   long long num);
```
[JSON.STRAPPEND](https://oss.redislabs.com/redisjson/commands/#jsonstrappend)
```C++
    auto strappend(const sw::redis::StringView &key,
                   const sw::redis::StringView &str);

    auto strappend(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   const sw::redis::StringView &str);
```
[JSON.STRLEN](https://oss.redislabs.com/redisjson/commands/#jsonstrlen)
```C++
    auto strlen(const sw::redis::StringView &key);

    auto strlen(const sw::redis::StringView &key,
                const sw::redis::StringView &path);
```
[JSON.ARRAPPEND](https://oss.redislabs.com/redisjson/commands/#jsonarrappend)
```C++
    template <typename Input>
    auto arrappend(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   Input first, Input last);
```
[JSON.ARRINDEX](https://oss.redislabs.com/redisjson/commands/#jsonarrindex)
```C++
    auto arrindex(const sw::redis::StringView &key,
                  const sw::redis::StringView &path,
                  const sw::redis::StringView &json,
                  long long from = 0,
                  long long to = 0);
```
[JSON.ARRINSERT](https://oss.redislabs.com/redisjson/commands/#jsonarrinsert)
```C++
    template <typename Input>
    auto arrinsert(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   long long index,
                   Input first, Input last);

    auto arrinsert(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   long long index,
                   const sw::redis::StringView &json);
```
[JSON.ARRLEN](https://oss.redislabs.com/redisjson/commands/#jsonarrlen)
```C++
    auto arrlen(const sw::redis::StringView &key);

    auto arrlen(const sw::redis::StringView &key,
                const sw::redis::StringView &path);
```
[JSON.ARRPROP](https://oss.redislabs.com/redisjson/commands/#jsonarrprop)
```C++
    auto arrpop(const sw::redis::StringView &key,
                const sw::redis::StringView &path,
                long long index = -1);

    auto arrpop(const sw::redis::StringView &key);
```
[JSON.ARRTRIM](https://oss.redislabs.com/redisjson/commands/#jsonarrtrim)
```C++
    auto arrtrim(const sw::redis::StringView &key,
                 const sw::redis::StringView &path,
                 long long from,
                 long long to);
```
[JSON.OBJKEYS](https://oss.redislabs.com/redisjson/commands/#jsonobjkeys)
```C++
    template <typename Output>
    void objkeys(const sw::redis::StringView &key,
                 const sw::redis::StringView &path,
                 Output &result);

    template <typename Output>
    void objkeys(const sw::redis::StringView &key,
                 Output &result);
```
[JSON.OBJLEN](https://oss.redislabs.com/redisjson/commands/#jsonobjlen)
```C++
    auto objlen(const sw::redis::StringView &key,
                const sw::redis::StringView &path);

    auto objlen(const sw::redis::StringView &key);
```
[JSON.DEBUG](https://oss.redislabs.com/redisjson/commands/#jsondebug)
```C++
    auto debugmem(const sw::redis::StringView &key,
                  const sw::redis::StringView &path);

    auto debugmem(const sw::redis::StringView &key);
```
[JSON.FORGET](https://oss.redislabs.com/redisjson/commands/#jsonforget)
```C++
    auto forget(const sw::redis::StringView &key);

    auto forget(const sw::redis::StringView &key,
                const sw::redis::StringView &path);
```
[JSON.RESP](https://oss.redislabs.com/redisjson/commands/#jsonresp)
```C++
    auto resp(const sw::redis::StringView &key);

    auto resp(const sw::redis::StringView &key,
              const sw::redis::StringView &path);
```
