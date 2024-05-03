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
    for(ZZ base = conv<ZZ>(2); base < p; base = NextPrime(base + 1)) {
        ZZ tempP = p;
        ZZ tempExp;
        while(divide(tempP, base)) {
            tempP /= base;
            ++tempExp;
        }

        if(tempP == 1) {
            tBase = base;
            tExp = tempExp;
            return 1;
        }
    }

    return 0;
}

std::vector<ZZ_pE> findMultiplicativeGroup() {
    std::vector<ZZ_pE> multiplicativeGroup;

    while(static_cast<long>(multiplicativeGroup.size()) != ZZ_pE::cardinality()) {
        bool isAlreadyExist = false;
        ZZ_pE el = random_ZZ_pE();

        for(const auto& mem : multiplicativeGroup) {
            if(el == mem) {
                isAlreadyExist = true;
                break;
            }
        }

        if(!isAlreadyExist) multiplicativeGroup.push_back(el);
    }

    return multiplicativeGroup;
}

std::vector<ZZ_pE> findPrimitiveElements(const ZZ& field, const ZZ& polDeg) {
    ZZ_p::init(field);
    ZZ_pE::init(BuildIrred_ZZ_pX(conv<long>(polDeg)));
    std::vector<ZZ_pE> allPrimElements;

    for(const auto& el : findMultiplicativeGroup()) {
        if(deg(conv<ZZ_pX>(el)) > 0) {
            bool isPrimitive = true;
            ZZ_pE potPrimEl = el;

            for(ZZ k = conv<ZZ>(2); k < ZZ_pE::cardinality(); k++) {
                potPrimEl *= el;
                if(IsOne(potPrimEl) && k != ZZ_pE::cardinality() - 1) {
                    isPrimitive = false;
                    break;
                } 
            }

            if(isPrimitive) allPrimElements.push_back(el);
        }
    }

    return allPrimElements;
}

inline void printInitial() {
    cout << "\n***** This program finds all primitive elements in GF(p) *****\n" <<
        "******* Developed by Artur Andriichenko and Anton Yakovenko *******\n\n\n";
}

void app(long p) {

    if(ProbPrime(conv<ZZ>(p))) {
        const auto& primEls = findPrimitiveElements(p);
        cout << "\nNumber of primitive elements in GF(" << p << ") is: " << primEls.size() << endl;
        cout << "Primitive elements in GF(" << p << ") are: ";
        for(const auto& primEl : primEls) {
            if (primEl != *primEls.begin()) {
                cout << ", ";
            }

            cout << primEl;
        }
    } else {
        ZZ base, exp;
        if(caclPrimitivePower(ZZ(p), base, exp)) {
            const auto& primEls = findPrimitiveElements(base, exp);
            cout << "\nNumber of primitive elements in GF(" << base << "^" << exp << ") is: "
                << primEls.size() << endl;
            cout << "Primitive elements in GF(" << base << "^" << exp << ") are: ";
            for(const auto& primEl : primEls) {
                if (primEl != *primEls.begin()) {
                    cout << ", ";
                }

                cout << primEl;
            }
        } else {
            cout << "Error: 'p' must be a prime number or prime power!";
        }
    }

    cout << endl << endl;
}



int main() {
    printInitial();

    while (true) {
        string input;
        long p;
        cout << "Enter p for GF(p) or 'q' to quit: ";
        cin >> input;

        if (input == "q") {
            cout << "Exiting program..." << endl;
            break;
        }

        try {
            p = stol(input);
        } catch (const std::invalid_argument& e) {
            cout << e.what() << ": Invalid argument: " << input << endl;
            continue;
        } catch (const std::out_of_range& e) {
            cout << e.what() << ": Out of range error: " << input << endl;
            continue;
        }

        app(p);
    }

    return 0;
}