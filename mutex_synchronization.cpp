#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;  // Mutex for synchronization

void print_numbers(int start, int end) {
    for (int i = start; i <= end; ++i) {
        std::lock_guard<std::mutex> lock(mtx);  // Lock the mutex
        std::cout << "Number: " << i << std::endl;
    }
}

int main() {
    // Launch two threads
    std::thread t1(print_numbers, 1, 5);
    std::thread t2(print_numbers, 6, 10);

    // Wait for both threads to finish
    t1.join();
    t2.join();

    return 0;
}
