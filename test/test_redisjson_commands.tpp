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

        auto del_ret = _json.del(key);
        REDIS_ASSERT(del_ret == 1 || del_ret == 0, "json_del failed");

        _json.set(key, ".", "bar");

        auto strlen_ret = _json.strlen(key);
        REDIS_ASSERT(strlen_ret == 3, "json_strlen failed");

        strlen_ret = _json.strlen(key, ".");
        REDIS_ASSERT(strlen_ret == 3, "json_strlen failed");

        strlen_ret = _json.strappend(key, ".", "000");
        REDIS_ASSERT(strlen_ret == 6, "json_append failed");

        strlen_ret = _json.strappend(key, "111");
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
        _json.set(key, ".", 2);
        auto num_ret = _json.numincrby(key, ".", 2);
        REDIS_ASSERT(num_ret == 4, "json_numincrby failed");

        num_ret = _json.nummultby(key, ".", 2);
        REDIS_ASSERT(num_ret == 8, "json_nummultby failed");

        _json.del(key, ".");
        _json.set(key, ".", " [\"a\",\"b\",\"c\",\"d\"]");
        auto arrlen_ret = _json.arrlen(key, ".");
        REDIS_ASSERT(arrlen_ret == 4, "json_arrlen failed");

        _json.del(key, ".");
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

        std::vector<std::string> paths = {"a", "c"};
        {
            std::string result = _json.template get(key, paths.begin(), paths.end());
            REDIS_ASSERT(result == "{\"a\":1,\"c\":3}", "json_get failed");
        }
        {
            std::string result = _json.template get(key, paths.begin(), paths.end(), "\t");
            REDIS_ASSERT(result == "{\t\"a\":1,\t\"c\":3}", "json_get failed");
        }
        {
            std::string result = _json.template get(key, paths.begin(), paths.end(), "\t", "\n");
            std::cout << result << std::endl;
            REDIS_ASSERT(result == "{\n\t\"a\":1,\n\t\"c\":3\n}", "json_get failed");
        }
        {
            _json.set(key, "c", "\x0002");
            std::string result = _json.template get(key, paths.begin(), paths.end(), "\t", "\n", " ");
            REDIS_ASSERT(result == "{\n\t\"a\": 1,\n\t\"c\": \"\\u0002\"\n}", "json_get failed");
        }
        {
            std::string result = _json.template get(key, paths.begin(), paths.end(), "\t", "\n", " ", true);
            REDIS_ASSERT(result == "{\n\t\"a\": 1,\n\t\"c\": \"\x0002\"\n}", "json_get failed");
        }
        del_ret = _json.forget(key);
        REDIS_ASSERT(del_ret == 1 || del_ret == 0, "json_del failed");

        del_ret = _json.forget(key, ".");
        REDIS_ASSERT(del_ret == 1 || del_ret == 0, "json_del failed");

    }

} // namespace

#endif
