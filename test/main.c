#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "load.h"
#include "execute.h"

// Convert string to ffi_arg_type_t
ffi_arg_type_t string_to_arg_type(const char *type_str) {
    if (strcmp(type_str, "int") == 0) return FFI_ARG_INT;
    if (strcmp(type_str, "long") == 0) return FFI_ARG_LONG;
    if (strcmp(type_str, "float") == 0) return FFI_ARG_FLOAT;
    if (strcmp(type_str, "double") == 0) return FFI_ARG_DOUBLE;
    if (strcmp(type_str, "pointer") == 0) return FFI_ARG_POINTER;
    if (strcmp(type_str, "string") == 0) return FFI_ARG_STRING;
    if (strcmp(type_str, "void") == 0) return FFI_ARG_VOID;
    
    fprintf(stderr, "Unknown type: %s\n", type_str);
    return FFI_ARG_INT; // Default to int
}

// Convert string to int
int string_to_int(const char *str) {
    return atoi(str);
}

// Convert string to float
float string_to_float(const char *str) {
    return atof(str);
}

// Convert string to double
double string_to_double(const char *str) {
    return atof(str);
}

// Print result based on type
void print_result(ffi_arg_type_t result_type, universal_result_t result) {
    switch (result_type) {
        case FFI_ARG_INT:
            printf("Result: %d\n", result.i);
            break;
        case FFI_ARG_LONG:
            printf("Result: %ld\n", result.l);
            break;
        case FFI_ARG_FLOAT:
            printf("Result: %f\n", result.f);
            break;
        case FFI_ARG_DOUBLE:
            printf("Result: %lf\n", result.d);
            break;
        case FFI_ARG_POINTER:
            printf("Result: %p\n", result.ptr);
            break;
        case FFI_ARG_STRING:
            printf("Result: %s\n", result.s);
            break;
        case FFI_ARG_VOID:
            printf("Result: void\n");
            break;
        default:
            printf("Result: %d\n", result.i);
            break;
    }
}

void print_usage(const char *program_name) {
    printf("Usage: %s <assembly_file> <function_name> <result_type> <arg1> <arg2> ...\n", program_name);
    printf("\n");
    printf("Arguments:\n");
    printf("  assembly_file  - Path to the .s assembly file\n");
    printf("  function_name  - Name of the function to call (e.g., _add_numbers)\n");
    printf("  result_type    - Type of return value (int, float, double, void, etc.)\n");
    printf("  arg1, arg2...  - Arguments to pass to the function\n");
    printf("\n");
    printf("Supported types: int, long, float, double, pointer, string, void\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s math.s _add_numbers int 5 3\n", program_name);
    printf("  %s multiply.s _multiply int 4 5\n", program_name);
    printf("  %s sqrt.s _sqrt float 16.0\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    const char *assembly_file = argv[1];
    const char *function_name = argv[2];
    const char *result_type_str = argv[3];
    
    // Calculate number of arguments
    int num_args = argc - 4;
    
    printf("Assembly Function Runner\n");
    printf("=======================\n");
    printf("Assembly file: %s\n", assembly_file);
    printf("Function name: %s\n", function_name);
    printf("Result type: %s\n", result_type_str);
    printf("Number of arguments: %d\n", num_args);
    
    // Load the assembly function
    dynamic_assembly_function_t *func = load_assembly_function(assembly_file, function_name);
    if (!func) {
        fprintf(stderr, "Failed to load assembly function\n");
        return 1;
    }
    
    printf("Successfully loaded assembly function\n");
    
    // Convert result type
    ffi_arg_type_t result_type = string_to_arg_type(result_type_str);
    
    // Prepare arguments
    universal_arg_t *args = NULL;
    if (num_args > 0) {
        args = malloc(num_args * sizeof(universal_arg_t));
        if (!args) {
            fprintf(stderr, "Failed to allocate memory for arguments\n");
            unload_assembly_function(func);
            return 1;
        }
        
        // Convert arguments to appropriate types
        // For simplicity, we'll assume all arguments are int for now
        for (int i = 0; i < num_args; i++) {
            args[i].type = FFI_ARG_INT;
            args[i].value.i = string_to_int(argv[4 + i]);
            printf("Argument %d: %d\n", i + 1, args[i].value.i);
        }
    }
    
    // Execute the function
    universal_result_t result = {0};
    int exec_result = execute_assembly_function_simple(func, result_type, &result, num_args, args);
    
    if (exec_result == 0) {
        print_result(result_type, result);
    } else {
        fprintf(stderr, "Failed to execute assembly function\n");
    }
    
    // Cleanup
    if (args) {
        free(args);
    }
    unload_assembly_function(func);
    
    return exec_result == 0 ? 0 : 1;
} 