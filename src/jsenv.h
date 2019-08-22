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
	const std::string executeFun(v8::Isolate* isolate, std::vector<const std::string> argvRaw,
	                             const std::function<v8::Local<v8::Function>()>& getGlobalfun);

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