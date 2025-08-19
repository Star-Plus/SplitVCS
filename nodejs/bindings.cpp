#include <napi.h>
#include "../src/SplitInterface.h"

Napi::Value Init(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a string argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    const std::string repoPath = info[0].As<Napi::String>().Utf8Value();

    try {
        Split::SplitInterface::Init(repoPath);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }

    return env.Null();
}

Napi::Value Add(const Napi::CallbackInfo& info) {
    const Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected a string argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    const str repoPath = info[0].As<Napi::String>().Utf8Value();
    const str filePath = info[1].As<Napi::String>().Utf8Value();

    try {
        Split::SplitInterface::Add(repoPath, filePath);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
    return env.Null();
}

Napi::Value Commit(const Napi::CallbackInfo& info) {
    const Napi::Env env = info.Env();
    if (info.Length() < 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString()) {
        Napi::TypeError::New(env, "Expected three string arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    const str repoPath = info[0].As<Napi::String>().Utf8Value();
    const str message = info[1].As<Napi::String>().Utf8Value();
    const str author = info[2].As<Napi::String>().Utf8Value();

    try {
        Split::SplitInterface::Commit(repoPath, message, author);
    }
    catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }

    return env.Null();
}

Napi::Value Checkout(const Napi::CallbackInfo& info) {
    const Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected two string arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    const str repoPath = info[0].As<Napi::String>().Utf8Value();
    const str commitHash = info[1].As<Napi::String>().Utf8Value();

    try {
        Split::SplitInterface::Checkout(repoPath, commitHash);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }

    return env.Null();
}
