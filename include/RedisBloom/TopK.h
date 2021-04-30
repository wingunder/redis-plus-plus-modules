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

#ifndef REDIS_PLUS_PLUS_TOP_K_H
#define REDIS_PLUS_PLUS_TOP_K_H

#include "RedisBloom.h"

namespace redis::module {

template <typename RedisInstance>
class TopK : public RedisBloom<RedisInstance>
{
public:
    explicit TopK(RedisInstance& redis) : RedisBloom<RedisInstance>(redis, "TOPK") {}

    // The following is an implementation of:
    //   https://oss.redislabs.com/redisbloom/TopK_Commands/

    bool reserve(const sw::redis::StringView &key,
                 long long topk,
                 long long width = default_width,
                 long long depth = default_depth,
                 double decay = default_decay) {
        auto result = RedisBloom<RedisInstance>::_redis.template
            command<std::string>("TOPK.RESERVE",  key, topk, width, depth, decay);
        return result == "OK";
    }

    template <typename Input, typename Output>
    void add(const sw::redis::StringView &key, Input first, Input last, Output output) {
        static const std::string cmd = "TOPK.ADD";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<sw::redis::StringView> args = { cmd, key };
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        RedisBloom<RedisInstance>::_redis.command(args.begin(), args.end(), output);
    }

    template <typename Input>
    void incrby(const sw::redis::StringView &key,
                Input first,
                Input last) {
        static const std::string cmd = "TOPK.INCRBY";
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
        static const std::string cmd = "TOPK.INCRBY";
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
        static const std::string cmd = "TOPK.QUERY";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<sw::redis::StringView> args = { cmd, key };
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        RedisBloom<RedisInstance>::_redis.command(args.begin(), args.end(), output);
    }

    template <typename Input, typename Output>
    void count(const sw::redis::StringView &key, Input first, Input last, Output output) {
        static const std::string cmd = "TOPK.COUNT";
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<sw::redis::StringView> args = { cmd, key };
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        RedisBloom<RedisInstance>::_redis.command(args.begin(), args.end(), output);
    }

    template <typename Output>
    void list(const sw::redis::StringView &key, Output output) {
        static const std::string cmd = "TOPK.LIST";
        std::vector<sw::redis::StringView> args = { cmd, key };
        RedisBloom<RedisInstance>::_redis.command(args.begin(), args.end(), output);
    }

    // The decay needs to be converted,
    template <typename Output>
    void info(const sw::redis::StringView &key, Output &output) {
        auto reply = RedisBloom<RedisInstance>::_redis.command("TOPK.INFO", key);
        if (reply == nullptr) {
            throw sw::redis::ProtoError("Null reply");
        }
        for (std::size_t idx = 0; idx<reply->elements;) {
            auto *sub_reply_key = reply->element[idx++];
            if (sub_reply_key == nullptr || !sw::redis::reply::is_status(*sub_reply_key)) {
                throw sw::redis::ProtoError("Null array element.key reply");
            }

            auto *sub_reply = reply->element[idx++];
            if (sub_reply == nullptr) {
                throw sw::redis::ProtoError("Null array element.val reply");
            }
            else if (sw::redis::reply::is_integer(*sub_reply)) {
                output.insert(std::pair<std::string, double>
                              (std::string(sub_reply_key->str),
                               double(sub_reply->integer)));
            }
            else if (sw::redis::reply::is_string(*sub_reply)) {
                output.insert(std::pair<std::string, double>
                              (std::string(sub_reply_key->str),
                               std::stod(sub_reply->str)));
            }
            else {
                throw sw::redis::ProtoError("Wrong array element reply type " + std::to_string(sub_reply->type));
            }
        }
    }

private:
    constexpr static const long long default_width = 8;
    constexpr static const long long default_depth = 7;
    constexpr static const double default_decay = 0.9;
};

} // namespace

#endif
