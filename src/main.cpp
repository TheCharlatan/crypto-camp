#include <cassert>
#include <cstdint>
#include <cstdio>
#include <utility>

constexpr uint64_t fast_mod_exp(const uint64_t num, uint64_t exponent, const uint64_t mod) {
    uint64_t product{1};
    uint64_t rolling_squares{num};

    /*
     * While exp is not zero:
     * 1. Check if it the least significant bit 1, then:
     *   1.1 Apply the rolling square to the product
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

constexpr std::pair<uint64_t, uint64_t> el_gamal_encrypt(
    const uint64_t generator,
    const uint64_t modulus,
    const uint64_t message,
    const uint64_t ephemeral_key,
    const uint64_t counterparty_public_key)
{
    uint64_t c_1 = fast_mod_exp(generator, ephemeral_key, modulus);
    uint64_t c_2 = message * fast_mod_exp(counterparty_public_key, ephemeral_key, modulus) % modulus;
    return std::make_pair(c_1, c_2);
}

constexpr uint64_t el_gamal_decrypt(
    const uint64_t modulus,
    std::pair<uint64_t, uint64_t> cypher,
    const uint64_t private_key)
{
    uint64_t x = fast_mod_exp(cypher.first, private_key, modulus);
    return (cypher.second * prime_modular_inverse(x, modulus)) % modulus;
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

    constexpr uint64_t modulus = 467;
    constexpr uint64_t generator = 2;
    constexpr uint64_t alice_private_key = 153;
    constexpr uint64_t alice_public_key = fast_mod_exp(generator, alice_private_key, modulus);
    assert(alice_public_key == 224);

    constexpr uint64_t message = 331;
    constexpr uint64_t ephemeral_key = 197;
    constexpr auto cypher = el_gamal_encrypt(generator, modulus, message, ephemeral_key, alice_public_key);
    
    constexpr uint64_t decrypted_message = el_gamal_decrypt(modulus, cypher, alice_private_key);
    assert(decrypted_message == message);

    return 0;
}
