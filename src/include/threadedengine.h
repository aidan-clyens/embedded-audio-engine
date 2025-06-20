#ifndef __THREADED_ENGINE_H_
#define __THREADED_ENGINE_H_

#include <thread>
#include <atomic>

#include "messagequeue.h"

/** @class ThreadedEngine
 @  @brief A base class for engines that can process messages in a separate thread.
 */
template <typename T>
class ThreadedEngine
{
public:
  static ThreadedEngine& instance()
  {
    static ThreadedEngine instance;
    return instance;
  }

  /** @brief Start the ThreadedEngine thread.
   */
  void start()
  {
    if (m_running) return;
    m_running = true;
    m_thread = std::thread(&ThreadedEngine::run, this);
  }

  /** @brief Stop the ThreadedEngine thread.
  */
  void stop()
  {
    if (!m_running) return;
    m_running = false;
    m_message_queue.stop();
    if (m_thread.joinable())
    {
      m_thread.join();
    }
  }

  /** @brief Check if the ThreadedEngine is running. 
  */
  bool is_running() const
  {
    return m_running;
  }

protected:
  ThreadedEngine() = default;
  virtual ~ThreadedEngine() = default;
  ThreadedEngine(const ThreadedEngine&) = delete;
  ThreadedEngine& operator=(const ThreadedEngine&) = delete;

  // Thread main loop
  virtual void run()
  {
    while (m_running)
    {
      // Sleep or yield to avoid busy waiting
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

  MessageQueue<T> m_message_queue;

  std::thread m_thread;
  std::atomic<bool> m_running{false};
};

#endif  // __THREADED_ENGINE_H_