/**
 * OSFNotificationCenter.cpp - Publish/subscribe notifications
 */

#include <algorithm>
#include <opensef/OpenSEFBase.h>


namespace opensef {

OSFNotificationCenter &OSFNotificationCenter::defaultCenter() {
  static OSFNotificationCenter center;
  return center;
}

OSFNotificationCenter::ObserverToken
OSFNotificationCenter::addObserver(const std::string &name,
                                   Callback callback) {
  std::lock_guard<std::mutex> lock(mutex_);
  ObserverToken token{name, nextObserverID_++};
  observers_[name].emplace_back(token.id, std::move(callback));
  return token;
}

void OSFNotificationCenter::removeObserver(const ObserverToken &token) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = observers_.find(token.name);
  if (it == observers_.end()) {
    return;
  }

  auto &list = it->second;
  list.erase(std::remove_if(list.begin(), list.end(),
                            [&token](const auto &entry) {
                              return entry.first == token.id;
                            }),
             list.end());
}

void OSFNotificationCenter::postNotification(const std::string &name) {
  std::vector<Callback> callbacks;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = observers_.find(name);
    if (it == observers_.end()) {
      return;
    }
    callbacks.reserve(it->second.size());
    for (const auto &entry : it->second) {
      callbacks.push_back(entry.second);
    }
  }

  for (const auto &callback : callbacks) {
    if (callback) {
      callback();
    }
  }
}

} // namespace opensef
