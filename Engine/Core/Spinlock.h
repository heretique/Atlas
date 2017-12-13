#pragma once

#include <atomic>

class SpinLock
{
public:
    void lock()
    {
        while (_lock.test_and_set(std::memory_order_acquire))
        {
        }
    }

    void unlock()
    {
        _lock.clear(std::memory_order_release);
    }

private:
    std::atomic_flag _lock = ATOMIC_FLAG_INIT;
};
