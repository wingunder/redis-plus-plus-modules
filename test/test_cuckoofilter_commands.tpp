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

        auto cnt = _bloom.add(key, "foo");
        REDIS_ASSERT(cnt == 1, "cf_add failed");
        RedisBloomCommand<RedisInstance>::test_chunks(_bloom, key);
    }

    template <typename RedisInstance>
    void CuckooFilterCommand<RedisInstance>::test_commands(const std::string &key) {
        {
            auto cnt = _bloom.add(key, "foo");
            REDIS_ASSERT(cnt == 1, "cf_add failed");

            auto exists = _bloom.exists(key, "foo");
            REDIS_ASSERT(exists, "cf_exists failed");

            auto deleted = _bloom.del(key, "foo");
            REDIS_ASSERT(deleted, "cf_del failed");

            exists = _bloom.exists(key, "foo");
            REDIS_ASSERT(!exists, "cf_del failed as it still exists");
        }

        {
            auto cnt = _bloom.addnx(key, "foo");
            REDIS_ASSERT(cnt == 1, "cf_addnx failed");

            auto exists = _bloom.exists(key, "foo");
            REDIS_ASSERT(exists, "cf_exists failed");

            auto deleted = _bloom.del(key, "foo");
            REDIS_ASSERT(deleted, "cf_del failed");
        }
        {
            insertVerify(key, 500, false);
            insertVerify(key, 500, true);
        }
        {
            insertnxVerify(key, 500, false);
            insertnxVerify(key, 500, true);
        }
    }

    template <typename RedisInstance>
    void CuckooFilterCommand<RedisInstance>::insertVerify(const sw::redis::StringView &key,
                                                          long long capacity,
                                                          bool nocreate) {
        if (nocreate) {
            _bloom.reserve(key, capacity, 3);
        }
        std::vector<std::string> input = { "foo", "bar", "zzz" };
        {
            std::vector<long long> output;
            _bloom.template insert(key, capacity, nocreate, input.begin(), input.end(), output);
            REDIS_ASSERT(output.size() == 3, "cf_insert failed");
        }
        std::unordered_map<std::string, sw::redis::OptionalLongLong> output;
        _bloom.info(key, output);
        REDIS_ASSERT(!output.empty() &&
                     (long long)(*output.at("Number of items inserted")) == input.size() &&
                     (long long)(*output.at("Number of filters")) == 1,
                     "cf_insert's verification failed");
        RedisBloomCommand<RedisInstance>::_redis.del(key);
    }

    template <typename RedisInstance>
    void CuckooFilterCommand<RedisInstance>::insertnxVerify(const sw::redis::StringView &key,
                                                            long long capacity,
                                                            bool nocreate) {
        if (nocreate) {
            _bloom.reserve(key, capacity, 3);
        }
        std::vector<std::string> input = { "foo", "bar", "zzz" };
        {
            std::vector<long long> output;
            _bloom.template insertnx(key, capacity, nocreate, input.begin(), input.end(), output);
            REDIS_ASSERT(output.size() == 3, "cf_insert failed");
        }
        std::unordered_map<std::string, sw::redis::OptionalLongLong> output;
        _bloom.info(key, output);
        REDIS_ASSERT(!output.empty() &&
                     (long long)(*output.at("Number of items inserted")) == input.size() &&
                     (long long)(*output.at("Number of filters")) == 1,
                     "cf_insert's verification failed");
        RedisBloomCommand<RedisInstance>::_redis.del(key);
    }

} // namespace

#endif
