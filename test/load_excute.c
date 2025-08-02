#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "load_excute.h"
#include <ffi.h>

extern int add_numbers(int a, int b);  // Declare the assembly function

int compile_assembly_file(const char *assembly_file, const char *object_file) {
    char command[1024];
    snprintf(command, sizeof(command), "as -o %s %s", object_file, assembly_file);

    int result = system(command);
    return result == 0 ? 0 : -1;
}

int create_shared_library(const char *object_file, const char *library_file) {
    char command[1024];
    snprintf(command, sizeof(command), "clang -shared -o %s %s", library_file, object_file);

    int result = system(command);
    return result == 0 ? 0 : -1;
}

dynamic_assembly_function_t(const char *assembly_file, 
                            const char *function_name) {
    

}

int main(int argc, char *argv[]) {
    compile_assembly_file("math.s", "math.o");
    create_shared_library("math.o", "math.so");

    void *handle = dlopen("math.so", RTLD_LAZY);
    if (!handle) {
        printf("Failed to open shared library: %s\n", dlerror());
        return -1;
    }

    int (*add_numbers)(int, int) = dlsym(handle, "add_numbers");

    int arg1 = atoi(argv[1]);
    int arg2 = atoi(argv[2]);
    int res = add_numbers(arg1, arg2); 

    printf("Results: %d\n", res);

    dlclose(handle);

    return 0;
}