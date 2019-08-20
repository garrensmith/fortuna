#pragma once

#include "iostream"
#include "libplatform/libplatform.h"
#include "v8.h"
#include <algorithm>
#include <vector>

// Extracts a C string from a V8 Utf8Value.
const char *ToCString(const v8::String::Utf8Value &value) {
    return *value ? *value : "<string conversion failed>";
}

// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
void print(const v8::FunctionCallbackInfo<v8::Value> &args) {
    bool first = true;
    for (int i = 0; i < args.Length(); i++) {
        v8::HandleScope handle_scope(args.GetIsolate());
        if (first) {
            first = false;
        } else {
            printf(" ");
        }
        v8::String::Utf8Value str(args.GetIsolate(), args[i]);
        const char *cstr = ToCString(str);
        printf("%s", cstr);
    }
    printf("\n");
    fflush(stdout);
}

// Reads a file into a v8 string.
v8::Local<v8::String> readFile(v8::Isolate *isolate, const char *name) {
    FILE *file = fopen(name, "rb");
    if (file == NULL)
        return v8::MaybeLocal<v8::String>().ToLocalChecked();

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char *chars = new char[size + 1];
    chars[size] = '\0';
    for (size_t i = 0; i < size;) {
        i += fread(&chars[i], 1, size - i, file);
        if (ferror(file)) {
            fclose(file);
            return v8::MaybeLocal<v8::String>().ToLocalChecked();
        }
    }
    fclose(file);
    std::cout <<"FIL" << chars << std::endl;
    v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(
        isolate, chars, v8::NewStringType::kNormal, static_cast<int>(size));
    delete[] chars;
    return result.ToLocalChecked();
}

class Environment {
  public:
    Environment(std::string &id) : id(id) {
        createIsolate();
        createGlobalObject();
    };

    void createIsolate() {
        v8::Isolate::CreateParams create_params;
        create_params.array_buffer_allocator =
            v8::ArrayBuffer::Allocator::NewDefaultAllocator();
        isolate = v8::Isolate::New(create_params);
    }

    void hello(std::string &name) {

        auto isolate = getIsolate();

        v8::Isolate::Scope isolate_scope(isolate);
        v8::Locker locker(isolate);

        v8::HandleScope handle_scope(isolate);
        {
            v8::Local<v8::Context> context =
                v8::Local<v8::Context>::New(isolate, context_);

            v8::Context::Scope context_scope(context);

            v8::Local<v8::String> v8Name =
                v8::String::NewFromUtf8(getIsolate(), name.c_str(),
                                        v8::NewStringType::kNormal)
                    .ToLocalChecked();

            v8::Local<v8::Value> result;

            v8::Local<v8::Function> process =
                v8::Local<v8::Function>::New(isolate, process_);

            // Set up an exception handler before calling the Process function
            const int argc = 1;
            v8::Local<v8::Value> argv[] = {v8Name};
            v8::TryCatch try_catch(getIsolate());
            if (!process->Call(context, context->Global(), argc, argv)
                     .ToLocal(&result)) {
                v8::String::Utf8Value error(getIsolate(),
                                            try_catch.Exception());
                Log(*error);
            }

            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(isolate, result);
            printf("%s\n", *utf8);
        }
    }

    void addFun(std::string &id, std::string &fun) {

        std::vector<std::string> args = {id, fun};
        executeFun(getIsolate(), args);
    }

    void executeFun(v8::Isolate *isolate, std::vector<std::string> argvRaw) {

        v8::Isolate::Scope isolate_scope(isolate);
        v8::Locker locker(isolate);

        v8::HandleScope handle_scope(isolate);
        {
            v8::Local<v8::Context> context =
                v8::Local<v8::Context>::New(isolate, context_);

            v8::Context::Scope context_scope(context);

            std::vector<v8::Local<v8::Value>> argv;

            std::transform(
                argvRaw.begin(), argvRaw.end(), std::back_inserter(argv),
                [isolate](std::string &arg) -> v8::Local<v8::Value> {
                    return v8::String::NewFromUtf8(isolate, arg.c_str(),
                                                   v8::NewStringType::kNormal)
                        .ToLocalChecked();
                });

            // v8::Local<v8::String> funStr =
            //     v8::String::NewFromUtf8(getIsolate(), fun.c_str(),
            //                             v8::NewStringType::kNormal)
            //         .ToLocalChecked();

            // v8::Local<v8::String> idStr =
            //     v8::String::NewFromUtf8(getIsolate(), fun.c_str(),
            //                             v8::NewStringType::kNormal)
            //         .ToLocalChecked();

            v8::Local<v8::Value> result;

            v8::Local<v8::Function> fun =
                v8::Local<v8::Function>::New(isolate, addFun_);

            // Set up an exception handler before calling the Process function
            // const int argc = 2;
            // v8::Local<v8::Value> argv[] = {idStr, funStr};
            v8::TryCatch try_catch(getIsolate());
            if (!fun->Call(context, context->Global(), argv.size(), argv.data())
                     .ToLocal(&result)) {
                v8::String::Utf8Value error(getIsolate(),
                                            try_catch.Exception());
                Log(*error);
            }

            // return result;

            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(isolate, result);
            printf("%s\n", *utf8);
        }
    }

