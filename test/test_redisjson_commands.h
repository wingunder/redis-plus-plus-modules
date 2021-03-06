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

#ifndef REDIS_MODULE_TEST_REDIS_JSON_COMMAND_H
#define REDIS_MODULE_TEST_REDIS_JSON_COMMAND_H

#include <redismods++/RedisJSON.h>

namespace redis::module::test {

    template <typename RedisInstance>
    class RedisJSONCommand
    {
    public:
        RedisJSONCommand(RedisInstance &redis)
            : _redis(redis), _json(redis) {}

        void run(const std::string &key);

        sw::redis::OptionalLongLong version() {
            return _json.version();
        }

    private:
        void test_del(const std::string &key);
        void test_objects(const std::string &key);
        void test_mget(const std::string &key);
        void test_get(const std::string &key);
        void test_numbers(const std::string &key);
        void test_strings(const std::string &key);
        void test_debug(const std::string &key);
        void test_arr(const std::string &key);

        RedisInstance &_redis;
        redis::module::RedisJSON<RedisInstance> _json;
    };

}

#include "test_redisjson_commands.tpp"

#endif
