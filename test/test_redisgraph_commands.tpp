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

#ifndef REDIS_MODULE_TEST_REDIS_GRAPH_COMMAND_TPP
#define REDIS_MODULE_TEST_REDIS_GRAPH_COMMAND_TPP

#include <sw/redis++/redis++.h>

namespace redis::module::test {

    template <typename RedisInstance>
    void RedisGraphCommand<RedisInstance>::run(const std::string &key) {

        const auto moduleName = _graph.getModuleName();
        sw::redis::OptionalLongLong ver = _graph.version();
        if (!ver) {
            std::cout << "Skipped RedisGraph tests, as '" << moduleName << "' module was not loaded." << std::endl;
            return;
        }
        std::cout << "Testing RedisGraph ('" << moduleName << "') module version " << *ver << std::endl;

        try {
            _graph.del(key);
        }
        catch (sw::redis::Error e) {
            ;
        }

        {
            std::vector<std::vector<std::string>> result;
            _graph.template query(key, "CREATE (:Member {name:'a'})-[:plays]->(:Team {name:'A'}), (:Member {name:'b'})-[:plays]->(:Team {name:'B'}), (:Member {name:'c'})-[:plays]->(:Team {name:'C'})", result);
        }

        _graph.del(key);
    }

} // namespace

#endif
