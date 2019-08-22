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

#include "jsenv.h"
#include "jsenv_manager.h"
#include <unordered_map>

JSEnv* JSEnvManager::createEnv(const std::string& id) {
	jsEnvs[id] = std::make_unique<JSEnv>(id);
	return jsEnvs.at(id).get();
}

JSEnv* JSEnvManager::get(const std::string& id) {
	return jsEnvs.at(id).get();
}

JSEnv* JSEnvManager::getOrCreateEnv(const std::string& id) {

	if (jsEnvs.count(id) == 0) {
		return createEnv(id);
	}

	return jsEnvs[id].get();
}

void JSEnvManager::deleteEnv(const std::string& id) {
	if (jsEnvs.count(id) == 0) {
		return;
	}

	jsEnvs.erase(id);
}