    v8::Isolate *getIsolate() { return isolate; }

  private:
    std::string &id;
    v8::Isolate *isolate;
    v8::Global<v8::Context> context_;
    v8::Global<v8::Function> process_;

    v8::Global<v8::Function> mapDoc_;
    v8::Global<v8::Function> addFun_;
    v8::Global<v8::Function> rewriteAnonFun_;

    void createGlobalObject() {
        v8::HandleScope handle_scope(isolate);
        {
            v8::Local<v8::ObjectTemplate> global =
                v8::ObjectTemplate::New(isolate);
            global->Set(v8::String::NewFromUtf8(isolate, "print",
                                                v8::NewStringType::kNormal)
                            .ToLocalChecked(),
                        v8::FunctionTemplate::New(isolate, print));

            auto context = v8::Context::New(isolate, NULL, global);

            // Each processor gets its own context so different processors don't
            // affect each other. Context::New returns a persistent handle which
            // is what we need for the reference to remain after we return from
            // this method. That persistent handle has to be disposed in the
            // destructor.
            context_.Reset(isolate, context);

            // Enter the new context so all the following operations take place
            // within it.
            v8::Context::Scope context_scope(context);

            // const char *csource = R"(
            //     function hello (name) {
            //     return "hope this works " + name;
            //     }
            // )";

            loadAllScripts(isolate);
            setFunctions(isolate);

            // // Create a string containing the JavaScript source code.
            // auto source = v8::String::NewFromUtf8(isolate, csource,
            //                                       v8::NewStringType::kNormal)
            //                   .ToLocalChecked();

            // executeScript(source);

            // The script compiled and ran correctly.  Now we fetch out the
            // Process function from the global object.
            // auto process_name =
            //     v8::String::NewFromUtf8(getIsolate(), "hello",
            //                             v8::NewStringType::kNormal)
            //         .ToLocalChecked();
            // v8::Local<v8::Value> process_val;
            // // If there is no Process function, or if it is not a function,
            // // bail out
            // if (!context->Global()
            //          ->Get(context, process_name)
            //          .ToLocal(&process_val) ||
            //     !process_val->IsFunction()) {
            //     std::cout << "NOT A FUNCTION" << std::endl;
            // }

            // // It is a function; cast it to a Function
            // v8::Local<v8::Function> process =
            //     v8::Local<v8::Function>::Cast(process_val);

            // // Store the function in a Global handle, since we also want
            // // that to remain after this call returns
            // process_.Reset(isolate, process);
        }
    }

    void setFunctions(v8::Isolate *isolate) {
        auto context = isolate->GetCurrentContext();

        auto mapFun = getFunction(isolate, "mapDoc");
        mapDoc_.Reset(isolate, mapFun);

        auto addFun = getFunction(isolate, "addFun");
        addFun_.Reset(isolate, addFun);

        auto rewriteAnon = getFunction(isolate, "rewriteAnonFun");
        rewriteAnonFun_.Reset(isolate, rewriteAnon);
    }

    v8::Local<v8::Function> getFunction(v8::Isolate *isolate,
                                        const char *funNameRaw) {
        auto context = isolate->GetCurrentContext();

        auto funName = v8::String::NewFromUtf8(getIsolate(), funNameRaw,
                                               v8::NewStringType::kNormal)
                           .ToLocalChecked();
        v8::Local<v8::Value> funVal;

        // If there is no Process function, or if it is not a function,
        // bail out
        if (!context->Global()->Get(context, funName).ToLocal(&funVal) ||
            !funVal->IsFunction()) {
            std::cout << "NOT A FUNCTION" << std::endl;
        }

        // It is a function; cast it to a Function
        return v8::Local<v8::Function>::Cast(funVal);
    }

    void loadAllScripts(v8::Isolate *isolate) {
        std::vector<std::string> files{
                                        "./js/ateles_map.js",
                                       "./js/escodegen.js",
                                       "./js/esprima.js",
                                       "./js/rewrite_anon_fun.js"};

        auto execute = [isolate, this](const std::string &file) {
            auto source = readFile(isolate, file.c_str());
            executeScript(isolate, source);
        };

        std::for_each(files.begin(), files.end(), execute);
    }

    bool executeScript(v8::Isolate *isolate, v8::Local<v8::String> script) {
        v8::HandleScope handle_scope(isolate);

        // We're just about to compile the script; set up an error handler to
        // catch any exceptions the script might throw.
        v8::TryCatch try_catch(isolate);

        v8::Local<v8::Context> context(isolate->GetCurrentContext());

        std::cout << "compiling" << std::endl;
        // Compile the script and check for errors.
        v8::Local<v8::Script> compiled_script;
        if (!v8::Script::Compile(context, script).ToLocal(&compiled_script)) {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            Log(*error);
            // The script failed to compile; bail out.
            return false;
        }

        std::cout << "run" << std::endl;
        // Run the script!
        v8::Local<v8::Value> result;
        if (!compiled_script->Run(context).ToLocal(&result)) {
            // The TryCatch above is still in effect and will have caught the
            // error.
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            Log(*error);
            // Running the script failed; bail out.
            return false;
        }
        std::cout << "done" << std::endl;

        return true;
    }

    void Log(const char *event) { printf("Logged: %s\n", event); }
};