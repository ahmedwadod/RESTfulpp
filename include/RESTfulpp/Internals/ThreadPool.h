#ifndef __RESTFULPP_INTERNALS_THREADPOOL_H__
#define __RESTFULPP_INTERNALS_THREADPOOL_H__

#include "sockpp/inet_address.h"
#include "sockpp/socket.h"
#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"
#include "RESTfulpp/Types.h"
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <vector>

namespace RESTfulpp
{
    class ThreadPool
    {
    public:
        ThreadPool();
        ThreadPool(
            std::function<void(std::pair<sockpp::tcp_socket, sockpp::inet_address>)>
                runner_function);
        void start(unsigned int num_threads);
        void queue_job(std::pair<sockpp::tcp_socket, sockpp::inet_address> job);
        void stop();
        bool is_busy();

    private:
        void _thread_loop();

        std::function<void(std::pair<sockpp::tcp_socket, sockpp::inet_address>)>
            runner;
        bool should_terminate = false;
        std::mutex queue_mutex;
        std::condition_variable mutex_condition;
        std::vector<std::thread> threads;
        std::queue<std::pair<sockpp::tcp_socket, sockpp::inet_address>> jobs;
    };
}
#endif // !__RESTFULPP_INTERNALS_THREADPOOL_H__