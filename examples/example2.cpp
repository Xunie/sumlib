#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "sumlib.hpp"

using namespace std;
using namespace sumlib;

template <typename S, typename Iterator>
uint64_t perftest( Iterator first, Iterator last ) {
    S summer;

    auto t_start = std::chrono::high_resolution_clock::now();

    for( auto it = first; it != last; it++ )
        summer += *it;

    auto t_end = std::chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::milliseconds>( t_end - t_start ).count();
}

int main() {
    // generate 40 million random floats (~160 megabyte)
    mt19937_64 rng( 0 ); // don't seed so we are deterministic
    normal_distribution<float> nd( 0, 1 );

    vector<float> vec;
    for( uint64_t i = 0; i < 20*1024*1024; i++ ) {
        const auto value = nd(rng);

        // Append both the positive and negative value.
        // This ensures the total sum of the vector equals zero.
        vec.push_back(  value );
        vec.push_back( -value );
    }

    // shuffle the vector randomly
    shuffle( begin(vec), end(vec), rng );

    // run 5 trials
    for( int i = 0; i < 10; i++ ) {

        // run the summation
        cout << "naive: " <<    perftest<naive<float>>( begin(vec), end(vec) ) << endl;
        cout << "kahan: " <<    perftest<kahan<float>>( begin(vec), end(vec) ) << endl;
        cout << "neuma: " << perftest<neumaier<float>>( begin(vec), end(vec) ) << endl;
        cout << "sortd: " <<   perftest<sorted<float>>( begin(vec), end(vec) ) << endl;
        cout << "rsort: " <<  perftest<rsorted<float>>( begin(vec), end(vec) ) << endl;
        cout << "accum: " <<    perftest<accum<float>>( begin(vec), end(vec) ) << endl;


        // Since the accum class privately stores the data to sum, it has unreasonable overhead
        // Let's compare the above to a raw std::accumulate() call:
        auto t_start = std::chrono::high_resolution_clock::now();
        std::accumulate( begin(vec), end(vec), 0.f );
        auto t_end = std::chrono::high_resolution_clock::now();

        cout << "std::accumulate(): " <<  chrono::duration_cast<chrono::milliseconds>( t_end - t_start ).count() << endl;


        // space things out a little
        cout << endl;
    }

    return 0;
}

