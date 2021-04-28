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

#ifndef REDIS_PLUS_PLUS_BLOOM_FILTER_H
#define REDIS_PLUS_PLUS_BLOOM_FILTER_H

#include "BloomBase.h"

namespace redis::module {

template <typename RedisInstance>
class BloomFilter : public BloomBase<RedisInstance>
{
public:
    explicit BloomFilter(RedisInstance& redis) : BloomBase<RedisInstance>(redis, "BF") {}

    // The following is an implementation of:
    //   https://oss.redislabs.com/redisbloom/Bloom_Commands/

    void reserve(const sw::redis::StringView &key,
                 double error_rate,
                 long long capacity,
                 bool nonscaling,
                 long long expansion = default_expansion_rate) {
        if (nonscaling) {
            BloomBase<RedisInstance>::_redis.template command<void>("BF.RESERVE",  key, error_rate, capacity, "EXPANSION", expansion, "NOSCALING");
        }
        else {
            BloomBase<RedisInstance>::_redis.template command<void>("BF.RESERVE",  key, error_rate, capacity, "EXPANSION", expansion);
        };
    }

    template <typename Input, typename Output>
    void madd(const std::string& key,
              Input first,
              Input last,
              Output &result) {
        m_command("BF.MADD", key, first, last, result);
    }

    template <typename Input, typename Output>
    void insert(const sw::redis::StringView &key,
                double error_rate,
                long long capacity,
                bool nonscaling,
                bool nocreate,
                Input first,
                Input last,
                Output &result,
                long long expansion = default_expansion_rate) {
        std::string cmd = "BF.INSERT";
        sw::redis::range_check(cmd.c_str(), first, last);

        std::vector<sw::redis::StringView> args = { cmd, key,
            "CAPACITY", std::to_string(capacity),
            "ERROR", std::to_string(error_rate),
            "EXPANSION", std::to_string(expansion)
        };
        if (nonscaling && nocreate) {
            throw sw::redis::Error("In BF.INSERT, nonscaling and nocreate are mutually exclusive. Setting both is not allowed.");
        }
        else if (nonscaling) {
            args.push_back("NONSCALING");
        }
        else if (nocreate) {
            args.push_back("NOCREATE");
        }
        args.push_back("ITEMS");
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        BloomBase<RedisInstance>::_redis.command(args.begin(), args.end(), std::back_inserter(result));
    }

    template <typename Input, typename Output>
    void mexists(const std::string& key,
                 Input first,
                 Input last,
                 Output &result) {
        m_command("BF.MEXISTS", key, first, last, result);
    }

private:
    template <typename Input, typename Output>
    void m_command(const std::string& cmd,
                   const std::string& key,
                   Input first,
                   Input last,
                   Output &result) {
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<std::string> args = { cmd, key };
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        BloomBase<RedisInstance>::_redis.command(args.begin(), args.end(), std::back_inserter(result));
    }

    static const long long default_expansion_rate = 2;
};

} // namespace

#endif
