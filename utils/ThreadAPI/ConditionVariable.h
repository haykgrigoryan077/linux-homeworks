#ifndef THREADAPI_CONDITIONVARIABLE_H
#define THREADAPI_CONDITIONVARIABLE_H
#include <mutex>
#include <pthread.h>
#include "LinuxMutex.h"

namespace rau
{

    class LinuxConditionVariable {
    public:
        using native_handle_type = pthread_cond_t*;

    private:
        pthread_cond_t cond;

    public:
        LinuxConditionVariable() {
            int res = pthread_cond_init(&cond, nullptr);
            if (res != 0) {
                throw std::system_error(res, std::system_category(), "Error initializing condition variable");
            }
        }

        LinuxConditionVariable(const LinuxConditionVariable &) = delete;

        ~LinuxConditionVariable() {
            int res = pthread_cond_destroy(&cond);
            if (res != 0) {
                throw std::system_error(res, std::system_category(), "Error destroying condition variable");
            }
        }

        void notify_one() noexcept
        {
            int res = pthread_cond_signal(&cond);
            if (res != 0) {
                throw std::system_error(res, std::system_category(), "Error signaling condition variable");
            }
        }

        void notify_all() noexcept
        {
            int res = pthread_cond_broadcast(&cond);
            if (res != 0) {
                throw std::system_error(res, std::system_category(), "Error broadcasting condition variable");
            }
        }

        void wait(std::unique_lock<LinuxMutex>& lock)
        {
            int res = pthread_cond_wait(&cond, lock.mutex()->native_handle());
            if (res != 0) {
                throw std::system_error(res, std::system_category(), "Error waiting on condition variable");
            }
        }

        template< class Predicate >
        void wait( std::unique_lock<LinuxMutex>& lock, Predicate stop_waiting )
        {
            while (!stop_waiting()) {
                wait(lock);
            }
        }

        native_handle_type native_handle()
        {
            return &cond;
        }
    };

} // namespace rau

#endif //THREADAPI_CONDITIONVARIABLE_H
