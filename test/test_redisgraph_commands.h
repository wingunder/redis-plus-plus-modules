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

#ifndef REDIS_MODULE_TEST_REDIS_GRAPH_COMMAND_H
#define REDIS_MODULE_TEST_REDIS_GRAPH_COMMAND_H

#include <redismods++/RedisGraph.h>

namespace redis::module::test {

    template <typename RedisInstance>
    class RedisGraphCommand
    {
    public:
        RedisGraphCommand(RedisInstance &redis)
            : _redis(redis), _graph(redis) {}

        void run(const std::string &key);

        sw::redis::OptionalLongLong version() {
            return _graph.version();
        }

    private:
        RedisInstance &_redis;
        redis::module::RedisGraph<RedisInstance> _graph;
    };

}

#include "test_redisgraph_commands.tpp"

#endif
