#ifndef SUMLIB_HPP
#define SUMLIB_HPP
#include <algorithm>
#include <iterator>
#include <vector>

namespace sumlib {
    /**
     * Helper function to utilize the sumation algorithms in an imperative manner
     */
    template <template <typename> class S, typename Iterator>
    auto sum( Iterator first, Iterator last ) {
        S<typename std::iterator_traits<Iterator>::value_type> s;

        for( auto it = first; it != last; it++ )
            s += *it;

        return s.get();
    }

    /**
     * Purely abstract prototype
     */
    template <typename T>
    class abstract_summer {
    public:
        virtual abstract_summer& operator+=( const T x ) = 0;
        virtual T get() const = 0;
    };

    /**
     * Naive summing algorithm, not recommended! (for completeness sake)
     */
    template <typename T>
    class naive : public abstract_summer<T> {
        T sum = 0;
    public:
        naive& operator+=( const T x ) {
            sum += x;
            return *this;
        }

        T get() const { return sum; }
    };

    /**
     * std::accumulate(), stores all elements in a vector
     * 
     * accumulate() is implementation defined but may just be the naive algorithm.
     * (It certainly is just naive summation on GCC 10.1)
     */
    template <typename T>
    class accum : public abstract_summer<T> {
        std::vector<T> xs;
    public:
        accum& operator+=( const T x ) {
            xs.push_back(x);
            return *this;
        }

        T get() const { return accumulate(begin(xs), end(xs), (T)0); }
    };

    /**
     * stores everything in a vector, sorts and then calls std::accumulate()
     */
    template <typename T>
    class sorted {
        std::vector<T> xs;
    public:
        sorted& operator+=( const T x ) {
            xs.push_back(x);
            return *this;
        }

        T get() {
            std::sort( begin(xs), end(xs) );
            return accumulate(begin(xs), end(xs), (T)0);
        }
    };

    /**
     * reverse-sorted std::accumulate(), stores all elements in a vector and sums in reverse
     */
    template <typename T>
    class rsorted {
        std::vector<T> xs;
    public:
        rsorted& operator+=( const T x ) {
            xs.push_back(x);
            return *this;
        }

        T get() {
            std::sort( begin(xs), end(xs) );
            std::reverse( begin(xs), end(xs) );
            return accumulate(begin(xs), end(xs), (T)0);
        }
    };

    /**
     * Traditional kahan summation algorithm
     */
    template <typename T>
    class kahan : public abstract_summer<T> {
        T sum = 0;
        T compensation = 0;
    public:
        kahan& operator+=( const T x ) {
            const T y = x - compensation;
            const T t = sum + y;
            compensation = (t - sum) - y;
            sum = t;

            return *this;
        }

        T get() const { return sum; }
    };


    /**
     * 'Neumaier' or 'improved Kahan-Babuska' is an improvement over traditional Kahan summation
     */ 
    template <typename T>
    class neumaier : public abstract_summer<T> {
        T sum = 0;
        T compensation = 0;
    public:
        neumaier& operator+=( const T x ) {
            // this is the total, this will be the new sum
            const T total = sum + x;

            // We are forced to drop some precision when adding numbers,
            // but we can decide to preserve the most significant digits.
            if( abs(x) >= abs(sum) )
                compensation += ((x - total) + sum);
            else
                compensation += ((sum-total) + x);

            // don't forget to store the actual sum!
            sum = total;
            return *this;
        }

        T get() const { return sum + compensation; }
    };
}

#endif /* SUMLIB_HPP */
