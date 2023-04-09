#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>

context (example) {

    __describe
    ("Hello world", 
    ({ void __$__() {
        __it ("true should be true", ({ void __$__()("true should be true") {
            should_bool(true) be equal to(true);
        } __$__;}));
    }__$__;}));
}