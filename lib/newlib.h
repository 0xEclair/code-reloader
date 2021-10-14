#pragma once

#include "cr.hpp"
using namespace cr;
// This line prevents C++ name mangling which would prevent dlsym from retrieving
// the correct symbols.
extern "C"
{
void foo();

// The extern keyword here exports a global variable that will be defined in Test.cpp
extern int bar;
}

constexpr std::array<std::string_view, 2> exports = {"foo", "bar"};
class TestModule : public CrModule<TestModule, exports.size()> {
public:
    static auto foo() {
        instance().execute<void>("foo");
    }

    static auto bar() {
        return *instance().var<decltype(::bar)>("bar");
    }

protected:
    std::string_view path() const override {
        return R"(./libnewlib.so)";
    }

    const std::array<std::string_view, 2UL>& symbols() const override {
        return exports;
    }
};