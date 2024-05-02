#include <iostream>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>

using namespace std;
using namespace NTL;

// (base^exp) % mod
ZZ power_mod(ZZ base, long exp, ZZ mod) {
    ZZ result;
    PowerMod(result, base, exp, mod);
    return result;
}

/**
 * Fermat's Little Theorem is used here
*/
ZZ_p find_primitive_element_GF_p(long p) {
    ZZ_p::init(ZZ(p)); // Initialize finite field GF(p)

    for (long alpha = 2; alpha < p; ++alpha) {  // Iterate over possible primitive elements {2, ..., p-1}
        bool is_primitive = true;

        // Check if alpha^(p-1) ≡ 1 (mod p)
        ZZ exp_result = power_mod(ZZ(alpha), p-1, ZZ(p));
        if (exp_result != 1)
            continue;

        // Check if alpha^k ≠ 1 (mod p) for all k < p-1
        for (long k = 1; k < p-1; ++k) {
            ZZ k_exp_result = power_mod(ZZ(alpha), k, ZZ(p));
            if (k_exp_result == 1) {
                is_primitive = false;
                break;
            }
        }

        if (is_primitive)
            return ZZ_p(alpha);
    }

    cerr << "Error: No primitive element found in GF(" << p << ")" << endl;
    exit(1);
}

void parsePrimitivePower(const ZZ& p, ZZ& tBase, ZZ& tExp) {
    tBase = ZZ(-1);
    tExp = ZZ(-1);

    for (ZZ base = ZZ(2); base < p; base = NextPrime(base + 1)) {
        ZZ tempP = p;
        ZZ tempExp = ZZ(0);
        while(divide(tempP, base)) {
            tempP /= base;
            tempExp++;
        }

        if(tempP == 1) {
            tBase = base;
            tExp = tempExp;
            return;
        }
    }
}

int main() {
    long p;
    cout << "Enter a prime number p or prime power p^k (example: 9 (3^2), 25(5^2), ...): ";
    cin >> p;

    if(ProbPrime(ZZ(p))) {
        ZZ_p primitiveElement;
        (p == 2) ? primitiveElement = 1 : primitiveElement = find_primitive_element_GF_p(p);
        cout << "Primitive element in GF(" << p << "): " << primitiveElement << endl;
    } else {
        ZZ base, exp;
        parsePrimitivePower(ZZ(p), base, exp);
        if(base == -1 || exp == -1) {
            cout << "error: p must be a prime number or a prime power!" << endl;
            return -1;
        }

        cout << base << "^" << exp << endl;
    }

    return 0;
}