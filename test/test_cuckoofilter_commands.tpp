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

#ifndef REDIS_MODULE_TEST_CUCKOOFILTER_COMMAND_TPP
#define REDIS_MODULE_TEST_CUCKOOFILTER_COMMAND_TPP

#include <sw/redis++/redis++.h>
#include "../redis-plus-plus/test/src/sw/redis++/utils.h"

namespace redis::module::test {

    template <typename RedisInstance>
    void CuckooFilterCommand<RedisInstance>::run(const std::string &key) {
        test_commands(key);
        BloomBaseCommand<RedisInstance>::test_chunks(key);
    }

    template <typename RedisInstance>
    void CuckooFilterCommand<RedisInstance>::test_commands(const std::string &key) {
        auto cnt = redisInstance().add(key, "foo");
        REDIS_ASSERT(cnt == 1, "cf_add failed");

        auto exists = redisInstance().exists(key, "foo");
        REDIS_ASSERT(exists, "cf_exists failed");

    }

} // namespace

#endif
