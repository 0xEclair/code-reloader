#pragma once
#include <string_view>
#include <array>
#include <functional>
#include <any>

#include <cstdio>

#include <dlfcn.h>
#include <cassert>

namespace cr {
    auto load = [](std::string_view filepath){
        return dlopen(filepath.data(), RTLD_NOW);
    };

    auto load_symbol = [](void* library, std::string_view symbol){
        return dlsym(library, symbol.data());
    };

    auto reload = [](void*& library, std::string_view filepath){
        dlclose(library);
        library = dlopen(filepath.data(), RTLD_NOW);
    };

    auto print_err = [](){
        printf("Error: %s\n", dlerror());
    };

    template<typename T, size_t num_of_symbols>
    class CrModule {
    public:
        static auto load_library() {
            instance().load();
        }
        static auto reload_library() {
            instance().reload();
        }

    protected:
        static T& instance() {
            static T ins;
            return ins;
        }

        virtual std::string_view path() const = 0;
        virtual const std::array<std::string_view, num_of_symbols>& symbols() const = 0;

        template<typename Ret, typename ...Args>
        Ret execute(std::string_view name, Args... args) {
            if(symbols_.contains(name)) {
                void* tmp = std::any_cast<void*>(symbols_[name]);
                return (reinterpret_cast<Ret(*)(Args...)>(tmp))(args...);
            }
            else {
                printf("function not found: %s", name.data());
            }
        }

        template<typename U>
        U* var(std::string_view name) {
            if(symbols_.contains(name)) {
                void* tmp = std::any_cast<void*>(symbols_[name]);
                return reinterpret_cast<U*>(tmp);
            }
            else {
                return nullptr;
            }
        }

    private:
        auto load() {
            handle_ = dlopen(path().data(), RTLD_NOW);
            if(handle_.has_value()) {
                load_symbols();
            }
        }

        auto reload() {
            dlclose(std::any_cast<void*>(handle_));
            symbols_.clear();
            load();
        }

        auto load_symbols() -> void {
            for(const auto& sym : symbols()) {
                symbols_[sym] = dlsym(std::any_cast<void*>(handle_), sym.data());
            }
        }
        std::any handle_;
    protected:
        std::unordered_map<std::string_view, std::any> symbols_;
    };
}