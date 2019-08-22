#pragma once

#include <unordered_map>
#include "jsenv.h"

class JSEnvManager {
public: 

    JSEnvManager(): jsEnvs() {

    }

    JSEnv* createEnv(const std::string& id);
    JSEnv* get(const std::string& id);
    JSEnv* getOrCreateEnv(const std::string& id);

    void deleteEnv(const std::string& id);

private:
    std::unordered_map<std::string, std::unique_ptr<JSEnv>> jsEnvs;


};