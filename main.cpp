#include <iostream>
#include <dlfcn.h>

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
}

int main() {
    std::cout << "Hello, World!" << std::endl;
//    Foo f;
//    std::cout << "The answer is " << f.GetTheAnswer() << '\n';
    std::string_view filepath = R"(./libnewlib.so)";
    auto handle = cr::load(filepath);
    if(handle) {
        auto foo = reinterpret_cast<void(*)()>(cr::load_symbol(handle, "foo"));
        foo();

        int bar = *reinterpret_cast<int*>(cr::load_symbol(handle, "bar"));
        std::cout << "bar == " << bar << '\n';
        std::cout << "Make some changes, recompile, and press enter." << std::flush;
        while(std::cin.get() != '\n') {

        }
        cr::reload(handle, filepath);

        foo = reinterpret_cast<void(*)()>(cr::load_symbol(handle, "foo"));
        foo();

        bar = *reinterpret_cast<int*>(cr::load_symbol(handle, "bar"));
        std::cout << "bar == " << bar << '\n';
    }
    else {
        cr::print_err();
    }
    return 0;
}
