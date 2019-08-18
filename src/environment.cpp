#pragma once

#include "libplatform/libplatform.h"

class Environment {
    public:
        Environment(std::string& id): id(id) {

            createIsolate();

        }

    void createIsolate();
    
    private:
        std::string& id;
};