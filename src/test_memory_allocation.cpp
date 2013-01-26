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
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

/**
  * Sleeps for ms milliseconds
  */
void cross_platform_sleep_ms(const unsigned int& ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    const unsigned int ms_to_us = 1000;
    usleep(ms*ms_to_us);
#endif
}

/**
  * Tests how much memory an allocation takes for
  * different basic variables
  */
template <typename T>
void allocation_test() {
    const unsigned int num_values = 10000000;
    const double bytes_to_megabytes = 1.0/(1024.0*1024.0);
    std::vector<T> values(0);
    values.resize(num_values);

    const T* start = &values[0];
    const T* end = &values[values.size()-1]+1;
    long long bytes_allocated = reinterpret_cast<long long>(end) - reinterpret_cast<long long>(start);

    std::cout << "Address of first element: " << start << std::endl;
    std::cout << "Address of last element: " << end << std::endl;
    std::cout << "Size of each element (bytes): " << sizeof(values[0]) << std::endl;
    std::cout << "Bytes allocated: " << bytes_allocated << " (" << bytes_allocated*bytes_to_megabytes << " MB)" << std::endl;
    std::cout << "Sleeping five seconds..." << std::endl;
    cross_platform_sleep_ms(5000);
    values.resize(0);
    std::cout << "Memory freed" << std::endl << std::endl;
}

int main() {    
    std::cout << "Floating point versus double: Which uses more memory?" << std::endl;
    std::cout << "Use your windows task manager or 'top' to view resource use" << std::endl;
    std::cout << std::endl;
    cross_platform_sleep_ms(1000);

    std::cout << "Testing allocation of float:" << std::endl;
    allocation_test<float>();

    cross_platform_sleep_ms(2000);
    
    std::cout << "Testing allocation of double:" << std::endl;
    allocation_test<double>();
}
