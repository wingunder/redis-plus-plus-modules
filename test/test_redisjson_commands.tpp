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

#ifndef REDIS_MODULE_TEST_REDIS_JSON_COMMAND_TPP
#define REDIS_MODULE_TEST_REDIS_JSON_COMMAND_TPP

#include <sw/redis++/redis++.h>
#include "../redis-plus-plus/test/src/sw/redis++/utils.h"

namespace redis::module::test {

    template <typename RedisInstance>
    void RedisJSONCommand<RedisInstance>::run(const std::string &key) {

        const auto moduleName = _json.getModuleName();
        sw::redis::OptionalLongLong ver = _json.version();
        if (!ver) {
            std::cout << "Skipped RedisJSON tests, as '" << moduleName << "' module was not loaded." << std::endl;
            return;
        }
        std::cout << "Testing RedisJSON ('" << moduleName << "') module version " << *ver << std::endl;

        test_del(key);
        test_objects(key);
        test_mget(key);
        test_get(key);
        test_numbers(key);
        test_strings(key);
        test_debug(key);
        test_arr(key);
    }


    template <typename RedisInstance>
    void RedisJSONCommand<RedisInstance>::test_del(const std::string &key) {

        auto del_ret = _json.del(key);
        REDIS_ASSERT(del_ret == 1 || del_ret == 0, "json_del failed");

        _json.set(key, ".", "\"x\"");

        del_ret = _json.forget(key);
        REDIS_ASSERT(del_ret == 1 || del_ret == 0, "json_del failed");

        del_ret = _json.forget(key, ".");
        REDIS_ASSERT(del_ret == 1 || del_ret == 0, "json_del failed");

    }

    template <typename RedisInstance>
    void RedisJSONCommand<RedisInstance>::test_mget(const std::string &key) {

        std::vector<std::string> inarr = {"{\"a\":1,\"b\":2,\"c\":3}", "{\"a\":3,\"b\":2,\"c\":1}"};
        _json.set(key, ".", inarr.at(0));
        const std::string key_1 = key + "_1";
        _json.set(key_1, ".", inarr.at(1));
        {
            std::vector<std::string> arr = {key, key_1};
            std::vector<std::string> result;
            _json.template mget(arr.begin(), arr.end(), ".", result);
            REDIS_ASSERT(arr.size() == result.size(), "json_mget failed");
            for (size_t i=0; i<arr.size(); i++) {
                REDIS_ASSERT(inarr.at(i) == result.at(i), "json_mget failed");
            }

            result.clear();
            _json.template mget(arr.begin(), arr.end(), result);
            REDIS_ASSERT(arr.size() == result.size(), "json_mget failed");
            for (size_t i=0; i<arr.size(); i++) {
                REDIS_ASSERT(inarr.at(i) == result.at(i), "json_mget failed");
            }
        }
        _json.del(key_1, ".");
        _json.del(key, ".");

    }

    template <typename RedisInstance>
    void RedisJSONCommand<RedisInstance>::test_get(const std::string &key) {

        _json.set(key, ".", "{\"a\":1,\"b\":2,\"c\":3}");
        std::vector<std::string> paths = {"a", "c"};
        {
            std::string result = _json.get(key, paths.begin(), paths.end());
            REDIS_ASSERT(result == "{\"a\":1,\"c\":3}", "json_get failed");
        }
        {
            std::string result = _json.get(key, paths.begin(), paths.end(), "\t");
            REDIS_ASSERT(result == "{\t\"a\":1,\t\"c\":3}", "json_get failed");
        }
        {
            std::string result = _json.get(key, paths.begin(), paths.end(), "\t", "\n");
            REDIS_ASSERT(result == "{\n\t\"a\":1,\n\t\"c\":3\n}", "json_get failed");
        }
        {
            _json.set(key, "c", "\"\\u0002\"");
            std::string result = _json.get(key, paths.begin(), paths.end(), "\t", "\n", " ");
            REDIS_ASSERT(result == "{\n\t\"a\": 1,\n\t\"c\": \"\\u0002\"\n}", "json_get failed");
        }
        {
            std::string result = _json.get(key, paths.begin(), paths.end(), "\t", "\n", " ", true);
            REDIS_ASSERT(result == "{\n\t\"a\": 1,\n\t\"c\": \"\x0002\"\n}", "json_get failed");
        }
        _json.del(key, ".");

    }

    template <typename RedisInstance>
    void RedisJSONCommand<RedisInstance>::test_objects(const std::string &key) {

        _json.set(key, ".", "{\"a\":1,\"b\":2,\"c\":3}");
        std::vector<std::string> result;
        _json.template objkeys(key, ".", result);
        REDIS_ASSERT(result.size() == 3 &&
                     result.at(0) == "a" &&
                     result.at(1) == "b" &&
                     result.at(2) == "c",
                     "json_objkeys failed");
        result.clear();
        _json.template objkeys(key, result);
        REDIS_ASSERT(result.size() == 3 &&
                     result.at(0) == "a" &&
                     result.at(1) == "b" &&
                     result.at(2) == "c",
                     "json_objkeys failed");

        auto len_ret = _json.objlen(key);
        REDIS_ASSERT(len_ret == 3, "json_objlen failed");

        len_ret = _json.objlen(key, ".");
        REDIS_ASSERT(len_ret == 3, "json_objlen failed");

        _json.del(key, ".");

    }

    template <typename RedisInstance>
    void RedisJSONCommand<RedisInstance>::test_numbers(const std::string &key) {

        _json.set(key, ".", 2);
        auto num_ret = _json.numincrby(key, ".", 2);
        REDIS_ASSERT(num_ret == 4, "json_numincrby failed");

        num_ret = _json.nummultby(key, ".", 2);
        REDIS_ASSERT(num_ret == 8, "json_nummultby failed");

        _json.del(key, ".");
    }

