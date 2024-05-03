#include <iostream>
#include <vector>

#include <NTL/ZZ.h>
#include <NTL/ZZ_pE.h>
#include <NTL/ZZ_pXFactoring.h>

using namespace std;
using namespace NTL;

/**
 * Fermat's Little Theorem is used here
*/
std::vector<ZZ_p> findPrimitiveElements(long p) {
    if(p == 2) return {conv<ZZ_p>(1)};

    ZZ_p::init(conv<ZZ>(p));
    std::vector<ZZ_p> allPrimElements;

    for(long alpha = 2; alpha < p; alpha++) {
        bool isPrimitive = true;
        ZZ_p potPrimElem = conv<ZZ_p>(alpha);

        if(power(potPrimElem, p - 1) != 1)
            continue;

        for(long k = 2; k < p - 1; k++) {
            if(power(potPrimElem, k) == 1) {
                isPrimitive = false;
                break;
            }
        }

        if(isPrimitive)
            allPrimElements.push_back(potPrimElem);
    }

    return allPrimElements;
}

int caclPrimitivePower(const ZZ& p, ZZ& tBase, ZZ& tExp) {
    for(ZZ prime = conv<ZZ>(2); prime < p; prime = NextPrime(prime + 1)) {
        ZZ tempP = p;
        ZZ tempExp = ZZ(0);
        while(divide(tempP, prime)) {
            tempP /= prime;
            tempExp++;
        }

        if(tempP == 1) {
            tBase = prime;
            tExp = tempExp;
            return 0;
        }
    }

    return 1;
}

std::vector<ZZ_pE> findPrimitiveElements(const ZZ& field, const ZZ& polDeg) {
    ZZ_p::init(field);
    //ZZ_pX irreducPol;
    //BuildIrred(irreducPol, conv<long>(polDeg));
    ZZ_pE::init(BuildIrred_ZZ_pX(conv<long>(polDeg)));
    std::vector<ZZ_pE> allPrimElements;

    while(allPrimElements.size() != ZZ_pE::cardinality()) {
        bool alreadyExist = false;
        ZZ_pE el;
        random(el);
        for(const auto& mem : allPrimElements)
            if(el == mem) {
                alreadyExist = true;
                break;
            }

        if(!alreadyExist) allPrimElements.push_back(el);
    }

    ZZ_pE primitiveEl;
    for(const auto& el : allPrimElements) {
        if(deg(conv<ZZ_pX>(el)) > 0) {
            ZZ_pE potentialPrimEl = el;
            bool isOK = true;
            for(ZZ i = conv<ZZ>(2); i < ZZ_pE::cardinality(); i++) {
                cout << potentialPrimEl << endl;
                potentialPrimEl *= el;
                if(IsOne(potentialPrimEl) && i != ZZ_pE::cardinality() - 1) {
                    isOK = false;
                    break;
                } 
            }

            if(isOK) {
                primitiveEl = el;
                break;
            }
        }
    }
}

int main() {
    long p;
    cout << "Enter p for GF(p): ";
    cin >> p;

    if(ProbPrime(conv<ZZ>(p))) {
        const auto& primEls = findPrimitiveElements(p);
        cout << "\nNumber of primitive elements in GF(" << p << ") is: " << primEls.size() << endl;
        cout << "Primitive elements in GF(" << p << ") are: ";
        for(const auto& primEl : primEls)
            cout << primEl << " ";
    } else {
        ZZ base, exp;
        if(caclPrimitivePower(ZZ(p), base, exp)) {
            const auto& primEls = findPrimitiveElements(base, exp);
            cout << "\nNumber of primitive elements in GF(" << base << "^" << exp << ") is: " 
                << primEls.size() << endl;
            cout << "Primitive elements in GF(" << base << "^" << exp << ") are: ";
            for(const auto& primEl : primEls)
                cout << primEl << " ";
        } else {
            cout << "Error: 'p' must be a prime number or prime power!" << endl;
            return -1;
        }
    }

    cout << endl << endl;
    return 0;
}