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

#ifndef REDIS_MODULE_TEST_BLOOMFILTER_COMMAND_H
#define REDIS_MODULE_TEST_BLOOMFILTER_COMMAND_H

#include "../src/bloomfilter.h"

namespace redis::module::test {

    template <typename RedisInstance>
    class BloomFilterCommand
    {
    public:
        BloomFilterCommand(const sw::redis::ConnectionOptions &opts)
            : _opts(opts) {}

        void run();

    private:
        void test_commands(redis::module::BloomFilter<RedisInstance> &bloom);

        void insertVerify(redis::module::BloomFilter<RedisInstance>& bloomfilter,
                          const sw::redis::StringView &key,
                          double error_rate,
                          long long capacity,
                          bool nonscaling,
                          bool nocreate,
                          int expansion);
        const sw::redis::ConnectionOptions _opts;
    };

}

#include "test_bloomfilter_commands.tpp"

#endif
