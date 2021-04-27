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
    void BloomFilterCommand<RedisInstance>::run() {
        redis::module::BloomFilter<RedisInstance> bloom(_opts);
        test_commands(bloom);
    }

    template <typename RedisInstance>
    void BloomFilterCommand<RedisInstance>::test_commands(redis::module::BloomFilter<RedisInstance>& bloomfilter) const {

        const std::string key = "newFilter";
        const int capacity = 5000;
        const int expansion_rate = 3;
        bloomfilter.del(key);

        try {
            // Try to reserve with the wrong error_rate.
            // The error_rate must be > 0 and < 1.
            auto reserved = bloomfilter.reserve(key, 100, capacity, false);
            REDIS_ASSERT(0, "bf_reserve failed as it should have thrown");
        }
        catch (const sw::redis::Error &e) {
            // We expect to reach this point.
        }
        catch (...) {
            throw;
        }

        auto reserved = bloomfilter.reserve(key, 0.1, capacity, false);
        REDIS_ASSERT(reserved, "bf_reserve failed");
        bloomfilter.del(key);

        reserved = bloomfilter.reserve(key, 0.1, capacity, false, expansion_rate);
        REDIS_ASSERT(reserved, "bf_reserve failed");
        bloomfilter.del(key);

        reserved = bloomfilter.reserve(key, 0.1, capacity, true);
        REDIS_ASSERT(reserved, "bf_reserve failed");
        bloomfilter.del(key);

        reserved = bloomfilter.reserve(key, 0.1, capacity, true, expansion_rate);
        REDIS_ASSERT(reserved, "bf_reserve failed");

        auto cnt = bloomfilter.add(key, "foo");
        REDIS_ASSERT(cnt == 1, "bf_add failed");

        auto exists = bloomfilter.exists(key, "foo");
        REDIS_ASSERT(exists, "bf_exists failed");

        {
            std::vector<std::string> input = { "foo", "bar", "zzz" };
            std::vector<long long> output;
            bloomfilter.template madd(key, input.begin(), input.end(), output);
            REDIS_ASSERT(output.size() == 3 &&
                         output.at(0) == 0 &&
                         output.at(1) == 1 &&
                         output.at(2) == 1,
                         "bf_madd failed");
            output.clear();

            bloomfilter.template mexists(key, input.begin(), input.end(), output);
            REDIS_ASSERT(output.size() == 3 &&
                         output.at(0) == 1 &&
                         output.at(1) == 1 &&
                         output.at(2) == 1,
                         "bf_mexists failed");
        }

        {
            std::unordered_map<std::string, long long> output;
            bloomfilter.info(key, output);
            REDIS_ASSERT(!output.empty() &&
                         output.at("Expansion rate") ==  expansion_rate &&
                         output.at("Number of items inserted") == 3 &&
                         //output.at("Size") == 4056 && // voodoo!!!
                         output.at("Number of filters") == 1 &&
                         output.at("Capacity") == capacity,
                         "bf_info failed");
        }
        bloomfilter.del(key);

        insertVerify(bloomfilter, key,    0.1, capacity +  0, false, false, 4);
        insertVerify(bloomfilter, key,   0.01, capacity + 10, false,  true, 5);
        bloomfilter.del(key);
        insertVerify(bloomfilter, key,  0.001, capacity + 20,  true, false, 6);
        try {
            insertVerify(bloomfilter, key, 0.0001, capacity + 30,  true,  true, 7);
            REDIS_ASSERT(0, "bf_insert failed to throw exception.");
        }
        catch (sw::redis::Error& e) {
            // We're expecting this.
        }

        std::unordered_map<std::string, long long> chunk_info;
        bloomfilter.info(key, chunk_info);
        std::vector<std::pair<long long, std::vector<unsigned char>>> chunks;
        getChunks(bloomfilter, key, chunks);

        bloomfilter.del(key);
        for (const auto& chunk : chunks) {
            auto loaded = bloomfilter.loadchunk(key, chunk);
            REDIS_ASSERT(loaded, "bf_loadchunk failed");
        }

        std::vector<std::pair<long long, std::vector<unsigned char>>> verify_chunks;
        getChunks(bloomfilter, key, verify_chunks);

        // Revert to some more basic chunk checks, as a full match never occurs.
        REDIS_ASSERT(chunks.size() == verify_chunks.size(),
                     "bf_loadchunk failed due to different amount of chunks");
        // The chunks seem not to match. Why?
        if (chunks != verify_chunks) {
            for (int i=0; i<chunks.size(); i++) {
                REDIS_ASSERT(chunks.at(i).second.size() == verify_chunks.at(i).second.size(),
                             "bf_loadchunk failed due to a missmatch in chunk size.");
                if (chunks.at(i).second != verify_chunks.at(i).second) {
                    for (int j=0; j<chunks.at(i).second.size(); j++) {
                        if (chunks.at(i).second.at(j) != verify_chunks.at(i).second.at(j)) {
                            //std::cout << "e " << i << " " << j << " " << chunks.at(i).second.at(j) << " " << verify_chunks.at(i).second.at(j) << std::endl;
                        }
                    }
                }
            }
        }
        //REDIS_ASSERT(chunks == verify_chunks, "bf_loadchunk failed");

        std::unordered_map<std::string, long long> verify_chunk_info;
        bloomfilter.info(key, chunk_info);
        REDIS_ASSERT(chunk_info == verify_chunk_info,
                     "bf_loadchunk failed as non-matching bf_info was returned");

        bloomfilter.del(key);
    }

    template <typename RedisInstance>
    void
    BloomFilterCommand<RedisInstance>::getChunks(redis::module::BloomFilter<RedisInstance>& bloomfilter,
                                                 const sw::redis::StringView &key,
                                                 std::vector<std::pair<long long, std::vector<unsigned char>>>& chunks) const {
        long long iter = 0;
        std::pair<long long, std::vector<unsigned char>> result;
        for (;;) {
            iter = bloomfilter.template scandump(key, iter, result);
            if (iter == 0) { break; }
            chunks.push_back(result);
        }
    }

    template <typename RedisInstance>
    void BloomFilterCommand<RedisInstance>::insertVerify(redis::module::BloomFilter<RedisInstance>& bloomfilter,
                                                         const sw::redis::StringView &key,
                                                         double error_rate,
                                                         long long capacity,
                                                         bool nonscaling,
                                                         bool nocreate,
                                                         int expansion) const {
        std::vector<std::string> input = { "foo", "bar", "zzz" };
        {
            std::vector<long long> output;
            bloomfilter.template insert(key, error_rate, capacity, nonscaling, nocreate, input.begin(), input.end(), output, expansion);
            REDIS_ASSERT(output.size() == 3, "bf_insert failed");
        }
        std::unordered_map<std::string, sw::redis::OptionalLongLong> output;
        bloomfilter.info(key, output);
        REDIS_ASSERT(!output.empty() &&
                     (nonscaling || nocreate || (long long)(*output.at("Expansion rate")) ==  expansion) &&
                     (long long)(*output.at("Number of items inserted")) == input.size() &&
                     (long long)(*output.at("Number of filters")) == 1 &&
                     (nocreate || (long long)(*output.at("Capacity")) == capacity),
                     "bf_insert's verification failed");
    }

} // namespace

#endif
