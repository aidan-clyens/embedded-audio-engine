#ifndef ___ENGINE_H_
#define ___ENGINE_H_

#include <thread>
#include <atomic>
#include <string>
#include <mutex>

#include "messagequeue.h"
#include "logger.h"

/** @class IEngine
 @  @brief A base class for engines that can process messages in a separate thread.
 */
template <typename T>
class IEngine
{
public:
  bool is_running() const noexcept { return m_running.load(); }

  void start_thread()
  {
    if (m_running)
      return;
    m_thread = std::thread(&IEngine::_run, this);

    // Block until the thread signals it's ready
    while (!m_running.load(std::memory_order_acquire))
    {
      std::this_thread::yield();
    }
  }

  void stop_thread()
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

  void push_message(const T& msg) { m_message_queue.push(msg); }
  std::optional<T> try_pop_message() { return m_message_queue.try_pop(); }
  bool pop_message(T& out) { return m_message_queue.pop(out); }
  bool queue_empty() const { return m_message_queue.empty(); }

protected:
  IEngine(const std::string &thread_name): m_thread_name(thread_name) {}

  virtual ~IEngine() = default;
  IEngine(const IEngine&) = delete;
  IEngine& operator=(const IEngine&) = delete;

  void _run()
  { 
    // Set the thread name
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      set_thread_name(m_thread_name);
    }

    // Signal that the thread is ready
    m_running.store(true, std::memory_order_release);

    LOG_INFO("Thread Started");

    run();

    LOG_INFO("Thread Stopped");
  }

  virtual void run() = 0;
  virtual void handle_messages() = 0;

private:
  std::string m_thread_name;
  MessageQueue<T> m_message_queue;
  std::thread m_thread;
  std::atomic<bool> m_running{false};
  std::mutex m_mutex;
};

#endif  // ___ENGINE_H_