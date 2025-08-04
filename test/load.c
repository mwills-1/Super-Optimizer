#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include "load.h"
#include <ffi.h>
#include <unistd.h>
#include <assert.h>

extern int add_numbers(int a, int b);  // Declare the assembly function

int compile_assembly_file(const char *assembly_file, const char *object_file) {
    assert(assembly_file != NULL);
    assert(object_file != NULL);

    char command[1024];
    snprintf(command, sizeof(command), "as -o %s %s", object_file, assembly_file);

    int result = system(command);
    return result == 0 ? 0 : -1;
}

int create_shared_library(const char *object_file, const char *library_file) {
    assert(object_file != NULL);
    assert(library_file != NULL);

    char command[1024];
    snprintf(command, sizeof(command), "clang -shared -o %s %s", library_file, object_file);

    int result = system(command);  
    return result == 0 ? 0 : -1;
}


dynamic_assembly_function_t *load_assembly_function(const char *assembly_file, 
                            const char *function_name) {
    assert(assembly_file != NULL);
    assert(function_name != NULL);
    
    dynamic_assembly_function_t *func = create_dynam_asm_func();

    func->assembly_file = strdup(assembly_file);
    func->function_name = strdup(function_name);

    // Create name for file names
    char object_file[256];
    char library_file[256];
    snprintf(object_file, sizeof(object_file), "asm_%d.o", getpid());
    snprintf(library_file, sizeof(library_file), "asm_%d.so", getpid());
    
    func->object_file = strdup(object_file);
    func->library_file = strdup(library_file);

    if (compile_assembly_file(assembly_file, object_file) != 0) {
        printf("Failed to compile assembly file: %s\n", assembly_file);
        unload_assembly_function(func);
        return NULL;
    }
    // printf("%s, %s, %s, %s", func->assembly_file, func->function_name, func->object_file, func->library_file);
    // return NULL;

    if (create_shared_library(object_file, library_file) != 0) {
        printf("Failed to create shared library: %s\n", object_file);
        unload_assembly_function(func);
        return NULL;
    }

    func->library_handle = dlopen(func->library_file, RTLD_LAZY);
    if (!func->library_handle) {
        printf("Failed to load library in shared memeory: %s\n", assembly_file);
        unload_assembly_function(func);
        return NULL;
    }

    func->function_ptr = dlsym(func->library_handle, func->function_name);
    if (!func->function_ptr) {
        printf("Failed to find function: %s, %s\n", function_name, dlerror());
        unload_assembly_function(func);
        return NULL;
    }

    return func;
}


// Main function moved to main.c