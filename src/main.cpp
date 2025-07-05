#include "arith_uint256.h"
#include "uint256.h"

#include <gmpxx.h>

#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <utility>

template<typename T>
constexpr T fast_mod_exp(const T num, T exponent, const T mod) {
    T product{1};
    T rolling_squares{num};

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

template<typename T>
constexpr T prime_modular_inverse(const T num, const T mod) {
    T exponent = mod - 2;
    return fast_mod_exp(num, exponent, mod);
}

template<typename T>
constexpr std::pair<T, T> el_gamal_encrypt(
    const T generator,
    const T modulus,
    const T message,
    const T ephemeral_key,
    const T counterparty_public_key)
{
    T c_1 = fast_mod_exp(generator, ephemeral_key, modulus);
    T c_2 = (message * fast_mod_exp(counterparty_public_key, ephemeral_key, modulus)) % modulus;
    return std::make_pair(c_1, c_2);
}

template<typename T>
constexpr T el_gamal_decrypt(
    const T modulus,
    std::pair<T, T> cypher,
    const T private_key)
{
    T x = fast_mod_exp(cypher.first, private_key, modulus);
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

    // El Gamal example from the textbook
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

    // El Gamal from zipkin
    const cpp_int pin_modulus{"15798791135088746123550015719513096545140450581792829335173644951168904821343"};
    const cpp_int pin_generator{"2"};
    const cpp_int pin_private_key{"11659842551292255865086477704221272176963931918275797345620444192196778436330"};
    const cpp_int pin_c1{"7855434657566654268873728025242363007386495727068873043180877667581048349284"};
    const cpp_int pin_c2{"7200831034730555987102729567301480124776715457374277384349240265842775915028"};
    const auto pin_cyphers{std::make_pair(pin_c1, pin_c2)};
    const cpp_int pin_message(el_gamal_decrypt(pin_modulus, pin_cyphers, pin_private_key));
    std::cout << std::hex << pin_message << std::endl;

    // El Gamal to zipkin
    const cpp_int tc_message{"0x3C332070696E68656164"};
    const cpp_int zipkin_public_key{"4153687504712048616134213761716598375904898566765967820561752172170149920951"};
    const cpp_int zipkin_modulus{"49615723209586197684920810872833175184349199614272830929806490530773652213629"};
    std::cout << std::hex << tc_message << std::endl;
    const cpp_int big_ephemeral_key = 190000007;
    const std::pair<cpp_int, cpp_int> tc_cypher = el_gamal_encrypt(pin_generator, zipkin_modulus, tc_message, big_ephemeral_key, zipkin_public_key);
    std::cout << std::dec << "tc c1: " << tc_cypher.first << " tc c2: " << tc_cypher.second << std::endl;

    // Textbook example, but with boost bigint
    const cpp_int big_modulus = 467;
    const cpp_int big_generator = 2;
    const cpp_int big_alice_private_key = 153;
    const cpp_int big_alice_public_key = fast_mod_exp(big_generator, big_alice_private_key, big_modulus);
    assert(big_alice_public_key == 224);

    const cpp_int big_message = 331;
    const std::pair<cpp_int, cpp_int> big_cypher = el_gamal_encrypt(big_generator, big_modulus, big_message, big_ephemeral_key, big_alice_public_key);
    
    const cpp_int big_decrypted_message = el_gamal_decrypt(big_modulus, big_cypher, big_alice_private_key);
    assert(big_decrypted_message == message);

    return 0;
}
