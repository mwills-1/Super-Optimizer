# Super Optimizer Testing Framework

A comprehensive testing framework for validating assembly functions against reference implementations. This framework is designed to help you test super-optimized assembly code against known correct implementations.

## Features

- **Test Suite Management**: Organize tests into suites and cases
- **Assembly Function Testing**: Test assembly functions against reference implementations
- **Sandboxed Execution**: Execute assembly code safely with timeouts and memory limits
- **Comprehensive Reporting**: Detailed test results with timing and statistics
- **Flexible Test Vectors**: Support for various input types and edge cases
- **Parallel Execution**: Optional parallel test execution for faster results

## Project Structure

```
superop/
├── test_program/
│   ├── test.h          # Test framework header
│   ├── test.c          # Main test framework implementation
│   └── assembly_tester.c # Example assembly function tester
├── claude/
│   └── superop.c       # Super optimizer implementation
├── Makefile            # Build configuration
└── README.md           # This file
```

## Quick Start

### 1. Build the Framework

```bash
# Build everything
make all

# Or build specific components
make test_runner
make superop
```

### 2. Run Tests

```bash
# Run the main test framework
make test

# Run the assembly function tester
./test_runner

# Run the super optimizer demo
make demo
```

### 3. Clean Build Artifacts

```bash
make clean
```

## Usage Examples

### Testing Assembly Functions

The framework provides a robust way to test assembly functions against reference implementations:

```c
// Define your assembly function
int my_assembly_function(int a, int b) {
    // Your assembly code here
    return a + b;
}

// Define reference implementation
int reference_function(int a, int b) {
    return a + b;
}

// Create test case
test_case_t *test = test_case_create("My Function", "Test my assembly function", test_assembly_function);

// Add test vectors
int inputs[] = {5, 3};
int expected[] = {8};
program_t prog = {NULL, 0, 0}; // Your actual assembly program

test_case_add_vector(test, "Basic test", 1, prog, inputs, expected, (void*)reference_function);
```

### Creating Test Suites

```c
// Create test runner
test_runner_t *runner = test_runner_create();

// Create test suite
test_suite_t *suite = test_suite_create("Arithmetic", "Test arithmetic operations");

// Add test cases to suite
test_suite_add_case(suite, test_case);

// Add suite to runner
suite->next = runner->suites;
runner->suites = suite;

// Run all tests
run_all_tests(runner);
```

### Custom Test Functions

You can create custom test functions for specific needs:

```c
test_result_t my_custom_test(test_case_t *tc) {
    // Your test logic here
    for (size_t i = 0; i < tc->vector_count; i++) {
        test_vector_t *vec = &tc->vectors[i];
        
        // Execute your assembly function
        // Compare with expected results
        // Report pass/fail
    }
    
    return TEST_PASS; // or TEST_FAIL, TEST_ERROR, TEST_SKIP
}
```

## Framework Components

### Test Runner (`test_runner_t`)

The main orchestrator that manages test execution:

- **Configuration**: Verbose output, stop on failure, parallel execution
- **Statistics**: Track total tests, passes, failures, timing
- **Output**: Console, XML, or JSON reporting

### Test Suite (`test_suite_t`)

Groups related test cases:

- **Organization**: Logical grouping of tests
- **Setup/Teardown**: Suite-level initialization and cleanup
- **Statistics**: Suite-level results tracking

### Test Case (`test_case_t`)

Individual test with multiple test vectors:

- **Test Function**: The actual test logic
- **Test Vectors**: Multiple input/output combinations
- **Setup/Teardown**: Test-specific initialization
- **Results**: Pass/fail status, timing, error messages

### Test Vector (`test_vector_t`)

Individual input/output combination:

- **Inputs**: Test input data
- **Expected Output**: Expected results
- **Assembly Program**: The assembly code to test
- **Custom Data**: Additional test-specific data

## Assembly Function Testing

The framework provides specialized support for testing assembly functions:

### Safe Execution

Assembly functions are executed in a sandboxed environment:

- **Process Isolation**: Each test runs in a separate process
- **Timeout Protection**: Configurable execution time limits
- **Memory Limits**: Prevent runaway memory usage
- **Error Handling**: Graceful handling of crashes and errors

### Reference Implementation Testing

