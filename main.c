// main.c - Complete Car Acceleration Calculator
// by Aaron Daniel Go

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>

// assembly function
extern void calculate_accel(int num_cars, double* input, int* output);

// C implementation of the same function (for comparison)
void calculate_accel_c(int num_cars, double* input, int* output) {
    for (int i = 0; i < num_cars; i++) {
        // Get Vi, Vf, Time for current car
        double vi = input[i * 3 + 0];  
        double vf = input[i * 3 + 1];  
        double t = input[i * 3 + 2];  
        
        // Calculate acceleration: (Vf - Vi) / T, with km/h to m/s conversion
        double diff = vf - vi;                      
        double diff_ms = diff * 0.2777777778;  
        double accel = diff_ms / t;                 
        
        // Round to nearest integer 
        output[i] = (int)round(accel);
    }
}

// Generate random double between min and max
double random_double(double min, double max) {
    return min + (rand() / (double)RAND_MAX) * (max - min);
}

//  timer using windows QueryPerformanceCounter since normal timers are not precise enough and gives 0s 
double get_time_microseconds() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);  // get timer frequency
    QueryPerformanceCounter(&counter);      // get current counter value
    return (double)counter.QuadPart / (double)frequency.QuadPart * 1000000.0;
}


// CORRECTNESS CHECK - Uses predefined values from the spec

void check_correctness() {
    printf("\n");
    printf("=============================================\n");
    printf("  CORRECTNESS CHECK (Predefined Values)\n");
    printf("=============================================\n");
    
    int num_cars = 3;
    
    // Sample data from the project specification
    double input[9] = {
        0.0, 62.5, 10.1,     // Car 1: Expected output = 2
        60.0, 122.3, 5.5,    // Car 2: Expected output = 3
        30.0, 160.7, 7.8     // Car 3: Expected output = 5
    };
    
    int output_asm[3] = {0};
    int output_c[3] = {0};
    int expected[3] = {2, 3, 5};
    
    // Call both functions
    calculate_accel(num_cars, input, output_asm);     // Assembly
    calculate_accel_c(num_cars, input, output_c);     // C
    
    printf("\nInput Data (Vi, Vf, T):\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < num_cars; i++) {
        printf("Car %d: Vi=%.1f km/h, Vf=%.1f km/h, T=%.1f s\n",
               i + 1, input[i*3], input[i*3+1], input[i*3+2]);
        printf("  Assembly: %d m/s^2 | C: %d m/s^2 | Expected: %d m/s^2",
               output_asm[i], output_c[i], expected[i]);
        
        if (output_asm[i] == expected[i] && output_c[i] == expected[i]) {
            printf(" [PASS]\n");
        } else {
            printf(" [FAIL]\n");
        }
    }
    
    printf("------------------------------------------------\n");
    
    // Summary
    int pass = 1;
    for (int i = 0; i < num_cars; i++) {
        if (output_asm[i] != expected[i] || output_c[i] != expected[i]) {
            pass = 0;
            break;
        }
    }
    
    if (pass) {
        printf("[YES] CORRECTNESS CHECK PASSED!\n");
    } else {
        printf("[NO] CORRECTNESS CHECK FAILED!\n");
    }
}


// USER INPUT TEST - Allows manual testing with custom values

void run_user_input_test() {
    printf("\n");
    printf("=============================================\n");
    printf("  USER INPUT TEST\n");
    printf("=============================================\n");
    
    int num_cars;
    printf("Enter number of cars: ");
    if (scanf("%d", &num_cars) != 1 || num_cars <= 0) {
        printf("Invalid input!\n");
        return;
    }
    
    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // Allocate memory
    double* input = (double*)malloc(num_cars * 3 * sizeof(double));
    int* output_asm = (int*)malloc(num_cars * sizeof(int));
    int* output_c = (int*)malloc(num_cars * sizeof(int));
    
    if (!input || !output_asm || !output_c) {
        printf("Memory allocation failed!\n");
        free(input);
        free(output_asm);
        free(output_c);
        return;
    }
    
    // Get input for each car
    printf("\nEnter data for each car (Vi, Vf, T):\n");
    printf("Format: Vi,Vf,T (separated by commas)\n");
    printf("Example: 0.0,62.5,10.1\n\n");
    
    for (int i = 0; i < num_cars; i++) {
        printf("Car %d: ", i + 1);
        char line[256];
        if (!fgets(line, sizeof(line), stdin)) {
            printf("Input error!\n");
            free(input);
            free(output_asm);
            free(output_c);
            return;
        }
        
        // Replace commas with spaces for easier parsing
        for (char* p = line; *p; p++) {
            if (*p == ',') *p = ' ';
        }
        
        if (sscanf(line, "%lf %lf %lf",
                   &input[i*3], &input[i*3+1], &input[i*3+2]) != 3) {
            printf("Invalid format! Please use: Vi,Vf,T\n");
            i--;  // Retry this car
        }
    }
    
    // Calculate using both methods
    calculate_accel(num_cars, input, output_asm);
    calculate_accel_c(num_cars, input, output_c);
    
    // Display results
    printf("\n--- Results ---\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < num_cars; i++) {
        printf("Car %d: Vi=%.1f, Vf=%.1f, T=%.1f\n",
               i + 1, input[i*3], input[i*3+1], input[i*3+2]);
        printf("  Assembly: %d m/s^2 | C: %d m/s^2",
               output_asm[i], output_c[i]);
        
        if (output_asm[i] == output_c[i]) {
            printf(" [[YES] Match]\n");
        } else {
            printf(" [[NO] Mismatch]\n");
        }
    }
    printf("------------------------------------------------\n");
    
    // Clean up
    free(input);
    free(output_asm);
    free(output_c);
}


