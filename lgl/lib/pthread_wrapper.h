// These are just simple thread/mutex wrappers.

#ifndef LGL_LIB_APTHHREAD_H_
#define LGL_LIB_APTHHREAD_H_

#include <pthread.h>
#include <unistd.h>

#include <cassert>
#include <iostream>

using namespace std;

typedef void* (*FunctionPtr)(void*);

namespace lgl {
namespace lib {

class Apthread {
 protected:
  pthread_t handle_;
  pthread_attr_t attr_;
  FunctionPtr functionPtr_;

 public:
  Apthread() {
    pthread_attr_init(&attr_);
    pthread_attr_setdetachstate(&attr_, PTHREAD_CREATE_JOINABLE);
  }
  Apthread(const pthread_attr_t& Attr) : attr_(Attr) {}

  int create(FunctionPtr f, void* args = 0);

  int create(void* args = 0);

  void functionPtr(FunctionPtr f) { functionPtr_ = f; }
  FunctionPtr functionPtr() const { return functionPtr_; }

  int scope(int scope) { return pthread_attr_setscope(&attr_, scope); }

  void handle(const pthread_t& h) { handle_ = h; }
  pthread_t handle() const { return handle_; }

  void attribute(const pthread_attr_t& a) { attr_ = a; }
  const pthread_attr_t& attribute() const { return attr_; }

  int scope() const;

  int wait(void** args = 0);

  void copy(const Apthread& t);

  Apthread& operator=(const Apthread& t);

  bool operator==(const Apthread& t) const;

  bool operator!=(const Apthread& t) const { return !(*this == t); }

  const pthread_t& threadHandle() { return handle_; }
  pthread_t id() { return (pthread_self()); }

  ~Apthread() {}
};

class ApthreadContainer {
 public:
  typedef unsigned int size_type;

 protected:
  Apthread* threads_;
  size_type size_;

 public:
  Apthread defaultThread;

 public:
  ApthreadContainer() : threads_(0), size_(0) {}
  ApthreadContainer(size_type count) : size_(count) {
    threads_ = new Apthread[count];
  }

  void copy(const ApthreadContainer& c);

  void create(FunctionPtr f, void* args = 0);

  void create(void* args = 0);

  void wait();

  void applyAttributes();

  size_type size() const { return size_; }

  ApthreadContainer& operator=(const ApthreadContainer& c);

  bool operator==(const ApthreadContainer& c);
  Apthread& operator[](size_type i) { return threads_[i]; }

  ~ApthreadContainer() { delete[] threads_; }
};

class Amutex {
 protected:
  pthread_mutex_t mutex_;
  int error;

  void errorCheck();

  // TODO(alex-kennedy): Remove or re-enable
  void parseError() {
#if 0
     cerr << "Mutex error: " << error << '\t';
     if ( error == EINVAL ) 
       cerr << "mutex has not been properly initialized\n";
     else if ( error == EDEADLK ) 
       cerr << "the mutex is already locked by the calling thread\n";
     else if ( error == EBUSY )
       cerr << "mutex could not be acquired because it was currently locked\n";
     else
       cerr << "unknown error\n";
#endif
  }

 public:
  Amutex(const pthread_mutex_t& m) : mutex_(m) {}
  Amutex();

  void copy(const Amutex& m);

  int lock();

  int trylock();

  int unlock();

  Amutex& operator=(const Amutex& m);
};

}  // namespace lib
}  // namespace lgl

#endif  // LGL_LIB_APTHHREAD_H_
