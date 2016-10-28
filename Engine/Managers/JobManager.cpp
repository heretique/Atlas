#include "Base.h"
#include "JobManager.h"
#include "Engine.h"
#include "fmt/printf.h"
#include <thread>

namespace atlas {


void JobManager::init()
{
    _cpuCount = thread::hardware_concurrency();

    if (_cpuCount > 2)
        _cpuCount -= 2; // substract main an rendering threads;

    fmt::print("Starting {} worker threads...\n", _cpuCount);

    _running.test_and_set(std::memory_order_acquire);

    for (uint i = 0;  i < _cpuCount; ++i)
    {
        _runners.push_back(thread([&](){
            fmt::print("Starting worker thread...\n");
            Job job;
            while (_running.test_and_set(std::memory_order_acquire) == true)
            {
                _jobQueue.wait_dequeue(job);

                job.func(job.data, job.count);

                _pendingTasks.fetch_add(-1, std::memory_order_release);
            }
            fmt::print("Exiting worker thread...\n");
            _running.clear();
        }));
    }
}

void JobManager::release()
{
    _running.clear();
}

void JobManager::addJob(JobFunc func, void *data, uint count)
{
    _pendingTasks.fetch_add(1, std::memory_order_release);
//    while (!_jobQueue.try_enqueue(Job{func, data, count})) continue;    // this doesn't work, can't figure out why :(
    _jobQueue.enqueue(Job{func, data, count});
}

void JobManager::wait()
{
    Job job;
    while (_pendingTasks.load(std::memory_order_acquire) != 0) {
        if (!_jobQueue.try_dequeue(job)) {
            continue;
        }

        job.func(job.data, job.count);

        _pendingTasks.fetch_add(-1, std::memory_order_release);
    }
}


} // atlas
