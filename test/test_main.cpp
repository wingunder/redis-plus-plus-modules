/**************************************************************************
   Copyright (c) 2017 sewenew
   Modifications Copyright (c) 2021 Pieter du Preez <pdupreez@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#include <unistd.h>
#include <string>
#include <chrono>
#include <tuple>
#include <iostream>
#include <sw/redis++/redis++.h>
#include "test_bloomfilter_commands.h"
#include "test_cuckoofilter_commands.h"
#include "test_countminsketch_commands.h"
#include "test_topk_commands.h"
#include "test_redisjson_commands.h"
#include "test_redisgraph_commands.h"

auto parse_options(int argc, char **argv)
    -> std::tuple<sw::redis::Optional<sw::redis::ConnectionOptions>, bool>;

template <typename RedisInstance>
void run_test(const sw::redis::ConnectionOptions &opts);

int main(int argc, char **argv) {
    try {
        sw::redis::Optional<sw::redis::ConnectionOptions> opts;
        bool use_cluster;
        std::tie(opts, use_cluster) = parse_options(argc, argv);

        if (use_cluster) {
            std::cout << "Testing RedisCluster..." << std::endl;
            run_test<sw::redis::RedisCluster>(*opts);
        }
        else {
            std::cout << "Testing Redis..." << std::endl;
            run_test<sw::redis::Redis>(*opts);
        }

        std::cout << "Passed all tests" << std::endl;
    } catch (const sw::redis::Error &e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

void print_help() {
    std::cerr << "Usage: test_redisbloom++ [-c] [-h host] [-p port] [-a auth]" << std::endl;
    std::cerr << "Defaults: non-cluster, host=127.0.0.1, port=6379, no-auth" << std::endl;
    std::cerr << "eg: For a cluster test on 127.0.0.1:6379" << std::endl;
    std::cerr << "test_redisbloom++ -c" << std::endl;
    std::cerr << "eg: For a non-cluster test on 127.0.0.1:6379" << std::endl;
    std::cerr << "test_redisbloom++" << std::endl;
    std::cerr << "eg: For a non-cluster test on 127.0.0.2:5001" << std::endl;
    std::cerr << "test_redisbloom++ -h 127.0.0.2 -p 5001" << std::endl;
}

auto parse_options(int argc, char **argv)
    -> std::tuple<sw::redis::Optional<sw::redis::ConnectionOptions>, bool> {
    std::string host = "127.0.0.1";
    int port = 6379;
    std::string auth;
    bool use_cluster = false;

    int opt = 0;
    while ((opt = getopt(argc, argv, "ch:p:a:")) != -1) {
        switch (opt) {
        case 'c':
            use_cluster = true;
            break;
        case 'h':
            host = optarg;
            break;
        case 'p':
            try {
                port = std::stoi(optarg);
            }
            catch (...) {
                print_help();
                throw sw::redis::Error("Invalid port number");
            }
            break;
        case 'a':
            auth = optarg;
            break;
        default:
            print_help();
            throw sw::redis::Error("Unknow command line option");
            break;
        }
    }

    sw::redis::ConnectionOptions opts;
    if (!host.empty()) {
        opts.host = host;
    }
    if (port >= 0) {
        opts.port = port;
    }
    opts.password = auth;

    return std::make_tuple(std::move(sw::redis::Optional<sw::redis::ConnectionOptions>(opts)), std::move(use_cluster));
}

template <typename RedisInstance>
void test_redisbloom(RedisInstance &redis) {

    redis::module::test::BloomFilterCommand<RedisInstance> bf_cmd(redis);
    const auto moduleName = bf_cmd.getModuleName();
    sw::redis::OptionalLongLong ver = bf_cmd.version();
    if (!ver) {
        std::cout << "Skipped RedisBloom tests, as '" << moduleName << "' module was not loaded." << std::endl;
        return;
    }
    std::cout << "Testing RedisBloom ('" << moduleName << "') module version " << *ver << std::endl;
    const std::string key = "newFilter";
    {
        redis::module::test::BloomFilterCommand<RedisInstance> bf_cmd(redis);
        bf_cmd.run(key);
        std::cout << "Passed BloomFilter tests" << std::endl;
    }
    {
        redis::module::test::CuckooFilterCommand<RedisInstance> cf_cmd(redis);
        cf_cmd.run(key);
        std::cout << "Passed CuckooFilter tests" << std::endl;
    }
    {
        redis::module::test::CountMinSketchCommand<RedisInstance> cms_cmd(redis);
        cms_cmd.run(key);
        std::cout << "Passed CountMinSketch tests" << std::endl;
    }
    {
        redis::module::test::TopKCommand<RedisInstance> topk_cmd(redis);
        topk_cmd.run(key);
        std::cout << "Passed TopK tests" << std::endl;
    }

    std::cout << "Passed RedisBloom tests" << std::endl;
}

template <typename RedisInstance>
void test_redisjson(RedisInstance &redis) {

    redis::module::test::RedisJSONCommand<RedisInstance> json_cmd(redis);

    json_cmd.run("json_test_key");

    std::cout << "Passed RedisJSON tests" << std::endl;
}

template <typename RedisInstance>
void test_redisgraph(RedisInstance &redis) {

    redis::module::test::RedisGraphCommand<RedisInstance> graph_cmd(redis);

    graph_cmd.run("graph_test_key");

    std::cout << "Passed RedisGraph tests" << std::endl;
}

template <typename RedisInstance>
void run_test(const sw::redis::ConnectionOptions &opts) {

    RedisInstance redis(opts);
    test_redisbloom(redis);
    test_redisjson(redis);
    test_redisgraph(redis);
}
