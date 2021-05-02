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

#ifndef REDIS_PLUS_PLUS_REDIS_GRAPH_H
#define REDIS_PLUS_PLUS_REDIS_GRAPH_H

#include "Module.h"
#include <sstream>
#include <iomanip>

namespace redis::module {

template <typename RedisInstance>
class RedisGraph : public Module<RedisInstance>
{
public:
    explicit RedisGraph(RedisInstance& redis)
        : Module<RedisInstance>(redis, "graph", "GRAPH")
        {}

    template <typename Output>
    auto query(const sw::redis::StringView &key,
               const sw::redis::StringView &data,
               Output& result) {
        return Module<RedisInstance>::_redis.template
            command("GRAPH.QUERY", key, data, std::back_inserter(result));
    }

    template <typename Output>
    auto slowlog(const sw::redis::StringView &key,
                 Output& result) {
        return Module<RedisInstance>::_redis.template
            command("GRAPH.SLOWLOG", key, std::back_inserter(result));
    }

    void del(const sw::redis::StringView &key) {
        Module<RedisInstance>::_redis.template
            command("GRAPH.DELETE", key);
    }

};

} // namespace

#endif
