#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "sumlib.hpp"

using namespace std;
using namespace sumlib;

int main() {
    // show the numbers for easy visual comparison
    cout << fixed << showpos << setprecision(25);


    mt19937_64 rng( 0 ); // don't seed so we are deterministic
    normal_distribution<float> nd( 0, 1 );

    // generate 2 million random numbers
    vector<float> vec;
    for( uint64_t i = 0; i < 1*1000*1000; i++ ) {
        const auto value = nd(rng);

        // Append both the positive and negative value.
        // This ensures the total sum of the vector equals zero.
        vec.push_back(  value );
        vec.push_back( -value );
    }


    // run 5 trials
    for( int i = 0; i < 5; i++ ) {
        // shuffle the vector randomly
        shuffle( begin(vec), end(vec), rng );

        // run the summation
        cout << "naive: " <<    sum<naive>( begin(vec), end(vec) ) << endl;
        cout << "kahan: " <<    sum<kahan>( begin(vec), end(vec) ) << endl;
        cout << "neuma: " << sum<neumaier>( begin(vec), end(vec) ) << endl;
        cout << "sortd: " <<   sum<sorted>( begin(vec), end(vec) ) << endl;
        cout << "rsort: " <<  sum<rsorted>( begin(vec), end(vec) ) << endl;
        cout << "accum: " <<    sum<accum>( begin(vec), end(vec) ) << endl;

        // space things out a little
        cout << endl;
    }

    return 0;
}
