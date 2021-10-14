#include <iostream>
#include <filesystem>

#include "lib/cr.hpp"
#include "lib/newlib.h"

auto main() -> int {
    std::cout << std::filesystem::current_path() << '\n';
    TestModule::load_library();
    TestModule::foo();
    std::cout << "bar == " << TestModule::bar() << '\n';

    std::cout << "make some changes, wait...\n";
    std::cout << "press a char\n";
    while(std::cin.get() !='\n') {}

    TestModule::reload_library();
    TestModule::foo();
    std::cout << "bar == " << TestModule::bar() << '\n';

    return 245;
}