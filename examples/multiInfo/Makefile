# Copyright 2021 Pieter du Preez <pdupreez@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

REDIS_PLUS_PLUS_MODULES_DIR = ../..

all: multiInfo

multiInfo: multiInfo.cpp
	g++ -I$(REDIS_PLUS_PLUS_MODULES_DIR)/include -L$(REDIS_PLUS_PLUS_MODULES_DIR)/lib -o $@ $< -lhiredis -lredis++ -lpthread

clean:
	rm -f multiInfo

run: multiInfo
	LD_LIBRARY_PATH=$(REDIS_PLUS_PLUS_MODULES_DIR)/lib ./multiInfo
