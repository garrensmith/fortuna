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

#include "iostream"
#include "libplatform/libplatform.h"
#include "v8.h"
#include <algorithm>
#include <vector>

class JSEnv {
public:
	JSEnv(const std::string& id) : id(id) {
		createIsolate();
		createGlobalObject();
	};

	void createIsolate();
	void addFun(const std::string& id, const std::string& fun);
	const std::string mapDoc(const std::string& doc);

	const std::string executeFun(v8::Global<v8::Function>& globalFun, std::vector<const std::string> argvRaw);

	const std::string getId();

private:
	const std::string& id;
	v8::Isolate* getIsolate();
	v8::Isolate* isolate;
	v8::Global<v8::Context> context_;

	v8::Global<v8::Function> mapDoc_;
	v8::Global<v8::Function> addFun_;

	void test(v8::Global<v8::Function> fun);
	void createGlobalObject();
	void setFunctions(v8::Isolate* isolate);
	v8::Local<v8::Function> getFunction(v8::Isolate* isolate, const char* funNameRaw);
	void loadAllScripts(v8::Isolate* isolate);
	bool executeScript(v8::Isolate* isolate, v8::Local<v8::String> script);
};