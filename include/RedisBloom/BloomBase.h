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

#ifndef REDIS_PLUS_PLUS_BLOOM_BASE_H
#define REDIS_PLUS_PLUS_BLOOM_BASE_H

#include <sw/redis++/redis++.h>
#include <algorithm>
#include <iterator>

namespace redis::module {

template <typename RedisInstance>
class BloomBase
{
public:
    long long add(const sw::redis::StringView &key,
                  const sw::redis::StringView &item) {
        return _redis.template command<long long>(_add_cmd,  key, item);
    }

    long long exists(const sw::redis::StringView &key,
                     const sw::redis::StringView &item) {
        return _redis.template command<long long>(_exists_cmd,  key, item);
    }

    template <typename Output>
    void info(const sw::redis::StringView &key, Output &output) {
        _redis.command(_info_cmd, key, std::inserter(output, output.end()));
    }

protected:
    explicit BloomBase(RedisInstance& redis, const std::string& command_prefix)
        : _redis(redis),
          _add_cmd(command_prefix + ".ADD"),
          _exists_cmd(command_prefix + ".EXISTS"),
          _info_cmd(command_prefix + ".INFO")
        {}

    BloomBase(const BloomBase &) = delete;
    BloomBase& operator=(const BloomBase &) = delete;
    BloomBase(BloomBase &&) = default;
    BloomBase& operator=(BloomBase &&) = default;

    RedisInstance& _redis;

private:
    const std::string _add_cmd;
    const std::string _exists_cmd;
    const std::string _info_cmd;
};

} // namespace

#endif
