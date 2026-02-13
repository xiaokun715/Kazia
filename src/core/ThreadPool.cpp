#include "ThreadPool.h"

namespace Kazia {

ThreadPool::ThreadPool(size_t numThreads) : m_stop(false) {
    // 创建线程
    for (size_t i = 0; i < numThreads; ++i) {
        m_threads.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                
                {   
                    std::unique_lock<std::mutex> lock(this->m_mutex);
                    
                    this->m_condition.wait(lock, [this]() {
                        return this->m_stop || !this->m_tasks.empty();
                    });
                    
                    if (this->m_stop && this->m_tasks.empty()) {
                        return;
                    }
                    
                    task = std::move(this->m_tasks.front());
                    this->m_tasks.pop();
                }
                
                // 执行任务
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::waitForAllTasks() {
    while (true) {
        {   
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_tasks.empty()) {
                break;
            }
        }
        
        // 短暂休眠，避免忙等
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void ThreadPool::shutdown() {
    {   
        std::unique_lock<std::mutex> lock(m_mutex);
        m_stop = true;
    }
    
    // 通知所有线程
    m_condition.notify_all();
    
    // 等待所有线程结束
    for (std::thread& thread : m_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

} // namespace Kazia
