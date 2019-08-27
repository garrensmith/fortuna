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

#include "libplatform/libplatform.h"
#include "v8.h"

class V8Init {
private:
	std::unique_ptr<v8::Platform> platform;

public:
	V8Init() {}

	~V8Init() { shutdownPlatform(); };

	void shutdownPlatform() {
		v8::V8::ShutdownPlatform();
		v8::V8::Dispose();
	};

	void initialisePlatform() {
		const char* argv[] = { "" };
		initialisePlatform(argv);
	};

	void initialisePlatform(const char* argv[]) {
		v8::V8::InitializeICUDefaultLocation(argv[0]);
		v8::V8::InitializeExternalStartupData(argv[0]);
		platform = v8::platform::NewDefaultPlatform();
		v8::V8::InitializePlatform(platform.get());
		v8::V8::Initialize();
	}

	v8::Platform* getPlatform() { return platform.get(); }
};