// PERFORMANCE TEST - Benchmarks with random values (30 runs each)

void run_performance_test(int num_cars) {
    printf("\n");
    printf("=============================================\n");
    printf("  PERFORMANCE TEST: %d cars\n", num_cars);
    printf("=============================================\n");
    
    // Allocate memory
    double* input = (double*)malloc(num_cars * 3 * sizeof(double));
    int* output_asm = (int*)malloc(num_cars * sizeof(int));
    int* output_c = (int*)malloc(num_cars * sizeof(int));
    
    if (!input || !output_asm || !output_c) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    // Generate random test data
    printf("Generating random test data...\n");
    for (int i = 0; i < num_cars; i++) {
        input[i * 3 + 0] = random_double(0.0, 100.0);    // Vi: 0-100 km/h
        input[i * 3 + 1] = random_double(50.0, 200.0);   // Vf: 50-200 km/h
        input[i * 3 + 2] = random_double(3.0, 15.0);     // Time: 3-15 seconds
    }
    
    // Show sample data (first 3 cars)
    printf("\nSample Input (first 3 cars):\n");
    int display = (num_cars < 3) ? num_cars : 3;
    for (int i = 0; i < display; i++) {
        printf("  Car %d: Vi=%.2f km/h, Vf=%.2f km/h, T=%.2f s\n",
               i + 1, input[i*3], input[i*3+1], input[i*3+2]);
    }
    
    
    // TIME ASSEMBLY (30 runs)
    
    printf("\n--- Assembly Implementation ---\n");
    double total_time_asm = 0.0;
    
    for (int run = 0; run < 30; run++) {
        double start = get_time_microseconds();
        calculate_accel(num_cars, input, output_asm);
        double end = get_time_microseconds();
        total_time_asm += (end - start);
    }
    
    double avg_time_asm = total_time_asm / 30.0;
    printf("Total time (30 runs): %.2f microseconds\n", total_time_asm);
    printf("Average time per run: %.4f microseconds\n", avg_time_asm);
    
    
    // TIME C (30 runs)
    
    printf("\n--- C Implementation ---\n");
    double total_time_c = 0.0;
    
    for (int run = 0; run < 30; run++) {
        double start = get_time_microseconds();
        calculate_accel_c(num_cars, input, output_c);
        double end = get_time_microseconds();
        total_time_c += (end - start);
    }
    
    double avg_time_c = total_time_c / 30.0;
    printf("Total time (30 runs): %.2f microseconds\n", total_time_c);
    printf("Average time per run: %.4f microseconds\n", avg_time_c);
    
    
    // COMPARISON
    
    printf("\n--- Performance Comparison ---\n");
    if (avg_time_asm < avg_time_c) {
        double speedup = avg_time_c / avg_time_asm;
        printf("[YES] Assembly is FASTER by %.2fx\n", speedup);
        printf("  Assembly: %.4f us | C: %.4f us\n", avg_time_asm, avg_time_c);
    } else if (avg_time_c < avg_time_asm) {
        double speedup = avg_time_asm / avg_time_c;
        printf("[YES] C is FASTER by %.2fx\n", speedup);
        printf("  Assembly: %.4f us | C: %.4f us\n", avg_time_asm, avg_time_c);
    } else {
        printf("Both have similar performance\n");
    }
    
    // Verify correctness (outputs should match)
    printf("\n--- Verification ---\n");
    int mismatches = 0;
    for (int i = 0; i < num_cars; i++) {
        if (output_asm[i] != output_c[i]) {
            mismatches++;
            if (mismatches <= 3) {  // Show first 3 mismatches
                printf("Mismatch at car %d: Asm=%d, C=%d\n",
                       i + 1, output_asm[i], output_c[i]);
            }
        }
    }
    
    if (mismatches == 0) {
        printf("[YES] All %d results match!\n", num_cars);
    } else {
        printf("[NO] Found %d mismatches out of %d\n", mismatches, num_cars);
    }
    
    // Show sample outputs
    printf("\nSample Outputs (first 3 cars):\n");
    for (int i = 0; i < display; i++) {
        printf("  Car %d: Assembly=%d m/s^2, C=%d m/s^2",
               i + 1, output_asm[i], output_c[i]);
        if (output_asm[i] == output_c[i]) {
            printf(" [[YES]]\n");
        } else {
            printf(" [[NO]]\n");
        }
    }
    
    // Clean up
    free(input);
    free(output_asm);
    free(output_c);
}


// MAIN FUNCTION

int main() {
    // Initialize random seed
    srand((unsigned int)time(NULL));
    
    printf("=============================================\n");
    printf("  Car Acceleration Calculator\n");
    printf("  C vs Assembly Performance Comparison\n");
    printf("=============================================\n");
    
    // STEP 1: Correctness Check (predefined values from spec)
    check_correctness();
    
    // STEP 2: User Input Test (optional)
    printf("\n\nWould you like to test with custom input? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    
    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (choice == 'y' || choice == 'Y') {
        run_user_input_test();
    }
    
    // STEP 3: Performance Benchmarks (random values, 30 runs each)
    printf("\n\n");
    printf("=============================================\n");
    printf("  PERFORMANCE BENCHMARKS\n");
    printf("=============================================\n");
    
    run_performance_test(10);
    run_performance_test(100);
    run_performance_test(1000);
    run_performance_test(10000);
    
    printf("\n");
    printf("=============================================\n");
    printf("  All tests completed!\n");
    printf("=============================================\n");
    
    return 0;
}