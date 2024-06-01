#include "ThreadPool.h"

ThreadPool::ThreadPool(int threadsCount) : stop(false) {

    for (int i = 0; i < threadsCount; i++) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this] {return stop || !tasks.empty(); });
            if (stop && tasks.empty()) return;
            task = std::move(tasks.front());
            tasks.pop();
        }

        task();

    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    cv.notify_all();
    for(auto& worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(task);
    }

    cv.notify_one();
}