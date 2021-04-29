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

#ifndef REDIS_MODULE_TEST_COUNT_MIN_SCETCH_COMMAND_H
#define REDIS_MODULE_TEST_COUNT_MIN_SCETCH_COMMAND_H

#include <RedisBloom/CountMinSketch.h>
#include "test_bloombase_commands.h"

namespace redis::module::test {

    template <typename RedisInstance>
    class CountMinSketchCommand : public BloomBaseCommand<RedisInstance>
    {
    public:
        CountMinSketchCommand(RedisInstance &redis)
            : BloomBaseCommand<RedisInstance>(redis), _bloom(redis) {}

        void run(const std::string &key);

    protected:
        CountMinSketch<RedisInstance>& redisInstance() { return _bloom; }

    private:
        void test_commands(const std::string &key);

        redis::module::CountMinSketch<RedisInstance> _bloom;
    };

}

#include "test_countminsketch_commands.tpp"

#endif
