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

#ifndef REDIS_MODULE_TEST_BLOOMBASE_COMMAND_H
#define REDIS_MODULE_TEST_BLOOMBASE_COMMAND_H

#include <RedisBloom/BloomBase.h>

namespace redis::module::test {

    template <typename RedisInstance>
    class BloomBaseCommand
    {
    protected:
        BloomBaseCommand(RedisInstance &redis)
            : _redis(redis) {}

        virtual void run(const std::string &key) = 0;
        virtual BloomBase<RedisInstance>& redisInstance() = 0;

        void test_chunks(const std::string &key);

        RedisInstance &_redis;

    private:
        void getChunks(const sw::redis::StringView &key,
                       std::vector<std::pair<long long, std::vector<unsigned char>>>& chunks);

    };

}

#include "test_bloombase_commands.tpp"

#endif