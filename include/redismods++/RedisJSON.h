/**
 * Copyright 2021 Pieter du Preez <pdupreez@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#ifndef REDIS_PLUS_PLUS_REDIS_JSON_H
#define REDIS_PLUS_PLUS_REDIS_JSON_H

#include "Module.h"
#include <sstream>
#include <iomanip>

namespace redis::module {

template <typename RedisInstance>
class RedisJSON : public Module<RedisInstance>
{
public:
    enum class DelOpt{ NX = 0, XX = 1 };

    explicit RedisJSON(RedisInstance& redis)
        : Module<RedisInstance>(redis, "ReJSON", "JSON")
        {}

    auto del(const sw::redis::StringView &key) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.DEL", key);
    }

    auto del(const sw::redis::StringView &key,
             const sw::redis::StringView &path) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.DEL", key, path);
    }

    template <typename Input>
    auto get(const sw::redis::StringView &key,
             Input first, Input last,
             const sw::redis::StringView &indent = "",
             const sw::redis::StringView &newline = "",
             const sw::redis::StringView &space = "",
             bool noescape = false) {
        static const std::string cmd = "JSON.GET";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<sw::redis::StringView> args = { cmd, key };
        if (indent.data() != "") {
            args.push_back("INDENT");
            args.push_back(indent);
        }
        if (newline.data() != "") {
            args.push_back("NEWLINE");
            args.push_back(newline);
        }
        if (space.data() != "") {
            args.push_back("SPACE");
            args.push_back(space);
        }
        if (noescape) {
            args.push_back("NOESCAPE");
        }
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        return Module<RedisInstance>::_redis.template
            command<std::string>(args.begin(), args.end());
    }

    auto get(const sw::redis::StringView &key,
             const sw::redis::StringView &path,
             const sw::redis::StringView &indent = "",
             const sw::redis::StringView &newline = "",
             const sw::redis::StringView &space = "",
             bool noescape = false) {
        std::vector<sw::redis::StringView> p = {path};
        return get(key, p.begin(), p.end(), indent, newline, space, noescape);
    }

    template <typename Input, typename Output>
    void mget(Input first, Input last,
              const std::string& path,
              Output &result) {
        static const std::string cmd = "JSON.MGET";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<sw::redis::StringView> args = { cmd };
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        args.push_back(path);
        Module<RedisInstance>::_redis.template
            command(args.begin(), args.end(), std::back_inserter(result));
    }

    template <typename Input, typename Output>
    void mget(Input first, Input last,
              Output &result) {
        mget(first, last, ".", result);
    }

    auto forget(const sw::redis::StringView &key) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.DEL", key);
    }

    auto forget(const sw::redis::StringView &key,
                const sw::redis::StringView &path) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.DEL", key, path);
    }

    bool set(const sw::redis::StringView &key,
             const sw::redis::StringView &path,
             const sw::redis::StringView &value,
             const DelOpt& opt) {
        auto ret = Module<RedisInstance>::_redis.template
            command<sw::redis::OptionalString>
            ("JSON.SET", key, path, value.data(),
             (opt == DelOpt::NX) ? "NX" : "XX");
        return (ret && *ret == "OK");
    }

    bool set(const sw::redis::StringView &key,
             const sw::redis::StringView &path,
             long long value,
             const DelOpt& opt) {
        auto ret = Module<RedisInstance>::_redis.template
            command<sw::redis::OptionalString>
            ("JSON.SET", key, path, value,
             (opt == DelOpt::NX) ? "NX" : "XX");
        return (ret && *ret == "OK");
    }

    bool set(const sw::redis::StringView &key,
             const sw::redis::StringView &path,
             const sw::redis::StringView &value) {
        auto ret = Module<RedisInstance>::_redis.template
            command<sw::redis::OptionalString>
            ("JSON.SET", key, path, value.data());
        return (ret && *ret == "OK");
    }

    bool set(const sw::redis::StringView &key,
             const sw::redis::StringView &path,
             long long value) {
        auto ret = Module<RedisInstance>::_redis.template
            command<sw::redis::OptionalString>
            ("JSON.SET", key, path, value);
        return (ret && *ret == "OK");
    }

    auto arrlen(const sw::redis::StringView &key) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.ARRLEN", key);
    }

    auto arrlen(const sw::redis::StringView &key,
                const sw::redis::StringView &path) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.ARRLEN", key, path);
    }

    auto arrpop(const sw::redis::StringView &key,
                const sw::redis::StringView &path,
                long long index = -1) {
        return Module<RedisInstance>::_redis.template
            command<std::string>("JSON.ARRPOP", key, path, index);
    }

    auto arrpop(const sw::redis::StringView &key) {
        return Module<RedisInstance>::_redis.template
            command<std::string>("JSON.ARRPOP", key);
    }

    template <typename Input>
    auto arrinsert(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   long long index,
                   Input first, Input last) {
        static const std::string cmd = "JSON.ARRINSERT";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<std::string> args = { cmd, key.data(), path.data(), std::to_string(index) };
        std::for_each(first, last, [this, &args](auto &s){
            args.push_back(s.data());
        });
        return Module<RedisInstance>::_redis.template
            command<long long>(args.begin(), args.end());
    }

    auto arrinsert(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   long long index,
                   const sw::redis::StringView &json) {
        std::vector<std::string> j = {json.data()};
        return arrinsert(key, path, index, j.begin(), j.end());
    }

    template <typename Input>
    auto arrappend(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   Input first, Input last) {
        static const std::string cmd = "JSON.ARRAPPEND";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<std::string> args = { cmd, key.data(), path.data() };
        std::for_each(first, last, [this, &args](auto &s){
            args.push_back(s);
        });
        return Module<RedisInstance>::_redis.template
            command<long long>(args.begin(), args.end());
    }

    auto arrtrim(const sw::redis::StringView &key,
                 const sw::redis::StringView &path,
                 long long from,
                 long long to) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.ARRTRIM", key, path, from, to);
    }

    auto arrindex(const sw::redis::StringView &key,
                  const sw::redis::StringView &path,
                  const sw::redis::StringView &json,
                  long long from = 0,
                  long long to = 0) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.ARRINDEX", key, path, json, std::to_string(from), std::to_string(to));
    }

    auto strlen(const sw::redis::StringView &key) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.STRLEN", key);
    }

    auto strlen(const sw::redis::StringView &key,
                const sw::redis::StringView &path) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.STRLEN", key, path);
    }

    auto type(const sw::redis::StringView &key) {
        return Module<RedisInstance>::_redis.template
            command<std::string>("JSON.TYPE", key);
    }

    auto type(const sw::redis::StringView &key,
              const sw::redis::StringView &path) {
        return Module<RedisInstance>::_redis.template
            command<std::string>("JSON.TYPE", key, path);
    }

    auto resp(const sw::redis::StringView &key) {
        return Module<RedisInstance>::_redis.template
            command<std::string>("JSON.RESP", key);
    }

    auto resp(const sw::redis::StringView &key,
              const sw::redis::StringView &path) {
        return Module<RedisInstance>::_redis.template
            command<std::string>("JSON.RESP", key, path);
    }

    auto numincrby(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   long long num) {
        return std::stoll(Module<RedisInstance>::_redis.template
                          command<std::string>("JSON.NUMINCRBY", key, path, num));
    }

    auto nummultby(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   long long num) {
        return std::stoll(Module<RedisInstance>::_redis.template
                          command<std::string>("JSON.NUMMULTBY", key, path, num));
    }

    auto strappend(const sw::redis::StringView &key,
                   const sw::redis::StringView &str) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.STRAPPEND", key, str.data());
    }

    auto strappend(const sw::redis::StringView &key,
                   const sw::redis::StringView &path,
                   const sw::redis::StringView &str) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.STRAPPEND", key, path, str.data());
    }

    template <typename Output>
    void objkeys(const sw::redis::StringView &key,
                 const sw::redis::StringView &path,
                 Output &result) {
        Module<RedisInstance>::_redis.template
            command("JSON.OBJKEYS", key, path, std::back_inserter(result));
    }

    template <typename Output>
    void objkeys(const sw::redis::StringView &key,
                 Output &result) {
        Module<RedisInstance>::_redis.template
            command("JSON.OBJKEYS", key, std::back_inserter(result));
    }

    auto objlen(const sw::redis::StringView &key,
                const sw::redis::StringView &path) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.OBJLEN", key, path);
    }

    auto objlen(const sw::redis::StringView &key) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.OBJLEN", key);
    }

};

} // namespace

#endif
