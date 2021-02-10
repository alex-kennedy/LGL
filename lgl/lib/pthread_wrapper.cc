#include "pthread_wrapper.h"

namespace lgl {
namespace lib {

int Apthread::create(FunctionPtr f, void* args) {
  functionPtr_ = f;
  return pthread_create(&handle_, &attr_, f, args);
}

int Apthread::create(void* args) {
  return pthread_create(&handle_, &attr_, functionPtr_, args);
}

int Apthread::scope() const {
  int scope;
  pthread_attr_getscope(const_cast<pthread_attr_t*>(&attr_), &scope);
  return scope;
}

int Apthread::wait(void** args) { return pthread_join(handle_, args); }

void Apthread::copy(const Apthread& t) {
  handle_ = t.handle_;
  attr_ = t.attr_;
}

Apthread& Apthread::operator=(const Apthread& t) {
  Apthread::copy(t);
  return *this;
}

bool Apthread::operator==(const Apthread& t) const {
  // This sucks
  return &handle_ == &(t.handle_) && &attr_ == &(t.attr_);
}

void ApthreadContainer::copy(const ApthreadContainer& c) {
  if (*this == c) return;
  delete[] threads_;
  size_ = c.size_;
  threads_ = new Apthread[c.size_];
  for (size_type ii = 0; ii < size_; ++ii) {
    threads_ = c.threads_;
  }
}

void ApthreadContainer::create(FunctionPtr f, void* args) {
  int rc;
  defaultThread.functionPtr(f);
  for (size_type ii = 0; ii < size_; ++ii) {
    rc = threads_[ii].create(f, args);
    if (rc != 0) {
      std::cerr << "ApthreadContainer: Error on thread create." << std::endl;
      // exit(1);
    }
  }
}

void ApthreadContainer::create(void* args) {
  int rc;
  for (size_type ii = 0; ii < size_; ++ii) {
    rc = threads_[ii].create(defaultThread.functionPtr(), args);
    if (rc != 0) {
      std::cerr << "ApthreadContainer: Error on thread create." << std::endl;
      // exit(1);
    }
  }
}

void ApthreadContainer::wait() {
  int rc;
  for (size_type ii = 0; ii < size_; ++ii) {
    rc = threads_[ii].wait();
    if (rc != 0) {
      std::cerr << "ApthreadContainer: Error on thread wait.\t";
      std::cerr << rc << std::endl;
      // exit(1);
    }
  }
}

void ApthreadContainer::applyAttributes() {
  for (size_type ii = 0; ii < size_; ++ii) {
    threads_[ii].attribute(defaultThread.attribute());
  }
}

ApthreadContainer& ApthreadContainer::operator=(const ApthreadContainer& c) {
  ApthreadContainer::copy(c);
  return *this;
}

bool ApthreadContainer::operator==(const ApthreadContainer& c) {
  for (size_type i = 0; i < size_; ++i) {
    if (threads_[i] != c.threads_[i]) return 0;
  }
  return 1;
}

void Amutex::errorCheck() {
  if (error != 0) Amutex::parseError();
}

Amutex::Amutex() : mutex_() {
  const int err = pthread_mutex_init(&mutex_, nullptr);
  if (err != 0)
    throw std::runtime_error("Mutex initialization failed with error code " +
                             std::to_string(err));
}

void Amutex::copy(const Amutex& m) { mutex_ = m.mutex_; }

int Amutex::lock() {
  const int ret = pthread_mutex_lock(&mutex_);
  if (ret) std::cerr << "pthread_mutex_lock returned nonzero: " << ret << '\n';
  return ret;
}

int Amutex::trylock() { return pthread_mutex_trylock(&mutex_); }

int Amutex::unlock() { return pthread_mutex_unlock(&mutex_); }

Amutex& Amutex::operator=(const Amutex& m) {
  Amutex::copy(m);
  return *this;
}

}  // namespace lib
}  // namespace lgl
