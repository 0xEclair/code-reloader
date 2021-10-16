#pragma once

#include <variant>

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

//constexpr std::array<std::string_view, 2> exports = {"foo", "bar"};
namespace std {
    template<typename T>
    using __detail_TI = type_identity<T>;

    auto TTI = []<typename T>(){
        return __detail_TI<std::remove_cvref_t<T>>{};
    };
    struct TI {
        template<typename T>
        auto operator()() const {
            return __detail_TI<std::remove_cvref_t<T>>{};
        }
    };
}
std::array<std::pair<std::string_view , std::any>, 2> exports = {{
    {"foo", std::TTI.operator()<decltype(&foo)>()},
    {"bar", std::TTI.operator()<decltype(bar)>()}
}};

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

    constexpr std::array<std::pair<std::string_view, std::any>, 2UL> & symbols() const override {
        return exports;
    }
};