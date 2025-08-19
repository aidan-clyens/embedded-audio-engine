#ifndef __THREADED_ENGINE_H_
#define __THREADED_ENGINE_H_

#include <thread>
#include <atomic>
#include <string>
#include <mutex>

#include "messagequeue.h"
#include "logger.h"

/** @class ThreadedEngine
 @  @brief A base class for engines that can process messages in a separate thread.
 */
template <typename T>
class ThreadedEngine
{
public:
  bool is_running() const { return m_running; }

  void start()
  {
    if (m_running)
      return;
    m_thread = std::thread(&ThreadedEngine::run, this);

    // Block until the thread signals it's ready
    while (!m_running.load(std::memory_order_acquire))
    {
      std::this_thread::yield();
    }
  }

  void stop()
  {
    if (!m_running)
      return;
    m_running = false;
    m_message_queue.stop();
    if (m_thread.joinable())
    {
      m_thread.join();
    }
  }

protected:
  ThreadedEngine(const std::string &thread_name): m_thread_name(thread_name) {}

  virtual ~ThreadedEngine() = default;
  ThreadedEngine(const ThreadedEngine&) = delete;
  ThreadedEngine& operator=(const ThreadedEngine&) = delete;

  MessageQueue<T> m_message_queue;

  void run()
  { 
    // Set the thread name
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      set_thread_name(m_thread_name);
    }

    // Signal that the thread is ready
    m_running.store(true, std::memory_order_release);

    LOG_INFO("Started");

    while (m_running)
    {
      // Sleep or yield to avoid busy waiting
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    LOG_INFO("Stopped");
  }

  std::string m_thread_name;

  std::thread m_thread;
  std::atomic<bool> m_running{false};

  std::mutex m_mutex;
};

#endif  // __THREADED_ENGINE_H_