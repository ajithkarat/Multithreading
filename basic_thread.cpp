#include <iostream>
#include <thread>

void print_hello() {
    std::cout << "Hello from the thread!" << std::endl;
}

int main() {
    // Create a thread that calls the print_hello function
    std::thread t(print_hello);

    // Wait for the thread to finish before exiting the main function
    t.join();

    std::cout << "Main thread exiting." << std::endl;
    return 0;
}
