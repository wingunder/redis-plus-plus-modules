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

#ifndef REDIS_MODULE_TEST_REDIS_BLOOM_COMMAND_H
#define REDIS_MODULE_TEST_REDIS_BLOOM_COMMAND_H

#include <redismods++/RedisBloom.h>

namespace redis::module::test {

    template <typename RedisInstance>
    class RedisBloomCommand
    {
    public:
        std::string getModuleName() {
            return redisInstance().getModuleName();
        }

        sw::redis::OptionalLongLong version() {
            return redisInstance().version();
        }
    protected:
        RedisBloomCommand(RedisInstance &redis)
            : _redis(redis) {}

        virtual void run(const std::string &key) = 0;
        virtual RedisBloom<RedisInstance>& redisInstance() = 0;

        void test_chunks(BloomCuckooBase<RedisInstance>& bloom, const std::string &key);

        RedisInstance &_redis;

    private:
        void getChunks(BloomCuckooBase<RedisInstance>& bloom,
                       const sw::redis::StringView &key,
                       std::vector<std::pair<long long, std::vector<unsigned char>>>& chunks);

    };

}

#include "test_redisbloom_commands.tpp"

#endif
