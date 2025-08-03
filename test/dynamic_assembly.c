#include <stdio.h>
#include "dynamic_assembly.h"

dynamic_assembly_function_t *create_dynam_asm_func() {
    dynamic_assembly_function_t *func = malloc(sizeof(dynamic_assembly_function_t));
    func->assembly_file = NULL;
    func->function_name = NULL;
    func->object_file = NULL;
    func->library_handle = NULL;
    func->library_file = NULL;
    func->arg_types = NULL; 
    func->return_type = NULL;

    return func;
}

void unload_assembly_function(dynamic_assembly_function_t *func) {
    if (!func) return;

    if (func->assembly_file) {free(func->assembly_file);}
    if (func->function_name) {free(func->function_name);}

    if (func->object_file) {
        if (access(func->object_file, F_OK) == 0){
            unlink(func->object_file);
        }
        free(func->object_file);
    }
    if (func->library_file) {
        if (access(func->library_file, F_OK) == 0){
            unlink(func->library_file);
        }
        free(func->library_file);
    }

    if (func->library_handle) {
        dlclose(func->library_handle);
    }

    if (func->arg_types) {
        free(func->arg_types);
    }
    free(func);
}