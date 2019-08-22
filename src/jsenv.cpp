#include <algorithm>
#include <iostream>
#include <vector>

#include "jsenv.h"
#include "libplatform/libplatform.h"
#include "v8.h"

#include "ateles_map.h"
#include "escodegen.h"
#include "esprima.h"
#include "rewrite_anon_fun.h"

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}

void log(const char* event) {
	printf("logged: %s\n", event);
}

// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
void print(const v8::FunctionCallbackInfo<v8::Value>& args) {
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope(args.GetIsolate());
		if (first) {
			first = false;
		} else {
			printf(" ");
		}
		v8::String::Utf8Value str(args.GetIsolate(), args[i]);
		const char* cstr = ToCString(str);
		printf("%s", cstr);
	}
	printf("\n");
	fflush(stdout);
}

const std::string JSEnv::getId() {
	return id;
}

void JSEnv::createIsolate() {
	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	isolate = v8::Isolate::New(create_params);
}

void JSEnv::addFun(const std::string& id, const std::string& fun) {

	std::vector<const std::string> args = { id, fun };
	auto isolate = getIsolate();
	executeFun(isolate, args, [&]() { return v8::Local<v8::Function>::New(isolate, addFun_); });
}

const std::string JSEnv::mapDoc(const std::string& doc) {
	std::vector<const std::string> args = { doc };
	auto isolate = getIsolate();
	return executeFun(isolate, args, [&]() { return v8::Local<v8::Function>::New(isolate, mapDoc_); });
}

const std::string JSEnv::executeFun(v8::Isolate* isolate, std::vector<const std::string> argvRaw,
                                    const std::function<v8::Local<v8::Function>()>& getGlobalfun) {

	v8::Isolate::Scope isolate_scope(isolate);
	v8::Locker locker(isolate);

	v8::HandleScope handle_scope(isolate);
	{
		v8::Local<v8::Context> context = v8::Local<v8::Context>::New(isolate, context_);

		v8::Context::Scope context_scope(context);

		std::vector<v8::Local<v8::Value>> argv;

		std::transform(
		    argvRaw.begin(), argvRaw.end(), std::back_inserter(argv),
		    [&](const std::string& arg) -> v8::Local<v8::Value> {
			    return v8::String::NewFromUtf8(isolate, arg.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
		    });

		v8::Local<v8::Value> result;

		auto fun = getGlobalfun();

		v8::TryCatch try_catch(getIsolate());
		if (!fun->Call(context, context->Global(), argv.size(), argv.data()).ToLocal(&result)) {
			v8::String::Utf8Value error(getIsolate(), try_catch.Exception());
			log(*error);
		}

		// return result;

		// Convert the result to an UTF8 string and print it.
		v8::String::Utf8Value utf8(isolate, result);
		printf("%s\n", *utf8);
		return std::string(*utf8);
	}
}

v8::Isolate* JSEnv::getIsolate() {
	return isolate;
}

void JSEnv::createGlobalObject() {
	v8::Isolate::Scope isolate_scope(isolate);
	v8::Locker locker(isolate);
	v8::HandleScope handle_scope(isolate);
	{
		v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
		global->Set(v8::String::NewFromUtf8(isolate, "print", v8::NewStringType::kNormal).ToLocalChecked(),
		            v8::FunctionTemplate::New(isolate, print));

		auto context = v8::Context::New(isolate, NULL, global);

		context_.Reset(isolate, context);

		// Enter the new context so all the following operations take place
		// within it.
		v8::Context::Scope context_scope(context);

		loadAllScripts(isolate);
		setFunctions(isolate);
	}
}

void JSEnv::setFunctions(v8::Isolate* isolate) {
	auto context = isolate->GetCurrentContext();

	auto mapFun = getFunction(isolate, "mapDoc");
	mapDoc_.Reset(isolate, mapFun);

	auto addFun = getFunction(isolate, "addFun");
	addFun_.Reset(isolate, addFun);
}

v8::Local<v8::Function> JSEnv::getFunction(v8::Isolate* isolate, const char* funNameRaw) {
	auto context = isolate->GetCurrentContext();

	auto funName = v8::String::NewFromUtf8(getIsolate(), funNameRaw, v8::NewStringType::kNormal).ToLocalChecked();
	v8::Local<v8::Value> funVal;

	// If there is no Process function, or if it is not a function,
	// bail out
	if (!context->Global()->Get(context, funName).ToLocal(&funVal) || !funVal->IsFunction()) {
		std::cout << "NOT A FUNCTION" << std::endl;
	}

	// It is a function; cast it to a Function
	return v8::Local<v8::Function>::Cast(funVal);
}

void JSEnv::loadAllScripts(v8::Isolate* isolate) {
	std::vector<const char*> sources{ ateles_map_source, escodegen_source, rewrite_anon_fun_source, esprima_source };

	auto execute = [isolate, this](const char* csource) {
		auto source = v8::String::NewFromUtf8(isolate, csource, v8::NewStringType::kNormal).ToLocalChecked();
		executeScript(isolate, source);
	};

	std::for_each(sources.begin(), sources.end(), execute);
}

bool JSEnv::executeScript(v8::Isolate* isolate, v8::Local<v8::String> script) {
	// v8::HandleScope handle_scope(isolate);

	// We're just about to compile the script; set up an error handler to
	// catch any exceptions the script might throw.
	v8::TryCatch try_catch(isolate);

	v8::Local<v8::Context> context(isolate->GetCurrentContext());

	// Compile the script and check for errors.
	v8::Local<v8::Script> compiled_script;
	if (!v8::Script::Compile(context, script).ToLocal(&compiled_script)) {
		v8::String::Utf8Value error(isolate, try_catch.Exception());
		log(*error);
		// The script failed to compile; bail out.
		return false;
	}

	// Run the script!
	v8::Local<v8::Value> result;
	if (!compiled_script->Run(context).ToLocal(&result)) {
		// The TryCatch above is still in effect and will have caught the
		// error.
		v8::String::Utf8Value error(isolate, try_catch.Exception());
		log(*error);
		// Running the script failed; bail out.
		return false;
	}

	return true;
}
