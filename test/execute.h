#include <ffi.h>
#include "dynamic_assembly.h"

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
int execute_assembly_function_simple(dynamic_assembly_function_t *, 
                                        ffi_arg_type_t,
                                        universal_result_t *,
                                        int,
                                        universal_arg_t *);

// =============================================================================
// Macros for execution
// =============================================================================

#define CALL_FFI_FUNC_2_INT(func, arg1, arg2) \
    ({ \
        ffi_arg_type_t args[] = {FFI_ARG_INT, FFI_ARG_INT}; \
        int result = 0; \
        if (prepare_ffi_signature(func, FFI_ARG_INT, 2, args) == 0) { \
            int a1 = (arg1), a2 = (arg2); \
            void *argv[] = {&a1, &a2}; \
            execute_assembly_function_ffi(func, &result, argv); \
        } \
        result; \
    })
