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

#ifndef REDIS_MODULE_TEST_BLOOMFILTER_COMMAND_TPP
#define REDIS_MODULE_TEST_BLOOMFILTER_COMMAND_TPP

#include <sw/redis++/redis++.h>
#include "../redis-plus-plus/test/src/sw/redis++/utils.h"

namespace redis::module::test {
    template <typename RedisInstance>
    void BloomFilterCommand<RedisInstance>::run(const std::string &key) {
        test_commands(key);
        BloomBaseCommand<RedisInstance>::test_chunks(key);
        BloomBaseCommand<RedisInstance>::_redis.del(key);
    }

    template <typename RedisInstance>
    void BloomFilterCommand<RedisInstance>::test_commands(const std::string &key) {

        const long long capacity = 5000;
        const long long expansion_rate = 3;
        BloomBaseCommand<RedisInstance>::_redis.del(key);

        try {
            // Try to reserve with the wrong error_rate.
            // The error_rate must be > 0 and < 1.
            _bloom.reserve(key, 100, capacity, false);
            REDIS_ASSERT(0, "bf_reserve failed as it should have thrown");
        }
        catch (const sw::redis::Error &e) {
            // We expect to reach this point.
        }
        catch (...) {
            throw;
        }

        _bloom.reserve(key, 0.1, capacity, false);
        BloomBaseCommand<RedisInstance>::_redis.del(key);

        _bloom.reserve(key, 0.1, capacity, false, expansion_rate);
        BloomBaseCommand<RedisInstance>::_redis.del(key);

        _bloom.reserve(key, 0.1, capacity, true);
        BloomBaseCommand<RedisInstance>::_redis.del(key);

        _bloom.reserve(key, 0.1, capacity, true, expansion_rate);

        auto cnt = _bloom.add(key, "foo");
        REDIS_ASSERT(cnt == 1, "bf_add failed");

        auto exists = _bloom.exists(key, "foo");
        REDIS_ASSERT(exists, "bf_exists failed");

        {
            std::vector<std::string> input = { "foo", "bar", "zzz" };
            std::vector<long long> output;
            _bloom.template madd(key, input.begin(), input.end(), output);
            REDIS_ASSERT(output.size() == 3 &&
                         output.at(0) == 0 &&
                         output.at(1) == 1 &&
                         output.at(2) == 1,
                         "bf_madd failed");
            output.clear();

            _bloom.template mexists(key, input.begin(), input.end(), output);
            REDIS_ASSERT(output.size() == 3 &&
                         output.at(0) == 1 &&
                         output.at(1) == 1 &&
                         output.at(2) == 1,
                         "bf_mexists failed");
        }

        using Info = std::unordered_map<std::string, sw::redis::OptionalLongLong>;
        {
            Info output;
            _bloom.info(key, output);
            REDIS_ASSERT(!output.empty() &&
                         *(output.at("Expansion rate")) == expansion_rate &&
                         *(output.at("Number of items inserted")) == 3 &&
                         //*(output.at("Size")) == 4056 && // voodoo!!!
                         *(output.at("Number of filters")) == 1 &&
                         *(output.at("Capacity")) == capacity,
                         "bf_info failed");
        }
        BloomBaseCommand<RedisInstance>::_redis.del(key);

        insertVerify(key,    0.1, capacity +  0, false, false, 4);
        insertVerify(key,   0.01, capacity + 10, false,  true, 5);
        BloomBaseCommand<RedisInstance>::_redis.del(key);
        insertVerify(key,  0.001, capacity + 20,  true, false, 6);
        try {
            insertVerify(key, 0.0001, capacity + 30,  true,  true, 7);
            REDIS_ASSERT(0, "bf_insert failed to throw exception.");
        }
        catch (sw::redis::Error& e) {
            // We're expecting this.
        }
    }

    template <typename RedisInstance>
    void BloomFilterCommand<RedisInstance>::insertVerify(const sw::redis::StringView &key,
                                                         double error_rate,
                                                         long long capacity,
                                                         bool nonscaling,
                                                         bool nocreate,
                                                         int expansion) {
        std::vector<std::string> input = { "foo", "bar", "zzz" };
        {
            std::vector<long long> output;
            _bloom.template insert(key, error_rate, capacity, nonscaling, nocreate, input.begin(), input.end(), output, expansion);
            REDIS_ASSERT(output.size() == 3, "bf_insert failed");
        }
        std::unordered_map<std::string, sw::redis::OptionalLongLong> output;
        _bloom.info(key, output);
        REDIS_ASSERT(!output.empty() &&
                     (nonscaling || nocreate || (long long)(*output.at("Expansion rate")) ==  expansion) &&
                     (long long)(*output.at("Number of items inserted")) == input.size() &&
                     (long long)(*output.at("Number of filters")) == 1 &&
                     (nocreate || (long long)(*output.at("Capacity")) == capacity),
                     "bf_insert's verification failed");
    }

} // namespace

#endif