Compare assembly functions against known correct implementations:

```c
// Test assembly function against reference
test_result_t test_assembly_against_reference(test_case_t *tc) {
    for (size_t i = 0; i < tc->vector_count; i++) {
        test_vector_t *vec = &tc->vectors[i];
        
        // Execute reference function
        reference_func_t ref_func = (reference_func_t)vec->custom_data;
        ref_func(vec->inputs, ref_output);
        
        // Execute assembly function
        execute_assembly_safely(&vec->test_program, vec->inputs, asm_output, 1000.0, 10);
        
        // Compare results
        if (memcmp(asm_output, ref_output, output_size) == 0) {
            // Test passed
        } else {
            // Test failed
        }
    }
}
```

## Configuration Options

### Test Runner Configuration

```c
test_runner_t *runner = test_runner_create();

runner->verbose = 1;                    // Enable verbose output
runner->stop_on_first_failure = 0;     // Continue after failures
runner->parallel_execution = 0;         // Sequential execution
runner->output_format = "console";      // Output format
runner->random_seed = 12345;           // Reproducible tests
```

### Test Execution Options

- **Timeout**: Maximum execution time per test (default: 1000ms)
- **Memory Limit**: Maximum memory usage per test (default: 10MB)
- **Sandboxing**: Process isolation and resource limits
- **Error Reporting**: Detailed error messages and stack traces

## Advanced Features

### Parallel Test Execution

Enable parallel execution for faster test runs:

```c
runner->parallel_execution = 1;
```

### Custom Output Formats

Support for different output formats:

- **Console**: Human-readable output
- **XML**: Machine-readable XML format
- **JSON**: JSON format for integration

### Test Vector Generation

Automated generation of test vectors:

```c
// Generate edge case test vectors
test_input_t* generate_edge_case_vectors(size_t *count) {
    int values[] = {0, 1, -1, INT_MAX, INT_MIN, 0x80000000, 0x7FFFFFFF};
    // Generate all combinations
}

// Generate random test vectors
test_input_t* generate_random_vectors(size_t count, uint32_t seed) {
    srand(seed);
    // Generate random inputs
}
```

### Performance Testing

Measure and compare performance:

```c
double start_time = get_time_ms();
// Execute function
double end_time = get_time_ms();
double execution_time = end_time - start_time;
```

## Integration with Super Optimizer

The framework integrates with your super optimizer:

1. **Generate Assembly**: Use your super optimizer to generate assembly functions
2. **Create Test Cases**: Define test cases with reference implementations
3. **Execute Tests**: Run the assembly functions against test vectors
4. **Validate Results**: Ensure the optimized code produces correct results
5. **Performance Analysis**: Measure and compare performance improvements

## Example Workflow

1. **Define Target Function**: Create a reference implementation
2. **Generate Test Vectors**: Create comprehensive test inputs
3. **Run Super Optimizer**: Generate optimized assembly code
4. **Execute Tests**: Test the assembly against reference implementation
5. **Validate Results**: Ensure correctness and measure performance
6. **Iterate**: Refine the optimization process

## Building and Running

### Prerequisites

- GCC compiler
- Standard C library
- POSIX-compliant system (Linux, macOS, etc.)

### Build Commands

```bash
# Build everything
make all

# Build test runner only
make test_runner

# Build super optimizer only
make superop

# Run tests
make test

# Run demo
make demo

# Clean build artifacts
make clean

# Show help
make help
```

### Running Tests

```bash
# Run all tests
./test_runner

# Run with specific configuration
./test_runner --verbose --stop-on-failure

# Run specific test suite
./test_runner --suite "Arithmetic"
```

## Contributing

To extend the framework:

1. **Add New Test Types**: Create new test function types
2. **Extend Test Vectors**: Support new input/output formats
3. **Add Output Formats**: Implement new reporting formats
4. **Enhance Sandboxing**: Improve security and isolation
5. **Performance Optimizations**: Optimize test execution speed

## License

This project is provided as-is for educational and research purposes.

## Support

For questions or issues:

1. Check the example code in `test_program/assembly_tester.c`
2. Review the test framework implementation in `test_program/test.c`
3. Examine the super optimizer in `claude/superop.c`
4. Use the Makefile for building and running tests # Super-Optimizer
