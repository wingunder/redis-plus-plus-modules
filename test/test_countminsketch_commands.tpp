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

#ifndef REDIS_MODULE_TEST_COUNT_MIN_SCETCH_COMMAND_TPP
#define REDIS_MODULE_TEST_COUNT_MIN_SCETCH_COMMAND_TPP

#include <sw/redis++/redis++.h>
#include "../redis-plus-plus/test/src/sw/redis++/utils.h"

namespace redis::module::test {
    template <typename RedisInstance>
    void CountMinSketchCommand<RedisInstance>::run(const std::string &key) {
        test_commands(key);
    }

    template <typename RedisInstance>
    void CountMinSketchCommand<RedisInstance>::test_commands(const std::string &key) {

        BloomBaseCommand<RedisInstance>::_redis.del(key);

        _bloom.initbydim(key, 2, 5);
        BloomBaseCommand<RedisInstance>::_redis.del(key);

        try {
            _bloom.initbyprob(key, 30, 0.01);
            REDIS_ASSERT(0, "cms_initbyprob 'error' range check failed");
        }
        catch (...) {
            // We expect this, as 'error' is out of range.
        }

        try {
            _bloom.initbyprob(key, 0.001, 5);
            REDIS_ASSERT(0, "cms_initbyprob 'probability' range check failed");
        }
        catch (...) {
            // We expect this, as 'error' is out of range.
        }

        _bloom.initbyprob(key, 0.001, 0.01);

        std::unordered_map<std::string, long long> items = {{"a", 5}, {"b", 3}, {"c", 1}};
        std::vector<std::string> input;
        std::for_each(items.begin(), items.end(), [&input](auto& k){ input.push_back(k.first); });

        for (int j=0; j<2; j++) {
            std::vector<long long> incresult;
            _bloom.template incrby(key, items.begin(), items.end(), std::back_inserter(incresult));

            std::vector<long long> result;
            _bloom.template query(key, input.begin(), input.end(), std::back_inserter(result));
            REDIS_ASSERT(items.size() == result.size(), "cms_query returned wrong size");
            for (int i=0; i<input.size(); i++) {
                REDIS_ASSERT( (j+1) *items.at(input.at(i)) == result.at(i), "cms_query failed");
            }
        }

        std::vector<std::string> keys = { key + "_1", key + "_2" };
        for (auto k: keys) {
            BloomBaseCommand<RedisInstance>::_redis.del(k);
        }

        { // test merge
            BloomBaseCommand<RedisInstance>::_redis.del(key);
            _bloom.initbydim(key, 2, 5);

            std::unordered_map<std::string, long long> weighted_keys;
            for (int i=0; i<keys.size(); i++) {
                _bloom.initbydim(keys.at(i), 2, 5);
                _bloom.template incrby(keys.at(i), items.begin(), items.end());
                weighted_keys.insert(std::pair<std::string, long long>(keys.at(i), (i+1)*2));
            }

            _bloom.template merge(key, weighted_keys.begin(), weighted_keys.end());
            std::vector<long long> result;
            _bloom.template query(key, input.begin(), input.end(), std::back_inserter(result));
            for (int i=0; i<input.size(); i++) {
                REDIS_ASSERT(result.at(i) == 6 * items.at(input.at(i)),
                             "cms_merge failed.");
            }
        }
        { // test merge_default_weight
            BloomBaseCommand<RedisInstance>::_redis.del(key);
            _bloom.initbydim(key, 2, 5);

            _bloom.template merge_default_weight(key, keys.begin(), keys.end());
            std::vector<long long> result;
            _bloom.template query(key, input.begin(), input.end(), std::back_inserter(result));
            for (int i=0; i<input.size(); i++) {
                REDIS_ASSERT(result.at(i) == keys.size() * items.at(input.at(i)),
                             "cms_merge failed.");
            }

            for (auto k: keys) {
                BloomBaseCommand<RedisInstance>::_redis.del(k);
            }
        }

        using Info = std::unordered_map<std::string, sw::redis::OptionalLongLong>;
        {
            Info output;
            _bloom.info(key, output);
            REDIS_ASSERT(!output.empty() &&
                         *(output.at("depth")) > 0 &&
                         *(output.at("count")) > 0 &&
                         *(output.at("width")) > 0,
                         "cms_info failed");
        }
        BloomBaseCommand<RedisInstance>::_redis.del(key);
    }

} // namespace

#endif
