#pragma once

#include "libplatform/libplatform.h"

class JS {
private:
    std::unique_ptr<v8::Platform> platform;
public:
    JS(/* args */) {

        initialise_platform();
    }
    ~JS() {

    }

    void initialise_platform () {
        v8::V8::InitializeICUDefaultLocation(argv[0]);
        v8::V8::InitializeExternalStartupData(argv[0]);
        platform = v8::platform::NewDefaultPlatform();
        v8::V8::InitializePlatform(platform.get());
        v8::V8::Initialize();
    }

    void hello();
};

// JS::js(/* args */)
// {
// }

// JS::~js()
// {
// }
