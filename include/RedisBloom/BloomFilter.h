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

#ifndef REDIS_PLUS_PLUS_BLOOM_FILTER_H
#define REDIS_PLUS_PLUS_BLOOM_FILTER_H

#include <sw/redis++/redis++.h>
#include <algorithm>
#include <iterator>

namespace redis::module {

template <typename T>
class BloomFilter : public T
{
public:
    explicit BloomFilter(const sw::redis::ConnectionOptions &connection_opts,
                   const sw::redis::ConnectionPoolOptions &pool_opts = {}) :
        T(connection_opts, pool_opts) {}

    explicit BloomFilter(const std::string &uri);

    BloomFilter(const std::shared_ptr<sw::redis::Sentinel> &sentinel,
          const std::string &master_name,
          sw::redis::Role role,
          const sw::redis::ConnectionOptions &connection_opts,
          const sw::redis::ConnectionPoolOptions &pool_opts = {}) :
        T(sentinel, master_name, role, connection_opts, pool_opts) {}

    BloomFilter(const BloomFilter &) = delete;
    BloomFilter& operator=(const BloomFilter &) = delete;
    BloomFilter(BloomFilter &&) = default;
    BloomFilter& operator=(BloomFilter &&) = default;

    // The following is an implementation of:
    //   https://oss.redislabs.com/redisbloom/Bloom_Commands/

    bool reserve(const sw::redis::StringView &key,
                 double error_rate,
                 long long capacity,
                 bool nonscaling,
                 long long expansion = default_expansion_rate) {
        if (nonscaling) {
            auto reply = T::command("BF.RESERVE",  key, error_rate, capacity, "EXPANSION", expansion, "NOSCALING");
            auto result = sw::redis::reply::parse<std::string>(*reply);
            return (result == "OK");
        }
        else {
            auto reply = T::command("BF.RESERVE",  key, error_rate, capacity, "EXPANSION", expansion);
            auto result = sw::redis::reply::parse<std::string>(*reply);
            return (result == "OK");
        };
    }

    long long add(const sw::redis::StringView &key,
                  const sw::redis::StringView &item) {
        auto reply = T::command("BF.ADD",  key, item);
        return sw::redis::reply::parse<long long>(*reply);
    }

    template <typename Input, typename Output>
    void madd(const std::string& key,
              Input first,
              Input last,
              Output &result) {
        m_command("BF.MADD", key, first, last, result);
    }

    template <typename Input, typename Output>
    void insert(const sw::redis::StringView &key,
                double error_rate,
                long long capacity,
                bool nonscaling,
                bool nocreate,
                Input first,
                Input last,
                Output &result,
                long long expansion = default_expansion_rate) {
        std::string cmd = "BF.INSERT";
        sw::redis::range_check(cmd.c_str(), first, last);

        std::vector<sw::redis::StringView> args = { cmd, key,
            "CAPACITY", std::to_string(capacity),
            "ERROR", std::to_string(error_rate),
            "EXPANSION", std::to_string(expansion)
        };
        if (nonscaling && nocreate) {
            throw sw::redis::Error("In BF.INSERT, nonscaling and nocreate are mutually exclusive. Setting both is not allowed.");
        }
        else if (nonscaling) {
            args.push_back("NONSCALING");
        }
        else if (nocreate) {
            args.push_back("NOCREATE");
        }
        args.push_back("ITEMS");
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        T::command(args.begin(), args.end(), std::back_inserter(result));
    }

    template <typename Input, typename Output>
    void mexists(const std::string& key,
                 Input first,
                 Input last,
                 Output &result) {
        m_command("BF.MEXISTS", key, first, last, result);
    }

    long long exists(const sw::redis::StringView &key,
                     const sw::redis::StringView &item) {
        auto reply = T::command("BF.EXISTS",  key, item);
        return sw::redis::reply::parse<long long>(*reply);
    }

    void
    scandump(const sw::redis::StringView &key, long long iter, std::pair<long long, std::vector<unsigned char>>& result) {
        std::vector<sw::redis::StringView> args = { "BF.SCANDUMP", key, std::to_string(iter) };
        auto reply = T::command(args.begin(), args.end());
        if (!sw::redis::reply::is_array(*reply)) {
            throw sw::redis::ProtoError("Expect ARRAY reply");
        }

        if (reply->elements != 2) {
            throw sw::redis::ProtoError("NOT key-value PAIR reply");
        }

        if (reply->element == nullptr) {
            throw sw::redis::ProtoError("Null PAIR reply");
        }

        auto *first = reply->element[0];
        auto *second = reply->element[1];
        if (first == nullptr) {
            throw sw::redis::ProtoError("Null iterator reply");
        }
        else {
            result.first = first->integer;
        }
        result.second.clear();
        if (second != nullptr) {
            auto str = sw::redis::reply::parse<std::string>(*second);
            std::copy(str.begin(), str.end(), std::back_inserter(result.second));
        }
    }

    template <typename Output>
    void info(const sw::redis::StringView &key, Output &output) {
        T::command("BF.INFO", key, std::inserter(output, output.end()));
    }

private:
    template <typename Input, typename Output>
    void m_command(const std::string& cmd,
                   const std::string& key,
                   Input first,
                   Input last,
                   Output &result) {
        sw::redis::range_check(cmd.c_str(), first, last);
        std::vector<std::string> args = { cmd, key };
        std::for_each(first, last, [&args](auto &s){ args.push_back(s); });
        T::command(args.begin(), args.end(), std::back_inserter(result));
    }

    static const long long default_expansion_rate = 2;

};

} // namespace

//#include "bloomfilter.tpp"

#endif
