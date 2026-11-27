#include <ctime>
#include <fstream>
using namespace std;

const int ROWS = 20480;
const int COLS = 4096;
const int TRIALS = 10;

// Each row is 4096 bytes, matching the UWF server's page size (use getconf PAGESIZE).
// Total matrix size: 83,886,080 bytes (80 MiB).
// 1 KiB = 1024 bytes.
// 1 MiB = 1024^2 bytes.
char matrix[ROWS][COLS]; // A char unlike an int occupies a single byte rather than four bytes. 

// Spatial locality: accessing nearby memory locations
// within a short period of time.

// TLB (Translation Lookaside Buffer): a small, fast cache
// that stores page-to-frame translations to avoid page table lookups.

// References: Our textbook, Section 10.9.5 was a big help, pages 433-434.

// Write horizontally across each row.
// C++ stores multidimensional arrays in row-major order.
// Accessing consecutive bytes improves spatial locality
// and can reduce TLB misses and page faults.
void writeRow() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            matrix[row][col] = 'A'; // Arbitruary re-write value. 
        }
    }
}

// Write vertically down each column.
// Each downward access jumps 4096 bytes instead of 1,
// resulting in poorer spatial locality.
void writeCol() {
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS; row++) {
            matrix[row][col] = 'A';
        }
    }
}

// Read horizontally across each row.
// value is deliberately unused, so void prevents compile warning
void readRow() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            char value = matrix[row][col];
            (void)value;
        }
    }
}

// Read vertically down each column.
// Same op as readRow, but differing order.
void readCol() {
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS; row++) {
            char value = matrix[row][col];
            (void)value;
        }
    }
}

// Runs a function TRIAL times and returns average seconds per run.
double averageSeconds(void (*func)()) {
    timespec start, finish;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int trial = 0; trial < TRIALS; trial++) {
        func();
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);

    double elapsed = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1e9;
    return elapsed/TRIALS;
}

int main() {
    struct Experiment {
        const char* name;
        void (*func)();
    };

    const Experiment experiments[] = {
        {"Write Row", writeRow},
        {"Write Col", writeCol},
        {"Read Row", readRow},
        {"Read Col", readCol},
    };

    ofstream results("results.txt");
    for (const Experiment& e : experiments) {
        results << e.name << ": " << averageSeconds(e.func) << " s\n";
    }

    return 0;
}