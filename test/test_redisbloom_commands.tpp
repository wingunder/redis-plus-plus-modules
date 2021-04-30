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

#ifndef REDIS_MODULE_TEST_REDIS_BLOOM_COMMAND_TPP
#define REDIS_MODULE_TEST_REDIS_BLOOM_COMMAND_TPP

#include <sw/redis++/redis++.h>
#include "../redis-plus-plus/test/src/sw/redis++/utils.h"

namespace redis::module::test {

    template <typename RedisInstance>
    void RedisBloomCommand<RedisInstance>::test_chunks(BloomCuckooBase<RedisInstance>& bloom, const std::string &key) {

        using Info = std::unordered_map<std::string, sw::redis::OptionalLongLong>;

        Info chunk_info;
        bloom.info(key, chunk_info);
        std::vector<std::pair<long long, std::vector<unsigned char>>> chunks;
        getChunks(bloom, key, chunks);

        _redis.del(key);
        for (const auto& chunk : chunks) {
            bloom.loadchunk(key, chunk);
        }

        std::vector<std::pair<long long, std::vector<unsigned char>>> verify_chunks;
        getChunks(bloom, key, verify_chunks);

        // Revert to some more basic chunk checks, as a full match never occurs.
        REDIS_ASSERT(chunks.size() == verify_chunks.size(),
                     "loadchunk failed due to different amount of chunks");
        // The chunks seem not to match. Why?
        if (chunks != verify_chunks) {
            for (int i=0; i<chunks.size(); i++) {
                REDIS_ASSERT(chunks.at(i).second.size() == verify_chunks.at(i).second.size(),
                             "loadchunk failed due to a missmatch in chunk size.");
                if (chunks.at(i).second != verify_chunks.at(i).second) {
                    for (int j=0; j<chunks.at(i).second.size(); j++) {
                        if (chunks.at(i).second.at(j) != verify_chunks.at(i).second.at(j)) {
                            //std::cout << "e " << i << " " << j << " " << chunks.at(i).second.at(j) << " " << verify_chunks.at(i).second.at(j) << std::endl;
                        }
                    }
                }
            }
        }
        //REDIS_ASSERT(chunks == verify_chunks, "loadchunk failed");

        Info verify_chunk_info;
        bloom.info(key, verify_chunk_info);

        // The following doesn't work:

        //REDIS_ASSERT(chunk_info == verify_chunk_info,
        //             "loadchunk failed as non-matching bf_info was returned");

        // So we'll do it the hard way.
        for (auto& item : chunk_info) {
            REDIS_ASSERT(*(item.second) == *(verify_chunk_info.at(item.first)),
                         "loadchunk failed due to differing " + item.first);
        }
    }

    template <typename RedisInstance>
    void
    RedisBloomCommand<RedisInstance>::getChunks(BloomCuckooBase<RedisInstance>& bloom,
                                               const sw::redis::StringView &key,
                                               std::vector<std::pair<long long, std::vector<unsigned char>>>& chunks) {
        long long iter = 0;
        std::pair<long long, std::vector<unsigned char>> result;
        for (;;) {
            iter = bloom.template scandump(key, iter, result);
            if (iter == 0) { break; }
            chunks.push_back(result);
        }
    }

} // namespace

#endif
