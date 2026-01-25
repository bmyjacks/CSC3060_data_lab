#include "data_lab.hpp"
#include <cstdint>

namespace data_lab {

int32_t add(int32_t a, int32_t b) {

    int32_t P = a ^ b;
    int32_t G = a & b;

    int32_t temp_P = P;

    G |= (G << 1) & P;
    P &= P << 1;
    G |= (G << 2) & P;
    P &= P << 2;
    G |= (G << 4) & P;
    P &= P << 4;
    G |= (G << 8) & P;
    P &= P << 8;
    G |= (G << 16) & P;

    return temp_P ^ (G << 1);

}

int32_t subtract(int32_t a, int32_t b) {
    
    // a minus b is equivalent to a plus -b
    // Use 2-complement to get -b
    return add(a, add(~b, 1));

}

int32_t multiply(int32_t a, int32_t b) {

    int32_t result = 0;
    
    for (int32_t i = 0; i < 32; ++i) {
        if (1 & b) {
            result = add(result, a);
        }
        a <<= 1;
        b >>= 1;
    }

    return result;

}

int32_t divide(int32_t a, int32_t b) {

    int32_t result = 0;

    // Check for sign of quotient
    // Operator priority: >>, &, ^; from high to low
    int32_t minus = a >> 31 & 1 ^ b >> 31 & 1;

    // Take absolute values
    if (a >> 31 & 1) a = add(~a, 1);
    if (b >> 31 & 1) b = add(~b, 1);
    
    for (int32_t i = 0; i < 32; ++i) {

        // Check whether divisor's multiplier > dividend
        // In case of overflow, we shrink the dividend instead of expanding the divisor
        if (subtract(a >> subtract(31, i), b) >> 31 & 1 ^ 1) {
            a = subtract(a, b << subtract(31, i));     // Reminder
            result |= 1 << subtract(31, i);     // Add corresponding value to quotient
        }

    }

    // Restore sign of quotient
    minus && (result = add(~result, 1));

    return result;

}

int32_t modulo(int32_t a, int32_t b) {
    
    // a mod b = a - (a / b) * b
    return subtract(a, multiply(divide(a, b), b));

}

}  // namespace data_lab