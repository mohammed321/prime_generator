#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define CANDIDATES_TO_CHECK (1000000)
#define WITNESSES_TO_CHECK (100)
#define number uint64_t


// calculate x ^ e (mod m)
number mod_exp(number x, number e, number m)
{
    if (m == 1)
    {
        return 0;
    }

    number res = 1;
    x = x % m;
    while (e > 0)
    {
        if (e & 0x1)
        {
            res = (res * x) % m;
        }
        e = e >> 1;
        x = (x * x) % m;
    }
    return res;
}

// get number in [min,max)
number get_number(number min, number max)
{
    return (rand() % (max - min)) + min;
}

// one of the conditions of the Miller–Rabin primality test
bool _prime_condition(number x, number s, number prime_candidate)
{
    for (int i = 0; i < s; i++)
    {
        x = (x * x) % prime_candidate;
        if (x == (prime_candidate - 1))
        {
            return true;
        }
    }
    return false;
}

// Miller–Rabin primality test
bool is_composite(number prime_candidate)
{
    if (prime_candidate == 2) return false;
    // if number is even
    if ((prime_candidate & 0x1) == 0) return true;

    // find s and d in the equation prime_candidate = (2^s)*d + 1, two_s = 2^s 
    number two_s = (prime_candidate - 1) & (-(prime_candidate - 1));
    number d = (prime_candidate - 1) / two_s;
    number s = 0;
    while (two_s > 1)
    {
        s++;
        two_s >>= 1;
    }

    for (int i = 0; i < WITNESSES_TO_CHECK; i++)
    {
        number a = get_number(2, prime_candidate);
        number x = mod_exp(a, d, prime_candidate);
        if (x == 1 || x == (prime_candidate - 1))
            continue;
        if (_prime_condition(x, s, prime_candidate))
            continue;
        // printf("%u is not prime\n", prime_candidate);
        return true;
    }
    return false;
}

// return a prime number between [min,max)
number get_prime(number min, number max)
{
    for (int i = 0; i < CANDIDATES_TO_CHECK; i++)
    {
        number n = get_number(min, max);
        if (!is_composite(n))
        {
            return n;
        }
    }
    return 0;
}

int main()
{
    // seed the random function with the current time
    srand(time(NULL));

    for (int i = 2; i < 30; i++)
    {
        number min = 1 << i;
        number p = get_prime(min, min << 1);
        if (p == 0)
        {
            printf("no primes found between 2^%d and 2^%d\n", i, (i + 1));
        }
        else
        {
            printf("between 2^%d and 2^%d, found the prime %u \n", i, (i + 1), p);
        }
    }
}