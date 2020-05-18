# sumlib
Equal to the sum of its parts (more or less).

### What?
This library allows more accurate summation of floating point numbers through different algorithms. So far, it supports the following algorithms:
* Kahan
* Neumaier or 'improved Kahan-Babuska'

### Why?
Summing a large number of floating point numbers may result in significant inaccuracies. While arbitrary precision libraries will result in higher accuracies, they also have more overhead. It's also fairly overkill to grab GNU MP if all you want to do is sum some numbers. Hence this library.
