// swap pairs of bits of a 64-bit value, using bitwise operators

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// return value with pairs of bits swapped
uint64_t bit_swap(uint64_t value) {













}


// // return value with pairs of bits swapped
// uint64_t bit_swap(uint64_t value) {

//     uint64_t newValue = 0;
//     for (int i = 0; i < (64 / 2); i++) {
//         int lshift = 2 * i;
//         uint64_t left = ((value >> (lshift + 1)) & 1);
//         uint64_t right = ((value >> (lshift)) & 1);
//         newValue |= (left << lshift);
//         newValue |= (right << (lshift + 1));

//     }

//     return newValue;
// }
