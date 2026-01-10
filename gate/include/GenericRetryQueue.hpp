#pragma once


#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
template <typename T>
class GenericRetryQueue
{
public:
    using Handler = std::function<bool(const T&)>;

    GenericRetryQueue(Handler handler, int max_size)
        : m_handler(handler), m_max_size(max_size)
    {}

    // Push an item into the queue
    void push(const T& item)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_queue.size() >= m_max_size)
            {
                // Pop oldest item to make room
                m_queue.pop();
                std::cout << "Queue full. Oldest item removed.\n";
            }
            m_queue.push(item);
        }
        m_cv.notify_one();
    }

    void run()
    {
        while (true)
        {
            std::cout << "retrying to handle in the Q .\n";
            T item;
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cv.wait(lock, [this]() { return !m_queue.empty(); });
                item = m_queue.front(); // copy the data
            }

            if (m_handler(item))
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_queue.pop();
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        }
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    Handler m_handler;
    int m_max_size;
};
