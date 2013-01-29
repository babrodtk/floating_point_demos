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
#include <omp.h>


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


/**
  * Computes the sum of ten million value_'s
  * in parallel
  */
template<typename T>
T parallel_sum_ten_million(const T& value_) {
    unsigned int iterations = 10000000;
    T result = 0.0;

    #pragma omp parallel for schedule(dynamic, 50) reduction(+:result)
    for (int i = 0; i<iterations; ++i) {
        result += value_;
    }
    return result;
}

template <typename T>
void perform_test(const T& value_) {
    const unsigned int iterations = 10;
    const unsigned int max_threads = 8;
    std::cout << "Floating point bits=" << sizeof(T)*8 << std::endl;
    for (unsigned int i=1; i<max_threads; ++i) {
        omp_set_num_threads(i);
        #pragma omp parallel
        if (omp_get_thread_num() == 0) {
            std::cout << "Using " << omp_get_num_threads() << " threads" << std::endl;
        }

        for (unsigned int j=0; j<iterations; ++j) {
            T result = parallel_sum_ten_million(value_);
            std::cout << "`-> Run " << j << ": " << std::fixed << std::setprecision(25) << result << std::endl;
        }
    }
}


int main() {    
    float value_f = 0.1f;
    std::cout << "Float:" << std::endl;
    perform_test(value_f);
    std::cout << std::endl;
    
    double value_d = 0.1;
    std::cout << "Double:" << std::endl;
    perform_test(value_d);
    std::cout << std::endl;
    
    long double value_ld = 0.1;
    std::cout << "Long double:" << std::endl;
    perform_test(value_ld);
    std::cout << std::endl;
}
