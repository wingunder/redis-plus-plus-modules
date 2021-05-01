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

    auto del(const sw::redis::StringView& key) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.DEL", key);
    }

    auto del(const sw::redis::StringView& key,
             const sw::redis::StringView& path) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.DEL", key, path);
    }

    template <typename Input>
    auto get(const sw::redis::StringView& key,
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

    auto forget(const sw::redis::StringView& key) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.DEL", key);
    }

    auto forget(const sw::redis::StringView& key,
                const sw::redis::StringView& path) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.DEL", key, path);
    }

    bool set(const sw::redis::StringView& key,
             const sw::redis::StringView& path,
             const sw::redis::StringView& value,
             const DelOpt& opt) {
        auto ret = Module<RedisInstance>::_redis.template
            command<sw::redis::OptionalString>
            ("JSON.SET", key, path, quote_escape(value.data()),
             (opt == DelOpt::NX) ? "NX" : "XX");
        return (ret && *ret == "OK");
    }

    bool set(const sw::redis::StringView& key,
             const sw::redis::StringView& path,
             long long value,
             const DelOpt& opt) {
        auto ret = Module<RedisInstance>::_redis.template
            command<sw::redis::OptionalString>
            ("JSON.SET", key, path, value,
             (opt == DelOpt::NX) ? "NX" : "XX");
        return (ret && *ret == "OK");
    }

    bool set(const sw::redis::StringView& key,
             const sw::redis::StringView& path,
             const sw::redis::StringView& value) {
        auto ret = Module<RedisInstance>::_redis.template
            command<sw::redis::OptionalString>
            ("JSON.SET", key, path, quote_escape(value.data()));
        return (ret && *ret == "OK");
    }

    bool set(const sw::redis::StringView& key,
             const sw::redis::StringView& path,
             long long value) {
        auto ret = Module<RedisInstance>::_redis.template
            command<sw::redis::OptionalString>
            ("JSON.SET", key, path, value);
        return (ret && *ret == "OK");
    }

    auto arrlen(const sw::redis::StringView& key) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.ARRLEN", key);
    }

    auto arrlen(const sw::redis::StringView& key,
                const sw::redis::StringView& path) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.ARRLEN", key, path);
    }

    auto strlen(const sw::redis::StringView& key) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.STRLEN", key);
    }

    auto strlen(const sw::redis::StringView& key,
                const sw::redis::StringView& path) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.STRLEN", key, path);
    }

    auto type(const sw::redis::StringView& key) {
        return Module<RedisInstance>::_redis.template
            command<std::string>("JSON.TYPE", key);
    }

    auto type(const sw::redis::StringView& key,
              const sw::redis::StringView& path) {
        return Module<RedisInstance>::_redis.template
            command<std::string>("JSON.TYPE", key, path);
    }

    auto resp(const sw::redis::StringView& key) {
        return Module<RedisInstance>::_redis.template
            command<std::string>("JSON.RESP", key);
    }

    auto resp(const sw::redis::StringView& key,
              const sw::redis::StringView& path) {
        return Module<RedisInstance>::_redis.template
            command<std::string>("JSON.RESP", key, path);
    }

    auto numincrby(const sw::redis::StringView& key,
                   const sw::redis::StringView& path,
                   long long num) {
        return std::stoll(Module<RedisInstance>::_redis.template
                          command<std::string>("JSON.NUMINCRBY", key, path, num));
    }

    auto nummultby(const sw::redis::StringView& key,
                   const sw::redis::StringView& path,
                   long long num) {
        return std::stoll(Module<RedisInstance>::_redis.template
                          command<std::string>("JSON.NUMMULTBY", key, path, num));
    }

    auto strappend(const sw::redis::StringView& key,
                   const sw::redis::StringView& str) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.STRAPPEND", key, quote_escape(str.data()));
    }

    auto strappend(const sw::redis::StringView& key,
                   const sw::redis::StringView& path,
                   const sw::redis::StringView& str) {
        return Module<RedisInstance>::_redis.template
            command<long long>("JSON.STRAPPEND", key, path, quote_escape(str.data()));
    }

private:
    bool is_struct(const std::string &s) {
        if (!s.empty()) {
            size_t start = s.find_first_not_of(" \n\r\t\f\v");
            if (start == std::string::npos) {
                start = 0;
            }
            if ((s.at(start) == '{' || s.at(start) == '[')) {
                return true;
            }
        }
        return false;
    }

    std::string quote_escape(const std::string &s) {
        if (is_struct(s)) {
            return s;
        }
        std::ostringstream o;
        o << "\"";
        // Taken from: https://stackoverflow.com/a/33799784
        for (auto c = s.cbegin(); c != s.cend(); c++) {
            switch (*c) {
            case '"': o << "\\\""; break;
            case '\\': o << "\\\\"; break;
            case '\b': o << "\\b"; break;
            case '\f': o << "\\f"; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default:
                if ('\x00' <= *c && *c <= '\x1f') {
                    o << "\\u"
                      << std::hex << std::setw(4) << std::setfill('0') << (int)*c;
                } else {
                    o << *c;
                }
            }
        }
        o << "\"";
        return o.str();
    }

};

} // namespace

#endif
