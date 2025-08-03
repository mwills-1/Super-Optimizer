#include <stdio.h>
#include "dynamic_assembly.h"

// =============================================================================
// Load File to Function
// =============================================================================

int compile_assembly_file(const char *, const char *);
int create_shared_library(const char *, const char *);
dynamic_assembly_function_t *load_assembly_function(const char *, const char *);
