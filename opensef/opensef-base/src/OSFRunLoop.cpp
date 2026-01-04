/**
 * OSFRunLoop.cpp - Basic task runner
 */

#include <opensef/OpenSEFBase.h>


namespace opensef {

OSFRunLoop &OSFRunLoop::main() {
  static OSFRunLoop runLoop;
  return runLoop;
}

void OSFRunLoop::run() {
  std::unique_lock<std::mutex> lock(mutex_);
  running_ = true;
  while (running_) {
    if (tasks_.empty()) {
      cv_.wait(lock, [this]() { return !running_ || !tasks_.empty(); });
    }

    while (!tasks_.empty()) {
      Task task = std::move(tasks_.front());
      tasks_.pop_front();
      lock.unlock();
      if (task) {
        task();
      }
      lock.lock();
    }
  }
}

void OSFRunLoop::stop() {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    running_ = false;
  }
  cv_.notify_all();
}

void OSFRunLoop::postTask(Task task) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    tasks_.push_back(std::move(task));
  }
  cv_.notify_one();
}

} // namespace opensef
