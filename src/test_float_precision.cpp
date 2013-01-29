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
#include <limits>
#include <cmath>

#ifndef _WIN32
/**
  * Print function for __float128 which cheats, and casts
  * to a long double
  */
std::ostream& operator<< (std::ostream& out, __float128 val) {
    const unsigned int* buff = reinterpret_cast<const unsigned int*>(&val);
    out << static_cast<long double>(val);
    out << " ( ";
    for (int i=0; i<128; i+=8*sizeof(unsigned int)) {
        out << std::hex << std::setfill('0') << std::setw(2*sizeof(unsigned int)) << buff[i] << " ";
    }
    out << ")";
    return out;
}
#endif

template <typename T>
void perform_test() {
    T zero = 0.0;
    T one = 1.0;
    T minus_one = -1.0;
    T a = 54;
    T b = -54;
    T c = 54*std::numeric_limits<T>::min();
    
    std::cout << std::setprecision(25);
    std::cout << "=============================" << std::endl;
    std::cout << "Digits:                             " << std::numeric_limits<T>::digits << std::endl;
    std::cout << "Epsilon:                            " << std::numeric_limits<T>::epsilon() << std::endl;
    std::cout << std::endl;
    std::cout << "Largest positive number:            " << std::numeric_limits<T>::max() << std::endl;
    std::cout << "Smallest positive normal number:    " << std::numeric_limits<T>::min() << std::endl;
    std::cout << "Largest positive subnormal number:  " << std::numeric_limits<T>::min() - std::numeric_limits<T>::denorm_min() << std::endl;
    std::cout << "Smallest positive subnormal number: " << std::numeric_limits<T>::denorm_min() << std::endl;
    std::cout << std::endl;
    std::cout << "Largest negative number:            " << -std::numeric_limits<T>::max() << std::endl;
    std::cout << "Smallest negative normal number:    " << -std::numeric_limits<T>::min() << std::endl;
    std::cout << "Largest negative subnormal number:  " << -std::numeric_limits<T>::min() + std::numeric_limits<T>::denorm_min() << std::endl;
    std::cout << "Smallest negative subnormal number: " << -std::numeric_limits<T>::denorm_min() << std::endl;
    std::cout << std::endl;
    std::cout << "Signaling NAN:     " << std::numeric_limits<T>::signaling_NaN() << std::endl;
    std::cout << "Quiet NAN:         " << std::numeric_limits<T>::quiet_NaN() << std::endl;
    std::cout << "Infinity:          " << std::numeric_limits<T>::infinity() << std::endl;
    std::cout << "Negative infinity: " << -std::numeric_limits<T>::infinity() << std::endl;
    std::cout << std::endl;
    std::cout << "1.0/0.0    = " << one/zero << std::endl;
    std::cout << "0.0/0.0    = " << zero/zero << std::endl;
    std::cout << "(-1.0)/0.0 = " << minus_one/zero << std::endl;
    std::cout << "log(0.0)   = " << log(zero) << std::endl;
    std::cout << "log(-1.0)  = " << log(minus_one) << std::endl;
    std::cout << "sqrt(-1.0) = " << sqrt(minus_one) << std::endl;
    std::cout << std::endl;
    std::cout << "a+b+c   = " << (a+b+c) << std::endl;
    std::cout << "(a+b)+c = " << ((a+b)+c) << std::endl;
    std::cout << "a+(b+c) = " << (a+(b+c)) << std::endl;
    std::cout << "=============================" << std::endl;
}

int main() {
    std::cout << "float:" << std::endl;
    perform_test<float>();
    std::cout << std::endl;
    
    std::cout << "double:" << std::endl;
    perform_test<double>();
    std::cout << std::endl;

    std::cout << "long double:" << std::endl;
    perform_test<long double>();
    std::cout << std::endl;

#ifndef _WIN32
    std::cout << "__float80:" << std::endl;
    perform_test<__float80>();
    std::cout << std::endl;

    std::cout << "__float128:" << std::endl;
    perform_test<__float128>();
    std::cout << std::endl;
#endif
}

