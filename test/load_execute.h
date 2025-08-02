#include <stdio.h>
#include <ffi.h>

// =============================================================================
// Load File to Function
// =============================================================================

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
    int is_prepared;

} dynamic_assembly_function_t;

int compile_assembly_file(const char *, const char *);
int create_shared_library(const char *, const char *);
dynamic_assembly_function_t *create_dynam_ass_func(void);
dynamic_assembly_function_t *load_assembly_function(const char *, const char *);

// =============================================================================
// Excute Function
// =============================================================================
typedef enum {
    FFI_ARG_VOID,
    FFI_ARG_INT,
    FFI_ARG_LONG,
    FFI_ARG_FLOAT,
    FFI_ARG_DOUBLE,
    FFI_ARG_POINTER,
    FFI_ARG_STRING
} ffi_arg_type_t;

typedef struct {
    ffi_arg_type_t type;
    union {
        int i;
        long l;
        float f;
        double d;
        void *p;
        char *s;
    } value;
} universal_arg_t;

typedef union {
    int i;
    long l;
    float f;
    double d;
    void *p;
    char *s;
} universal_result_t;

ffi_type *get_ffi_type(ffi_arg_type_t type);
int prepare_ffi_signature(dynamic_assembly_function_t *, ffi_arg_type_t, int, ffi_arg_type_t);
int execute_assembly_function_ffi(dynamic_assembly_function_t *, void *reuslt, void **args);
long execute_assembly_function_simple(dynamic_assembly_function_t *, ffi_arg_type_t, int);

// =============================================================================
// Clean Up
// =============================================================================

void unload_assembly_function(dynamic_assembly_function_t *);



