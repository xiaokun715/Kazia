#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

namespace Kazia {

class ThreadPool {
private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    
    std::mutex m_mutex;
    std::condition_variable m_condition;
    bool m_stop;
    
public:
    ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
    ~ThreadPool();
    
    // 提交任务
    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
        using ReturnType = typename std::invoke_result<F, Args...>::type;
        
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<ReturnType> result = task->get_future();
        
        {   
            std::unique_lock<std::mutex> lock(m_mutex);
            
            if (m_stop) {
                throw std::runtime_error("ThreadPool is stopped");
            }
            
            m_tasks.emplace([task]() {
                (*task)();
            });
        }
        
        m_condition.notify_one();
        return result;
    }
    
    // 等待所有任务完成
    void waitForAllTasks();
    
    // 关闭线程池
    void shutdown();
    
    // 获取线程数量
    size_t getThreadCount() const { return m_threads.size(); }
};

} // namespace Kazia

#endif // THREADPOOL_H
