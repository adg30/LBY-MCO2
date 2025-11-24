# Car Acceleration Calculator
**by Aaron Daniel Go**

**Section:** S17B

## Project Description
This project implements a C program that interfaces with an x86-64 Assembly function to calculate the acceleration of vehicles. The program takes input parameters (Initial Velocity, Final Velocity, and Time) for multiple cars, computes the acceleration in meters per second squared ($m/s^2$), and compares the performance speed between the C implementation and the x86-64 Assembly implementation.

The project demonstrates the use of:
* **C:** Memory allocation, input/output handling, and performance timing.
* **x86-64 Assembly:** High-performance arithmetic using scalar SIMD floating-point instructions (`movsd`, `subsd`, `mulsd`, `divsd`, `roundsd`).

### The Formula
The acceleration is calculated using the formula:
$$a = \frac{V_f - V_i}{t}$$
*Where velocities are converted from km/h to m/s using the factor 0.277778.*

---

## Performance Analysis
We compared the execution time of the C function vs. the x86-64 Assembly function across different input sizes ($N$). The results below represent the average time per run over 30 iterations.

### Execution Results

| Input Size (N) | Assembly Time (avg) | C Time (avg) | Speedup Factor |
| :--- | :--- | :--- | :--- |
| **10 cars** | 0.1067 μs | 0.1233 μs | **1.16x Faster** |
| **100 cars** | 0.1500 μs | 0.4600 μs | **3.07x Faster** |
| **1,000 cars** | 1.2033 μs | 4.3033 μs | **3.58x Faster** |
| **10,000 cars** | 16.1867 μs | 103.4333 μs | **6.39x Faster** |

> *Note: Times are in microseconds (μs).*

### Screenshots of 10 and 10000's execution
<img width="457" height="567" alt="image" src="https://github.com/user-attachments/assets/aa164579-4976-477f-9aae-d604c1fdd737" />

<img width="469" height="562" alt="image" src="https://github.com/user-attachments/assets/34c666fd-f7e6-4adf-bafd-fe2adaa25213" />

### Analysis
As discussed in the demonstration video, the x86-64 Assembly implementation consistently outperformed the C implementation, achieving a massive **6.39x speedup** at 10,000 inputs. The reasons for this performance gap are:
1.  **Direct Instruction Control:** In Assembly, we have absolute control over every instruction executed. We explicitly selected efficient hardware instructions (like `roundsd` for rounding) rather than relying on the compiler's translation. In contrast, the C compiler may make generalized optimization choices or call heavy library functions that are not strictly necessary for this specific logic.
2.  **Lean Execution:** The Assembly implementation performs *only* the necessary operations (load, compute, convert, store). It strips away the overhead found in C, such as standard library safety checks (e.g., handling Infinity/NaNs) and complex stack frame management.
3.  **Compounding Efficiency:** The performance difference is non-linear. While the Assembly is only slightly faster at small inputs (1.16x for 10 cars), the efficiency gains compound as the dataset grows, resulting in the drastic 6.39x speedup observed for 10,000 cars.



---

## Correctness Check
The program verifies the output against a predefined dataset and compares the Assembly output against the C output for every random test run.

**Screenshot of Correctness Output:**

<img width="569" height="344" alt="image" src="https://github.com/user-attachments/assets/d9b7625b-e36b-49d6-8aaa-d69f62cc9d85" />


---

## Demonstration Video
Here is a short video demonstrating the source code, compilation process, and program execution:

[**Link to Demo Video**](https://drive.google.com/file/d/1wpeax_gJAZQ_BukoH5hKcjo9BhCHpPLF/view?usp=sharing)
