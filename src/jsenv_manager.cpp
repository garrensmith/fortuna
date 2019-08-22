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