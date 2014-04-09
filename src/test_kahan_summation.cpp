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
#include <vector>
#include <cstdlib>


template<typename T>
T kahan_sum(const std::vector<T>& values_) {
    T result = 0.0;
    #pragma omp parallel
    {
        T thread_result = 0.0;
        T compensation = 0.0;
        #pragma omp for schedule(dynamic, 50)
        for (int i = 0; i<values_.size(); ++i) {
            T value = values_[i] - compensation;
            T sum = thread_result + value;
            compensation = (sum - thread_result) - value;
            thread_result = sum;
        }
        #pragma omp critical
        result = result + thread_result;
    }
    return result;
}

template <typename T>
T parallel_sum(const std::vector<T>& values_) {
    T result = 0.0;
    #pragma omp parallel for schedule(dynamic, 50) reduction(+:result)
    for (int i = 0; i<values_.size(); ++i) {
        result = result + values_[i];
    }
    return result;
}

template <typename T>
T sum(const std::vector<T>& values_) {
    T result = 0.0;
    for (unsigned int i = 0; i<values_.size(); ++i) {
        result = result + values_[i];
    }
    return result;
}

template <typename T>
void perform_test() {
    std::vector<T> values(10000000);
    srand(0); //Make sure that the random numbers are the same for each run

    for (unsigned int i=0; i<values.size(); ++i) {
        values[i] = rand() / static_cast<T>(RAND_MAX);
    }

    const unsigned int iterations = 15;
    std::cout << "Floating point bits=" << sizeof(T)*8 << std::endl;
    std::cout << "Serial sum " << sum(values) << std::endl;
    std::cout << "Parallel sum, Kahan sum" << std::endl;
    for (unsigned int i=0; i<iterations; ++i) {
        T parallel_result = parallel_sum(values);
        T kahan_result = kahan_sum(values);
        std::cout << "Run " << i << ": ";
        std::cout << std::fixed << std::setprecision(40) << parallel_result << ", " 
                  << std::fixed << std::setprecision(40) << kahan_result << std::endl;
    }
}


int main() {
    omp_set_num_threads(10);
    #pragma omp parallel
    if (omp_get_thread_num() == 0) {
        std::cout << "OpenMP float test using " << omp_get_num_threads() << " threads" << std::endl;
    }

    std::cout << "Float:" << std::endl;
    perform_test<float>();
    std::cout << std::endl;

    std::cout << "Double:" << std::endl;
    perform_test<double>();
    std::cout << std::endl;

    std::cout << "Long double:" << std::endl;
    perform_test<long double>();
    std::cout << std::endl;
}
