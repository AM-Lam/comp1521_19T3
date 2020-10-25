#include "bit_rotate.h"

// return the value bits rotated left n_rotations
uint16_t bit_rotate(int n_rotations, uint16_t bits) {
    return 42; //REPLACE ME WITH YOUR CODE
}
















// // return the value bits rotated left n_rotations
// uint16_t bit_rotate(int n_rotations, uint16_t bits) {

//     char arrOfBits[16];
//     for (int i = 0; i < 16; i++) {
//         int shift = (n_rotations > 0) ? (i + n_rotations) % 16 : i + (16 + n_rotations) % 16;
//         if ((i - shift) < 0) {
//             shift %= 16;
//             arrOfBits[shift] = (bits << i) & 1;
//         } 

//         if (shift < 0) {
//             shift += 15;
//         }

//         arrOfBits[shift] = (bits >> i) & 1;
//     }

//     uint16_t newBits = 0;
//     for (int j = 0; j < 16; j++) {
//         newBits |= (arrOfBits[j] << j);
//     }

//     return newBits; 

// }

