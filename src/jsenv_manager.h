// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

#pragma once

#include "jsenv.h"
#include "lru.h"

class JSEnvManager {
public:
	JSEnvManager(size_t capacity = 100) : jsEnvs(capacity) {}

	std::shared_ptr<JSEnv> createEnv(const std::string& id);
	std::shared_ptr<JSEnv> get(const std::string& id);
	std::shared_ptr<JSEnv> getOrCreateEnv(const std::string& id);

	void deleteEnv(const std::string& id);
	bool hasEnv(const std::string& id);

private:
	LRU<std::string, std::shared_ptr<JSEnv>> jsEnvs;
};