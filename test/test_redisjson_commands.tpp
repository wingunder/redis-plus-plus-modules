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

        auto type_ret = _json.type(key);
        REDIS_ASSERT(type_ret == "string", "json_type failed");

        type_ret = _json.type(key, ".");
        REDIS_ASSERT(type_ret == "string", "json_type failed");

        del_ret = _json.del(key, ".");
        REDIS_ASSERT(del_ret == 1 || del_ret == 0, "json_del failed");

        del_ret = _json.forget(key);
        REDIS_ASSERT(del_ret == 1 || del_ret == 0, "json_del failed");

        del_ret = _json.forget(key, ".");
        REDIS_ASSERT(del_ret == 1 || del_ret == 0, "json_del failed");

    }

} // namespace

#endif
