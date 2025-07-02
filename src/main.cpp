#include <cassert>
#include <cstdint>
#include <cstdio>

constexpr uint64_t fast_mod_exp(const uint64_t num, uint64_t exponent, const uint64_t mod) {
    uint64_t product{1};
    uint64_t rolling_squares{num};

    /*
     * While exp is not zero:
     * 1. Check if it the least significant bit 1, then:
     *   1.1 Apply the rolling square to the procut
     * 2. Compute the rolling squares of the number 
     * 3. Left shift the exponent to eliminate the least significant bit
     */
    while (exponent != 0) {
        if (exponent & 1) {
            product = (product * rolling_squares) % mod;
        }
        rolling_squares = (rolling_squares * rolling_squares) % mod;
        exponent = exponent >> 1;
    }

    return product;
}

constexpr uint64_t prime_modular_inverse(const uint64_t num, const uint64_t mod) {
    return fast_mod_exp(num, mod - 2, mod);
}

int main() {
    constexpr uint64_t num{3};
    constexpr uint64_t exponent{218};
    constexpr uint64_t mod{1000};
    constexpr uint64_t product{fast_mod_exp(num, exponent, mod)};
    assert(product == 489);
    constexpr uint64_t product_2{fast_mod_exp(4, 217, 1004)};
    assert(product_2 == 492);

    constexpr uint64_t prime_mod{13};
    // Do a quick and dirty insufficient primality test
    assert(fast_mod_exp(num, prime_mod, prime_mod) == num);
    constexpr uint64_t inverse{prime_modular_inverse(num, prime_mod)};
    assert(((inverse * num) % 13) == 1);

    std::printf("%ld to the power of %ld mod %ld is %ld\n", num, exponent, mod, product);
    std::printf("The inverse of %ld under the prime modulus %ld is %ld\n", num, prime_mod, inverse);
    return 0;
}
