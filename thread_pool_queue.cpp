#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <atomic>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t num_threads) : stop(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->cv.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template <class F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.push(std::forward<F>(f));
        }
        cv.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        cv.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable cv;
    bool stop;
};

void print_number(int n) {
    std::cout << "Task " << n << " is being executed." << std::endl;
}

int main() {
    ThreadPool pool(3);  // Create a thread pool with 3 threads

    // Enqueue tasks
    for (int i = 1; i <= 5; ++i) {
        pool.enqueue([i] { print_number(i); });
    }

    // ThreadPool will clean up automatically when going out of scope
    return 0;
}
