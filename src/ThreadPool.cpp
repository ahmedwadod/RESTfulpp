#include "RESTfulpp/Internals/ThreadPool.h"
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "sockpp/inet_address.h"
#include "sockpp/socket.h"
#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"

using namespace RESTfulpp;

ThreadPool::ThreadPool(){};
ThreadPool::ThreadPool(
    std::function<void(std::pair<sockpp::tcp_socket, sockpp::inet_address>)>
        runner_function)
    : runner(runner_function) {}

void ThreadPool::start(unsigned int num_threads)
{
    for (unsigned int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(&ThreadPool::_thread_loop, this);
    }
}

void ThreadPool::queue_job(
    std::pair<sockpp::tcp_socket, sockpp::inet_address> job)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(std::move(job));
    }

    mutex_condition.notify_one();
}

void ThreadPool::_thread_loop()
{
    while (1)
    {
        std::pair<sockpp::tcp_socket, sockpp::inet_address> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(
                lock, [this]()
                { return !jobs.empty() || should_terminate; });
            if (should_terminate)
            {
                return;
            }
            job = std::move(jobs.front());
            jobs.pop();
        }
        runner(std::move(job));
    }
}

bool ThreadPool::is_busy()
{
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = !jobs.empty();
    }
    return poolbusy;
}

void ThreadPool::stop()
{
    if (should_terminate)
        return;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread &active_thread : threads)
    {
        active_thread.join();
    }
    threads.clear();
}