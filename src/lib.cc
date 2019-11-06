#include <iostream>
#include <string>
#include <direct.h>
#include "lib.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

functions_union xelib;
HINSTANCE XEditLib;

void log(std::string message) {
    std::cout << message << std::endl;
}
// dllPath must be absolute on linux, or if it's relative, LD_LIBRARY_PATH must be set
int load(std::string dllPath) {
    LPSTR libPath = (LPSTR) strdup(dllPath.c_str());

    try {
        #ifdef _WIN32
            XEditLib = LoadLibrary(libPath);
        #else
            XEditLib = dlopen(libPath);
        #endif

        if (!XEditLib) {
            log("Failed to to load " + dllPath);
            return EXIT_FAILURE;
        }

        for (int i = 0; i < NUM_FUNCTIONS; i++) {
            
            #ifdef _WIN32
                xelib.function_pointers[i] = GetProcAddress(XEditLib, FUNCTION_NAMES[i]);
            #else
                xelib.function_pointers[i] = dlsym(XEditLib, FUNCTION_NAMES[i]);
            #endif

            if (xelib.function_pointers[i] == NULL) {
                log("Failed to to bind XEditLib.dll:" + std::string(FUNCTION_NAMES[i]));
                return EXIT_FAILURE;
            }
        }
        return EXIT_SUCCESS;
    } catch(...) {#ifdef _WIN32
                free(libPath);
            #else
                dlclose(libPath);
            #endif
        return EXIT_FAILURE;
    }
}
