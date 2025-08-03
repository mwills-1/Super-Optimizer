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

    if (func->is_prepared && func->arg_types) { 
        func->is_preapged = false;
        free(func->arg_types);
        func->arg_types = NULL;
    }

    func->arg_count = arg_count;
    func->return_type = get_ffi_type(return_type);

    if (arg_count > 0) {
        func->arg_types = malloc(arg_count *sizeof(ffi_type *));

        if (!func->arg_types) {
            return -1;
        }
        for (int i =  0; i < arg_count; i ++) {
            func->arg_types[i] = get_ffi_type(arg_types[i]);
        }
    }

    ffi_status status = ffi_prep_cif(&func->cif, FFI_DEFAULT_ABI, 
        arg_count, func->return_type, func->arg_types);

    if (status == FFI_OK) {
        func->is_prepared = 1;
        return 0;
    }

    // Cleanup on failure
    if (func->arg_types) {
        free(func->arg_types);
        func->arg_types = NULL;
    }

    return -1;
}
