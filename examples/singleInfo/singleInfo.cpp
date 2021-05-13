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

#include <iostream>
#include <unordered_map>
#include <redismods++/BloomFilter.h>

template <typename RedisInstance>
void
getInfo(const std::string &key,
        redis::module::BloomFilter<RedisInstance> &bloomFilter,
        RedisInstance &redis)
{
    // Make sure the key does not exist.
    // Note: The BloomFilter object does not have a 'del' call,
    //       so we need to call the native redis 'DEL' command
    //       via the underlying redis-plus-plus lib.
    redis.del(key);

    // Create a BloomFilter.
    bloomFilter.reserve(key, 0.1, 100, false);

    // Prepare the output buffer.
    std::unordered_map<std::string, sw::redis::OptionalLongLong> output;
    bloomFilter.info(key, output);

    // Print the result.
    for (auto &item : output) {
        std::cout << item.first << ": ";
        if (item.second) {
            std::cout << *item.second;
        }
        else {
            std::cout << "nil";
        }
        std::cout << std::endl;
    }

    // Clean up.
    redis.del(key);
}

int main() {

    // Setup the connection options.
    sw::redis::ConnectionOptions opts;
    opts.host = "127.0.0.1";
    opts.port = 6379;
    opts.password = "";

    // Choose the instance type.
    using RedisInstance = sw::redis::Redis;
    // use RedisInstance = sw::redis::RedisCluster;

    // Create a redis-plus-plus object.
    RedisInstance redis(opts);

    // Create a BloomFilter object.
    redis::module::BloomFilter<RedisInstance> bloomFilter(redis);

    getInfo<RedisInstance>("some_test_key", bloomFilter, redis);

    return 0;
}
