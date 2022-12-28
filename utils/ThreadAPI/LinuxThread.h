#ifndef THREADAPI_LINUXTHREAD_H
#define THREADAPI_LINUXTHREAD_H

#include <cstring>
#include <typeinfo>
#include <functional>

namespace rau
{

class LinuxThread
{
public:
    using native_handle_type = pthread_t;

public:

    // The class LinuxThread::id is a lightweight, trivially copyable class that serves as
    // a unique identifier of LinuxThread.
    //
    // Instances of this class may also hold the special distinct value that
    // does not represent any thread. Once a thread has finished,
    // the value of LinuxThread::id may be reused by another thread.
    //
    // This class is designed for use as key in associative containers, both ordered and unordered.
    class id
    {
    public:

        friend bool operator==( id lhs, id rhs ) noexcept
        {
            return 0 != pthread_equal(lhs.m_threadID, rhs.m_threadID);
        }

        friend bool operator!=( id lhs, id rhs ) noexcept
        {
            return !(lhs == rhs);
        }

        friend bool operator < ( id lhs, id rhs ) noexcept
        {
            return lhs.m_threadID < rhs.m_threadID;
        }

        friend bool operator<=( id lhs, id rhs ) noexcept
        {
            return !(lhs > rhs);
        }

        friend bool operator> ( id lhs, id rhs ) noexcept
        {
            return rhs < lhs;
        }

        friend bool operator>=( id lhs, id rhs ) noexcept
        {
            return !(lhs < rhs);
        }

        friend std::ostream& operator<<(std::ostream& out, id obj)
        {
            out << obj.m_threadID;
            return out;
        }

        friend std::hash<LinuxThread::id>;

        native_handle_type m_threadID{0ull};
    };

private:
    id m_ID;

public:

    native_handle_type native_handle()
    {
        return *reinterpret_cast<native_handle_type*>(&m_ID);
    }

    LinuxThread() noexcept = default;

    template <typename Callable, typename ...Args>
    explicit LinuxThread(Callable&& c, Args&&... args) noexcept
    {
        auto* wrapper = new detail::ThreadRoutineWrapper<Callable&&, Args&&...>
                {std::forward<Callable&&>(c), std::forward<Args&&>(args)...};
        using lambda_type = std::remove_reference_t<decltype(*wrapper)>;
        native_handle_type temp_id;
        auto result = pthread_create(&temp_id, nullptr,
                                     &detail::posix_thread_routine<lambda_type>, wrapper);
        if(result != 0)
        {
            delete wrapper;
            throw std::runtime_error{strerror(errno)};
        }
        m_ID = *reinterpret_cast<id*>(temp_id);
    }

    LinuxThread( LinuxThread&& other ) noexcept
    {
        m_ID = std::move(other.m_ID);
        other.m_ID = id{};
    }

    LinuxThread( const LinuxThread& ) = delete;

    ~LinuxThread()
    {
        if (joinable())
        {
            std::terminate();
        }
    }

    LinuxThread& operator=(LinuxThread&& other)
    {
        if (joinable())
        {
            throw std::runtime_error{"Cannot move from a joinable thread"};
        }
        if (other.joinable())
        {
            throw std::runtime_error{"Cannot move to a joinable thread"};
        }
        m_ID = other.m_ID;
        other.m_ID = id{};
        return *this;
    }

    LinuxThread& operator=(LinuxThread& other) = delete;

    [[nodiscard]]
    id get_id() const
    {
        return this->m_ID;
    }

    [[nodiscard]]

    bool joinable() const noexcept
    {
        return get_id() != LinuxThread::id();
    }

    void join()
    {
        if (!joinable())
        {
            throw std::runtime_error{"Cannot join a non-joinable thread"};
        }
        pthread_join(m_ID.m_threadID, nullptr);
        m_ID = id{};
    }

    void detach()
    {
        if (!joinable())
        {
            throw std::runtime_error{"Cannot detach a non-joinable thread"};
        }
        pthread_detach(m_ID.m_threadID);
        m_ID = id{};
    }
};

}

namespace std
{

template <>
struct hash<rau::LinuxThread::id>
{
    size_t operator()(rau::LinuxThread::id&& id)
    {
        return id.m_threadID;
    }
};

} // namespace std


#endif //THREADAPI_LINUXTHREAD_H
