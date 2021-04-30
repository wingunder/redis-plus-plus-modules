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

#ifndef REDIS_PLUS_PLUS_BLOOM_CUCKOO_BASE_H
#define REDIS_PLUS_PLUS_BLOOM_CUCKOO_BASE_H

#include "RedisBloom.h"

namespace redis::module {

template <typename RedisInstance>
class BloomCuckooBase : public RedisBloom<RedisInstance>
{
public:
    // The following is an implementation of common calls in:
    //   https://oss.redislabs.com/redisbloom/Bloom_Commands/
    //   https://oss.redislabs.com/redisbloom/Cuckoo_Commands/

    long long add(const sw::redis::StringView &key,
                  const sw::redis::StringView &item) {
        return RedisBloom<RedisInstance>::_redis.template command<long long>(_add_cmd,  key, item);
    }

    long long exists(const sw::redis::StringView &key,
                     const sw::redis::StringView &item) {
        return RedisBloom<RedisInstance>::_redis.template command<long long>(_exists_cmd,  key, item);
    }

    void
    loadchunk(const sw::redis::StringView &key,
              const std::pair<long long, std::vector<unsigned char>>& payload) {
        sw::redis::StringView data(reinterpret_cast<const char*>(payload.second.data()), payload.second.size());
        RedisBloom<RedisInstance>::_redis.template command<void>(_loadchunk_cmd,  key, std::to_string(payload.first), data);
    }

    long long
    scandump(const sw::redis::StringView &key,
             long long iter,
             std::pair<long long, std::vector<unsigned char>>& result) {
        std::vector<sw::redis::StringView> args = { _scandump_cmd, key, std::to_string(iter) };
        auto reply = RedisBloom<RedisInstance>::_redis.command(args.begin(), args.end());
        if (!sw::redis::reply::is_array(*reply)) {
            throw sw::redis::ProtoError("Expect ARRAY reply");
        }

        if (reply->elements != 2) {
            throw sw::redis::ProtoError("NOT key-value PAIR reply");
        }

        if (reply->element == nullptr) {
            throw sw::redis::ProtoError("Null PAIR reply");
        }

        result.second.clear();
        auto *first = reply->element[0];
        auto *second = reply->element[1];
        if (first == nullptr) {
            throw sw::redis::ProtoError("Null iterator reply");
            result.first = 0;
        }
        else {
            result.first = sw::redis::reply::parse<long long>(*first);
        }
        if (second != nullptr && sw::redis::reply::is_string(*second)) {
            auto str = sw::redis::reply::parse<std::string>(*second);
            std::copy(str.begin(), str.end(), std::back_inserter(result.second));
        }
        return result.first;
    }

protected:
    explicit BloomCuckooBase(RedisInstance& redis, const std::string& command_prefix)
        : RedisBloom<RedisInstance>(redis, command_prefix),
          _add_cmd(command_prefix + ".ADD"),
          _exists_cmd(command_prefix + ".EXISTS"),
          _loadchunk_cmd(command_prefix + ".LOADCHUNK"),
          _scandump_cmd(command_prefix + ".SCANDUMP")
        {}

private:
    const std::string _add_cmd;
    const std::string _exists_cmd;
    const std::string _loadchunk_cmd;
    const std::string _scandump_cmd;
};

} // namespace

#endif
