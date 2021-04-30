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

#ifndef REDIS_MODULE_TEST_TOP_K_COMMAND_TPP
#define REDIS_MODULE_TEST_TOP_K_COMMAND_TPP

#include <sw/redis++/redis++.h>
#include "../redis-plus-plus/test/src/sw/redis++/utils.h"

namespace redis::module::test {
    template <typename RedisInstance>
    void TopKCommand<RedisInstance>::run(const std::string &key) {
        test_commands(key);
    }

    template <typename RedisInstance>
    void TopKCommand<RedisInstance>::test_commands(const std::string &key) {

        RedisBloomCommand<RedisInstance>::_redis.del(key);

        REDIS_ASSERT(_bloom.reserve(key, 1), "topk_reserve failed.");

        std::vector<std::string> items = {"a", "b", "c"};
        std::vector<sw::redis::OptionalString> result;
        _bloom.template add(key, items.begin(), items.end(), std::back_inserter(result));

        std::unordered_map<std::string, long long> item_pairs = {{"a", 500}, {"b", 3}, {"c", 1}};
        std::vector<std::string> input;
        std::for_each(item_pairs.begin(), item_pairs.end(), [&input](auto& k){ input.push_back(k.first); });

        for (int j=0; j<2; j++) {
            std::vector<sw::redis::OptionalString> incresult;
            _bloom.template incrby(key, item_pairs.begin(), item_pairs.end(), std::back_inserter(incresult));
            REDIS_ASSERT(items.size() == incresult.size(), "topk_incrby returned wrong size");
            std::vector<long long> result;
            _bloom.template query(key, input.begin(), input.end(), std::back_inserter(result));
            REDIS_ASSERT(items.size() == result.size(), "topk_query returned wrong size");
            std::vector<long long> cntresult;
            _bloom.template count(key, input.begin(), input.end(), std::back_inserter(cntresult));
            REDIS_ASSERT(items.size() == cntresult.size(), "topk_count returned wrong size");
            std::vector<std::string> listresult;
            _bloom.template list(sw::redis::StringView(key), std::back_inserter(listresult));
        }

        using Info = std::unordered_map<std::string, double>;
        {
            Info output;
            _bloom.info(key, output);
            REDIS_ASSERT(!output.empty() &&
                         output.find("k") != output.end() &&
                         output.find("width") != output.end() &&
                         output.find("depth") != output.end() &&
                         output.find("decay") != output.end(),
                         "cms_info failed");
        }
        RedisBloomCommand<RedisInstance>::_redis.del(key);
    }

} // namespace

#endif
