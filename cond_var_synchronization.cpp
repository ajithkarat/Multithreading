#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void print_numbers() {
    std::cout << "Waiting for signal..." << std::endl;
    std::unique_lock<std::mutex> lock(mtx); // Lock mutex
    cv.wait(lock, []{ return ready; });      // Wait until ready is true
    std::cout << "Printing numbers now!" << std::endl;
    
    for (int i = 1; i <= 5; ++i) {
        std::cout << "Number: " << i << std::endl;
    }
}

void set_ready() {
    std::cout << "Setting ready flag to true." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));  // Simulate some work
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();  // Notify the waiting thread
}

int main() {
    std::thread t1(print_numbers);
    std::thread t2(set_ready);

    t1.join();
    t2.join();

    return 0;
}
