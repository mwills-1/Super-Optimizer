#include <ffi.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "execute.h"

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
        func->is_prepared = false;
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

void execute_assembly_function_ffi(dynamic_assembly_function_t func *, \
                                    void *reuslt, \
                                    void **args) {
    assert(func != NULL);

    if (!func->is_prepared || !func->function_ptr) {
        return -1;
    }

    ffi_call(&func->cfi, func->function_ptr, result, args);
    return 0;
}

int execute_assembly_function_simple(dynamic_assembly_function_t *func,
                                        ffi_arg_type_t return_type,
                                        universal_result_t *result,
                                        int arg_count,
                                        universal_arg_t *args) {
    if (!func || !func->function_ptr || !result) {
        return -1;
    }
    
    // Prepare FFI signature if not already done or if signature changed
    if (!func->is_prepared || func->arg_count != arg_count) {
        if (func->is_prepared && func->arg_types) {
            free(func->arg_types);
            func->arg_types = NULL;
            func->is_prepared = 0;
        }
        
        ffi_arg_type_t *arg_types = NULL;
        if (arg_count > 0) {
            arg_types = malloc(arg_count * sizeof(ffi_arg_type_t));
            if (!arg_types) {
                return -1;
            }
            
            for (int i = 0; i < arg_count; i++) {
                arg_types[i] = args[i].type;
            }
        }
        
        int prep_result = prepare_ffi_signature(func, return_type, arg_count, arg_types);
        free(arg_types);
        
        if (prep_result != 0) {
            return -1;
        }
    }
    
    // Prepare argument pointers
    void **arg_ptrs = NULL;
    if (arg_count > 0) {
        arg_ptrs = malloc(arg_count * sizeof(void*));
        if (!arg_ptrs) {
            return -1;
        }
        
        for (int i = 0; i < arg_count; i++) {
            switch (args[i].type) {
                case FFI_ARG_INT:
                    arg_ptrs[i] = &args[i].value.i;
                    break;
                case FFI_ARG_LONG:
                    arg_ptrs[i] = &args[i].value.l;
                    break;
                case FFI_ARG_FLOAT:
                    arg_ptrs[i] = &args[i].value.f;
                    break;
                case FFI_ARG_DOUBLE:
                    arg_ptrs[i] = &args[i].value.d;
                    break;
                case FFI_ARG_POINTER:
                case FFI_ARG_STRING:
                    arg_ptrs[i] = &args[i].value.p;
                    break;
                default:
                    free(arg_ptrs);
                    return -1;
            }
        }
    }
    
    // Execute the function based on return type
    switch (return_type) {
        case FFI_ARG_VOID:
            execute_assembly_function_ffi(func, NULL, arg_ptrs);
            result->ptr = NULL;
            break;
        case FFI_ARG_INT:
            execute_assembly_function_ffi(func, &result->i, arg_ptrs);
            break;
        case FFI_ARG_LONG:
            execute_assembly_function_ffi(func, &result->l, arg_ptrs);
            break;
        case FFI_ARG_FLOAT:
            execute_assembly_function_ffi(func, &result->f, arg_ptrs);
            break;
        case FFI_ARG_DOUBLE:
            execute_assembly_function_ffi(func, &result->d, arg_ptrs);
            break;
        case FFI_ARG_POINTER:
        case FFI_ARG_STRING:
            execute_assembly_function_ffi(func, &result->ptr, arg_ptrs);
            break;
        default:
            free(arg_ptrs);
            return -1;
    }
    
    free(arg_ptrs);
    return 0;                                
}
