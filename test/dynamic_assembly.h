#include <ffi.h>
#include <stdbool.h>

/*
This file contains the declarations for the dyanmic assembly_function_t. It also
contains the the set up and tear down functions for a dynamic_assembly_functiont_t.
To load the excute these functions is contained in execute.c and load.c
*/
typedef struct {
    void *library_handle;
    void *function_ptr;
    char *function_name;
    char *assembly_file;
    char *object_file;
    char *library_file;

    // Foreign Function Interface for excuting assmebly functions in C
    ffi_cif cif;
    ffi_type **arg_types;
    ffi_type *return_type;
    int arg_count;
    bool is_prepared;
} dynamic_assembly_function_t;

typedef enum {
    FFI_ARG_VOID,
    FFI_ARG_INT,
    FFI_ARG_LONG,
    FFI_ARG_FLOAT,
    FFI_ARG_DOUBLE,
    FFI_ARG_POINTER,
    FFI_ARG_STRING
} ffi_arg_type_t;


dynamic_assembly_function_t *create_dynam_asm_func(void);
void unload_assembly_function(dynamic_assembly_function_t *);
