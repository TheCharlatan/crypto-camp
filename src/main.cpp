#include <cstdint>
#include <cstdio>

constexpr int fast_mod_exp(const uint64_t num, const uint64_t exponent, const uint64_t mod) {
    uint64_t exp{exponent};
    uint64_t product{1};
    uint64_t rolling_squares{num};
    uint64_t counter{0};

    /*
     * While exp is not zero:
     * 1. Check if it the least significant bit 1, then:
     *   1.1 Apply the rolling square to the procut
     * 2. Compute the rolling squares of the number 
     * 3. Left shift the exponent to eliminate the least significant bit
     */
    while (exp != 0) {
        if (exp & 1) {
            product = (product * rolling_squares) % mod;
        }
        counter++;
        rolling_squares = (rolling_squares * rolling_squares) % mod;
        exp = exp >> 1;
    }

    return product;
}

int main() {
    constexpr uint64_t num{3};
    constexpr uint64_t exponent{218};
    constexpr uint64_t mod{1000};
    constexpr uint64_t product{fast_mod_exp(num, exponent, mod)};

    std::printf("%ld to the power of %ld mod %ld is %ld\n", num, exponent, mod, product);
    return 0;
}
