#pragma once
#include <functional>
#include <queue>
#include <mutex>
#include <vector>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(int threadsCount);
    ~ThreadPool();
    void enqueue(std::function<void()> task);
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::condition_variable cv;
    std::mutex queueMutex;
    bool stop;

    void workerThread();
};