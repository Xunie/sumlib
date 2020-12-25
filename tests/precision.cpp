#include <iomanip>
#include <random>
#include <iostream>
#include <vector>
using namespace std;

#define SUMLIB_ENABLE_DANGEROUS
#include "sumlib.hpp"
using namespace sumlib;

template <typename T>
struct distribution {
    uint64_t terms;
    int64_t mean;
    T deviation;
};

template <typename T, template <typename> class SUMMER, typename RNG>
T verify_accuracy( vector<distribution<T>> dists, RNG rng ) {
    vector<T> numbers;
    T total = 0;
    for( distribution<T> d : dists ) {
        normal_distribution<T> nd( 0, d.deviation );

        for( uint64_t i = 0; i < (d.terms/2); i++ ) {
            T deviation = nd( rng );
            numbers.push_back( d.mean + deviation );
            numbers.push_back( d.mean - deviation );
        }

        total += d.terms * d.mean;
    }

    shuffle( numbers.begin(), numbers.end(), rng );

    SUMMER<T> sum;
    for( auto x : numbers )
        sum += x;

    return sum.get()-total;
}

template <typename T, typename RNG>
void verify_accuracy_all( RNG rng ) {
    const vector<distribution<T>> dists = {
        {20*1000*1000, 0, 1},
        {20*1000*1000, 10, 1},
    };

    cout << "testing for " << typeid(T).name() << ":" << endl;
    cout << "kahan:    " << verify_accuracy<T, kahan>   ( dists, rng ) << endl;
    cout << "neumaier: " << verify_accuracy<T, neumaier>( dists, rng ) << endl;
    cout << "naive:    " << verify_accuracy<T, naive>   ( dists, rng ) << endl;
    cout << "sorted:   " << verify_accuracy<T, sorted>  ( dists, rng ) << endl;
    cout << "rsorted:  " << verify_accuracy<T, rsorted> ( dists, rng ) << endl;

    cout << endl;
}

int main() {
    mt19937_64 rng( 0 ); // seeded with a constant for test determinism

    cout << fixed << setprecision(50) << showpos;

    verify_accuracy_all<float,mt19937_64>( rng );
    verify_accuracy_all<double, mt19937_64>( rng );
    verify_accuracy_all<long double,mt19937_64>( rng );

    return EXIT_SUCCESS;
}
