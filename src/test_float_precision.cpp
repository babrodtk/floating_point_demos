/**
  * Copyright (c) 2013, André R. Brodtkorb <babrodtk@ifi.uio.no>
  * All rights reserved.
  * 
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met: 
  * 
  * 1. Redistributions of source code must retain the above copyright notice, this
  *    list of conditions and the following disclaimer. 
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution. 
  * 
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
  * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * 
  * The views and conclusions contained in the software and documentation are those
  * of the authors and should not be interpreted as representing official policies, 
  * either expressed or implied, of the FreeBSD Project.
  */

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
