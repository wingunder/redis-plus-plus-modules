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

#ifndef REDIS_PLUS_PLUS_COUNT_MIN_SKETCH_H
#define REDIS_PLUS_PLUS_COUNT_MIN_SKETCH_H

#include "RedisBloom.h"

namespace redis::module {

template <typename RedisInstance>
class CountMinSketch : public RedisBloom<RedisInstance>
{
public:
    explicit CountMinSketch(RedisInstance& redis) : RedisBloom<RedisInstance>(redis, "CMS") {}

    // The following is an implementation of:
    //   https://oss.redislabs.com/redisbloom/CountMinSketch_Commands/

    void initbydim(const sw::redis::StringView &key,
                   long long width,
                   long long depth) {
        RedisBloom<RedisInstance>::_redis.template command<void>("CMS.INITBYDIM",  key, width, depth);
    }

    void initbyprob(const sw::redis::StringView &key,
                    double error,
                    double probability) {
        if (error <= 0 || error >= 1.0) {
            throw sw::redis::Error("Parameter 'error' is out of range.");
        }
        else if (probability <= 0 || probability >= 1.0) {
            throw sw::redis::Error("Parameter 'probability' is out of range.");
        }
        else {
            RedisBloom<RedisInstance>::_redis.template command<void>("CMS.INITBYPROB",  key, error, probability);
        }
    }

    template <typename Input>
    void incrby(const sw::redis::StringView &key,
                Input first,
                Input last) {
        static const std::string cmd = "CMS.INCRBY";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<std::string> args = { cmd.data(), key.data() };
        std::for_each(first, last, [&args](auto &p){
            args.push_back(p.first);
            args.push_back(std::to_string(p.second));
        });
        RedisBloom<RedisInstance>::_redis.command(args.begin(), args.end());
    }

    template <typename Input, typename Output>
    void incrby(const sw::redis::StringView &key,
                Input first,
                Input last,
                Output output) {
        static const std::string cmd = "CMS.INCRBY";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<std::string> args = { cmd.data(), key.data() };
        std::for_each(first, last, [&args](auto &p){
            args.push_back(p.first);
            args.push_back(std::to_string(p.second));
        });
        RedisBloom<RedisInstance>::_redis.command(args.begin(), args.end(), output);
    }

    template <typename Input, typename Output>
    void query(const sw::redis::StringView &key, Input first, Input last, Output output) {
        static const std::string cmd = "CMS.QUERY";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<sw::redis::StringView> args = { cmd, key };
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        RedisBloom<RedisInstance>::_redis.command(args.begin(), args.end(), output);
    }

    template <typename Input>
    void merge(const sw::redis::StringView &key, Input first, Input last) {
        static const std::string cmd = "CMS.MERGE";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<std::string> args = { cmd.data(), key.data() };
        size_t cnt = 0;
        std::for_each(first, last, [&cnt](auto&){ cnt++; });
        args.push_back(std::to_string(cnt));
        std::for_each(first, last, [&args](auto &p){
            args.push_back(p.first);
        });
        args.push_back("WEIGHTS");
        std::for_each(first, last, [&args](auto &p){
            args.push_back(std::to_string(p.second));
        });
        RedisBloom<RedisInstance>::_redis.command(args.begin(), args.end());
    }

    template <typename Input>
    void merge_default_weight(const sw::redis::StringView &key, Input first, Input last) {
        static const std::string cmd = "CMS.MERGE";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<std::string> args = { cmd.data(), key.data() };
        size_t cnt = 0;
        std::for_each(first, last, [&cnt](auto&){ cnt++; });
        args.push_back(std::to_string(cnt));
        std::for_each(first, last, [&args](auto &i){ args.push_back(i); });
        args.push_back("WEIGHTS");
        auto weight = std::to_string(default_merge_weight);
        std::for_each(first, last, [&args, weight](auto&){ args.push_back(weight); });
        RedisBloom<RedisInstance>::_redis.command(args.begin(), args.end());
    }

private:

    static const long long default_merge_weight = 1;
};

} // namespace

#endif
