#include "config.h"
#include <iostream>

using namespace std;

int main() {
    cout
        << PACKAGE_STRING "\n"
        << "Copyright (C) 2022 Lea C.J. <" PACKAGE_BUGREPORT ">\n"
        "This work is free. You can redistribute it and/or modify it under the\n"
        "terms of the Do What The Fuck You Want To Public License, Version 2,\n"
        "as published by Sam Hocevar. See the COPYING file for more details.\n"
        << "\n";
    return 0;
}
