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

#include <unordered_map>
#include "jsenv.h"

class JSEnvManager {
public:
	JSEnvManager() : jsEnvs() {}

	JSEnv* createEnv(const std::string& id);
	JSEnv* get(const std::string& id);
	JSEnv* getOrCreateEnv(const std::string& id);

	void deleteEnv(const std::string& id);

private:
	std::unordered_map<std::string, std::unique_ptr<JSEnv>> jsEnvs;
};