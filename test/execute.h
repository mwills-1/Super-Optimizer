#include <ffi.h>

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
    void *ptr;
    int i;
    long l;
    float f;
    double d;
    char *s;
} universal_result_t;

// =============================================================================
// Excute Asm Function
// =============================================================================

ffi_type *get_ffi_type(ffi_arg_type_t type);
int prepare_ffi_signature(dynamic_assembly_function_t *, ffi_arg_type_t, int, ffi_arg_type_t *);
int execute_assembly_function_ffi(dynamic_assembly_function_t *, void *reuslt, void **args);
long execute_assembly_function_simple(dynamic_assembly_function_t *, ffi_arg_type_t, int);