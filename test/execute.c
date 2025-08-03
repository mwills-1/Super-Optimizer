#include <ffi.h>
#include "execute.h"
#include <stdbool.h>

ffi_type* get_ffi_type(ffi_arg_type_t type) {
    switch (type) {
        case FFI_ARG_VOID:    return &ffi_type_void;
        case FFI_ARG_INT:     return &ffi_type_sint;
        case FFI_ARG_LONG:    return &ffi_type_slong;
        case FFI_ARG_FLOAT:   return &ffi_type_float;
        case FFI_ARG_DOUBLE:  return &ffi_type_double;
        case FFI_ARG_POINTER:
        case FFI_ARG_STRING:  return &ffi_type_pointer;
        default:              return &ffi_type_void;
    }
}

int prepare_ffi_signature(dynamic_assembly_function_t *func, 
                        ffi_arg_type_t return_type, 
                        int arg_count, 
                        ffi_arg_type_t *arg_types) {
    
    assert(func != NULL);

    if (func->is_prepared &&q

}
