#ifndef __RESOURCE_ENGINE_H__
#define __RESOURCE_ENGINE_H__

#include "subject.h"
#include "threadedengine.h"

#include <string>

/** @class ResourceEngine 
 *  @brief This abstract class is used as a base for engines that manage resources
 *  such as audio or MIDI.
 */
template <typename T>
class ResourceEngine : public ThreadedEngine<T>, public Subject<T>
{
protected:
  // This is an abstract class, so we do not allow instantiation
  ResourceEngine(const std::string &thread_name): ThreadedEngine<T>(thread_name) {}
  ~ResourceEngine() override = default;

  ResourceEngine(const ResourceEngine&) = delete;
  ResourceEngine& operator=(const ResourceEngine&) = delete;

  void push_message(const T& message)
  {
    this->m_message_queue.push(message);
  }

  void process_message(const T &message)
  {
    this->notify(message);
  }

  void handle_messages()
  {
    auto message = this->m_message_queue.try_pop();
    
    if (message)
    {
      LOG_INFO("Received message: ", message.value());
      this->process_message(message.value());
    }
  }

  virtual void run()
  {
    while (this->m_running)
    {
      handle_messages();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
};

#endif  // __RESOURCE_ENGINE_H__