    template <typename RedisInstance>
    void RedisJSONCommand<RedisInstance>::test_strings(const std::string &key) {

        _json.set(key, ".", "\"bar\"");

        auto strlen_ret = _json.strlen(key);
        REDIS_ASSERT(strlen_ret == 3, "json_strlen failed");

        strlen_ret = _json.strlen(key, ".");
        REDIS_ASSERT(strlen_ret == 3, "json_strlen failed");

        strlen_ret = _json.strappend(key, ".", "\"000\"");
        REDIS_ASSERT(strlen_ret == 6, "json_append failed");

        strlen_ret = _json.strappend(key, "\"111\"");
        REDIS_ASSERT(strlen_ret == 9, "json_append failed");

        auto type_ret = _json.type(key);
        REDIS_ASSERT(type_ret == "string", "json_type failed");

        type_ret = _json.type(key, ".");
        REDIS_ASSERT(type_ret == "string", "json_type failed");

        auto resp_ret = _json.resp(key);
        REDIS_ASSERT(resp_ret == "bar000111", "json_resp failed");

        resp_ret = _json.resp(key, ".");
        REDIS_ASSERT(resp_ret == "bar000111", "json_resp failed");

        _json.del(key, ".");
    }

    template <typename RedisInstance>
    void RedisJSONCommand<RedisInstance>::test_debug(const std::string &key) {
        std::string json = "[{\"a\":1,\"b\":2,\"c\":3}, {\"a\":3,\"b\":2,\"c\":1}]";
        _json.set(key, ".", json);
        auto mem_0 = _json.debugmem(key, ".");
        REDIS_ASSERT(mem_0 && *mem_0 > 0, "json_debugmem failed");
        auto mem_1 = _json.debugmem(key, ".");
        REDIS_ASSERT(mem_1 && *mem_1 > 0, "json_debugmem failed");
        REDIS_ASSERT(*mem_0 == *mem_1, "json_debugmem failed");
        auto mem_x = _json.debugmem(key + "_does_not_exist", ".");
        REDIS_ASSERT(!mem_x, "json_debugmem failed");
    }

    template <typename RedisInstance>
    void RedisJSONCommand<RedisInstance>::test_arr(const std::string &key) {
        const std::string json_arr = "[\"a\",\"b\",\"c\",\"d\"]";
        _json.set(key, ".", json_arr);
        auto arrlen_ret = _json.arrlen(key, ".");
        REDIS_ASSERT(arrlen_ret == 4, "json_arrlen failed");

        auto arrpop_ret = _json.arrpop(key);
        REDIS_ASSERT(arrpop_ret == "\"d\"", "json_arrpop failed");

        arrpop_ret = _json.arrpop(key, ".");
        REDIS_ASSERT(arrpop_ret == "\"c\"", "json_arrpop failed");

        arrpop_ret = _json.arrpop(key, ".", 0);
        REDIS_ASSERT(arrpop_ret == "\"a\"", "json_arrpop failed");

        auto arrins_retX = _json.template arrinsert(key, ".", 0, "\"a\"");
        REDIS_ASSERT(arrins_retX == 2, "json_arrinsert failed");

        std::vector<std::string> paths = {"\"c\"", "\"d\""};
        auto arrins_ret = _json.template arrinsert(key, ".", 2, paths.begin(), paths.end());
        REDIS_ASSERT(arrins_ret == 4, "json_arrinsert failed");

        std::vector<std::string> query_paths = {"."};
        auto result = _json.get(key, query_paths.begin(), query_paths.end());
        REDIS_ASSERT(result == json_arr, "json_get failed");

        result = _json.get(key, "[0]");
        REDIS_ASSERT(result == "\"a\"", "json_get failed");

        auto arrtrim_ret = _json.template arrtrim(key, ".", 0, 1);
        REDIS_ASSERT(arrtrim_ret == 2, "json_arrtrim failed");

        arrins_ret = _json.template arrappend(key, ".", paths.begin(), paths.end());
        REDIS_ASSERT(arrins_ret == 4, "json_arrinsert failed");

        result = _json.get(key, query_paths.begin(), query_paths.end());
        REDIS_ASSERT(result == json_arr, "json_get failed");

        _json.del(key, ".");
        _json.set(key, ".", "[\"abc\", true, 25, \"abc\", null, false]");
        REDIS_ASSERT(_json.arrindex(key, ".", "\"abc\"") == 0, "json_arrindex failed");
        REDIS_ASSERT(_json.arrindex(key, ".", "true") == 1, "json_arrindex failed");
        REDIS_ASSERT(_json.arrindex(key, ".", "25") == 2, "json_arrindex failed");
        REDIS_ASSERT(_json.arrindex(key, ".", "null") == 4, "json_arrindex failed");
        REDIS_ASSERT(_json.arrindex(key, ".", "false") == 5, "json_arrindex failed");
        REDIS_ASSERT(_json.arrindex(key, ".", "\"abc\"", 1) == 3, "json_arrindex failed");
        REDIS_ASSERT(_json.arrindex(key, ".", "\"abc\"", 2, 3) == -1, "json_arrindex failed");
        REDIS_ASSERT(_json.arrindex(key, ".", "\"abc\"", 2, 4) == 3, "json_arrindex failed");
        REDIS_ASSERT(_json.arrindex(key, ".", "\"abc\"", 4, 5) == -1, "json_arrindex failed");
        REDIS_ASSERT(_json.arrindex(key, ".", "\"abc\"", 4) == -1, "json_arrindex failed");
        _json.del(key, ".");
    }
} // namespace

#endif
