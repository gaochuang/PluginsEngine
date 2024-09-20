#ifndef THREAD_THREAD_HPP
#define THREAD_THREAD_HPP

#include <thread>
#include <memory>
#include <condition_variable>

namespace commonApi
{
    class Thread
    {
    public:
      Thread();
      virtual ~Thread();

      Thread(const Thread&) = delete;
      Thread& operator=(const Thread&) = delete;
      Thread(const Thread&&) = delete;
      Thread& operator=(const Thread&&) = delete;

      void start();
      static std::thread::id getThreadId();
      bool isStarted() const;
      
      virtual void run() = 0;
    protected:
       void sleepSeconds(int seconds);

    private:
      void beginRun();
      bool started;
      std::mutex mutex;
      std::condition_variable  condition;
      std::shared_ptr<std::thread> thread;

    };
}

#endif

