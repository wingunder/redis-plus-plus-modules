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

#ifndef REDIS_MODULE_TEST_CUCKOOFILTER_COMMAND_H
#define REDIS_MODULE_TEST_CUCKOOFILTER_COMMAND_H

#include <RedisBloom/CuckooFilter.h>
#include "test_redisbloom_commands.h"

namespace redis::module::test {

    template <typename RedisInstance>
    class CuckooFilterCommand : public RedisBloomCommand<RedisInstance>
    {
    public:
        CuckooFilterCommand(RedisInstance &redis)
            : RedisBloomCommand<RedisInstance>(redis), _bloom(redis) {}

        void run(const std::string &key);

    protected:
        virtual CuckooFilter<RedisInstance>& redisInstance() { return _bloom; }

    private:
        void test_commands(const std::string &key);

        void insertVerify(const sw::redis::StringView &key,
                          long long capacity,
                          bool nocreate);

        void insertnxVerify(const sw::redis::StringView &key,
                            long long capacity,
                            bool nocreate);

        redis::module::CuckooFilter<RedisInstance> _bloom;
    };

}

#include "test_cuckoofilter_commands.tpp"

#endif
