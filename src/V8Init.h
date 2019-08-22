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
