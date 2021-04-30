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

#ifndef REDIS_PLUS_PLUS_MODULE_H
#define REDIS_PLUS_PLUS_MODULE_H

#include <sw/redis++/redis++.h>
#include <algorithm>
#include <iterator>

namespace redis::module {

template <typename RedisInstance>
class Module
{
public:
    template <typename Output>
    void info(const sw::redis::StringView &key, Output &output) {
        _redis.command(_info_cmd, key, std::inserter(output, output.end()));
    }

    std::string getModuleName() const { return _module_name; }

    sw::redis::OptionalLongLong version() {
        auto reply = _redis.command("MODULE", "LIST");
        if (!sw::redis::reply::is_array(*reply)) {
            throw sw::redis::ProtoError("Expect ARRAY reply");
        }
        for (size_t i=0; i<reply->elements; i++) {
            auto *sub_reply = reply->element[i];
            std::string name;
            sw::redis::OptionalLongLong version;
            for (size_t j=0; j<sub_reply->elements-1; j+=2) {
                auto key = sw::redis::reply::parse<std::string>(*sub_reply->element[j]);
                if (key == "name") {
                    name = sw::redis::reply::parse<std::string>(*sub_reply->element[j+1]);
                }
                else if (key == "ver") {
                    version = sw::redis::reply::parse<sw::redis::OptionalLongLong>(*sub_reply->element[j+1]);
                }
            }
            if (name == _module_name) {
                return version;
            }
        }
        return sw::redis::OptionalLongLong({});
    }

protected:
    explicit Module(RedisInstance& redis,
                         const std::string& name,
                         const std::string& command_prefix)
        : _redis(redis),
          _module_name(name),
          _info_cmd(command_prefix + ".INFO")
        {}

    Module(const Module &) = delete;
    Module& operator=(const Module &) = delete;
    Module(Module &&) = default;
    Module& operator=(Module &&) = default;

    RedisInstance &_redis;

private:
    const std::string _module_name;
    const std::string _info_cmd;
};

} // namespace

#endif
