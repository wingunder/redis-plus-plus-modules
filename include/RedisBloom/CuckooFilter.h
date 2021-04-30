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

#ifndef REDIS_PLUS_PLUS_CUCKOO_FILTER_H
#define REDIS_PLUS_PLUS_CUCKOO_FILTER_H

#include "BloomCuckooBase.h"

namespace redis::module {

template <typename RedisInstance>
class CuckooFilter : public BloomCuckooBase<RedisInstance>
{
public:
    explicit CuckooFilter(RedisInstance &redis) : BloomCuckooBase<RedisInstance>(redis, "CF") {}

    // The following is an implementation of:
    //   https://oss.redislabs.com/redisbloom/Cuckoo_Commands/

    void reserve(const sw::redis::StringView &key,
                 long long capacity,
                 long long bucketsize,
                 long long max_iterations = default_max_iterations,
                 long long expansion = default_expansion_rate) {
        BloomBase<RedisInstance>::_redis.template command<void>
            ("CF.RESERVE",  key, capacity,
             "BUCKETSIZE", bucketsize,
             "MAXITERATIONS",  max_iterations,
             "EXPANSION", expansion);
    }

    long long addnx(const sw::redis::StringView &key,
                    const sw::redis::StringView &item) {
        return BloomBase<RedisInstance>::_redis.template command<long long>("CF.ADDNX",  key, item);
    }

    long long del(const sw::redis::StringView &key,
                  const sw::redis::StringView &item) {
        return BloomBase<RedisInstance>::_redis.template command<long long>("CF.DEL",  key, item);
    }

    template <typename Input, typename Output>
    void insert(const sw::redis::StringView &key,
                  long long capacity,
                  bool nocreate,
                  Input first,
                  Input last,
                  Output &result) {
        cf_insert("CF.INSERT", key, capacity, nocreate, first, last, result);
    }

    template <typename Input, typename Output>
    void insertnx(const sw::redis::StringView &key,
                  long long capacity,
                  bool nocreate,
                  Input first,
                  Input last,
                  Output &result) {
        cf_insert("CF.INSERTNX", key, capacity, nocreate, first, last, result);
    }

    template <typename Input, typename Output>
    long long count(const sw::redis::StringView &key,
                    const sw::redis::StringView &item) {
        return BloomBase<RedisInstance>::_redis.template command<long long>("CF.COUNT",  key, item);
    }

private:
    template <typename Input, typename Output>
    void cf_insert(const std::string& cmd,
                   const sw::redis::StringView &key,
                   long long capacity,
                   bool nocreate,
                   Input first,
                   Input last,
                   Output &result) {
        sw::redis::range_check(cmd.c_str(), first, last);

        std::vector<sw::redis::StringView> args = { cmd, key, "CAPACITY", std::to_string(capacity) };
        if (nocreate) {
            args.push_back("NOCREATE");
        }
        args.push_back("ITEMS");
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        BloomBase<RedisInstance>::_redis.command(args.begin(), args.end(), std::back_inserter(result));
    }

    static const long long default_expansion_rate = 1;
    static const long long default_max_iterations = 20;
};

} // namespace

#endif
