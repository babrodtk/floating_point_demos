#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

/**
  * Returns current time in milliseconds
  */
unsigned long long getCurrentTimeMS() {
#ifdef _WIN32
    LARGE_INTEGER f;
    LARGE_INTEGER t;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&t);
    return static_cast<unsigned long long>(t.QuadPart * 1e6/(double) f.QuadPart);
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return static_cast<unsigned long long>(ts.tv_sec*1e6+ts.tv_nsec/1e3);
#endif
}

/**
  * Computes the sum of ten million value_'s
  */
template<typename T>
T sum_ten_million(const T& value_) {
    unsigned int iterations = 10000000;
    T result = 0.0;
    for (unsigned int i = 0; i<iterations; ++i) {
        result += value_;
    }
    return result;
}

#ifndef _WIN32
/**
  * Print function for __float128 which cheats, and casts
  * to a long double
  */
std::ostream& operator<< (std::ostream& out, __float128 val) {
    out << static_cast<long double>(val);
    return out;
}
#endif

template <typename T>
void perform_test(const T& value_) {
    unsigned long long start = getCurrentTimeMS();
    T result = sum_ten_million(value_);
    unsigned long long end = getCurrentTimeMS();
    std::cout << "Floating point bits=" << sizeof(T)*8 << std::endl;
    std::cout << std::fixed << std::setprecision(50) << result << std::setprecision(-1) << std::endl;
    std::cout << "Completed in " << (end-start) / 1.0e6 << " s." << std::endl;
}


int main() {
    float value_f = 0.1f;
    std::cout << "float:" << std::endl;
    perform_test(value_f);
    std::cout << std::endl;
    
    double value_d = 0.1;
    std::cout << "double:" << std::endl;
    perform_test(value_d);
    std::cout << std::endl;

    long double value_ld = 0.1;
    std::cout << "long double:" << std::endl;
    perform_test(value_ld);
    std::cout << std::endl;

#ifndef _WIN32
    __float80 value_80 = 0.1w;
    std::cout << "__float80:" << std::endl;
    perform_test(value_80);
    std::cout << std::endl;

    __float128 value_128 = 0.1q;
    std::cout << "__float128:" << std::endl;
    perform_test(value_128);
    std::cout << std::endl;
#endif
}
