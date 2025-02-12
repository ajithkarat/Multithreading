#include <iostream>
#include <thread>

void print_numbers(int start, int end) {
    for (int i = start; i <= end; ++i) {
        std::cout << "Number: " << i << std::endl;
    }
}

int main() {
    // Launch a thread with two arguments
    std::thread t(print_numbers, 1, 5);

    // Wait for the thread to finish
    t.join();

    return 0;
}
