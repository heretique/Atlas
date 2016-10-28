#ifndef JOBMANAGER_H
#define JOBMANAGER_H

#include "blockingconcurrentqueue.h"

namespace atlas {

typedef std::function<void (void*, uint)> JobFunc;

struct Job
{
    JobFunc func;
    void* data;
    uint count;
};

template <typename DataType, int Size>
class CountSplitter
{
public:
    CountSplitter() {}

    static bool split(uint count)
    {
        return count > Size;
    }
};

template <typename DataType, int Size>
class DataSizeSplitter
{
public:
    DataSizeSplitter() {}

    static bool split(uint count)
    {
        return count*sizeof(DataType) > Size;
    }
};

struct ConcurrentQueueTraits : public moodycamel::ConcurrentQueueDefaultTraits
{
    static const size_t BLOCK_SIZE = 256;       // Use bigger blocks
};

class JobManager
{
public:
    void init();
    void release();

    void addJob(JobFunc func, void* data, uint count = 1);

    template <typename DataType, typename SplitterType>
    void parallel_for(JobFunc func, void* data, uint count);

    void wait();


private:
    moodycamel::BlockingConcurrentQueue<Job, ConcurrentQueueTraits> _jobQueue;
    vector<thread> _runners;
    uint _cpuCount{0};
    atomic<uint> _pendingTasks{0};
    atomic_flag _running;
};

template <typename DataType, typename SplitterType>
void JobManager::parallel_for(JobFunc func, void* data, uint count)
{

    if (SplitterType::split(count))
    {
        auto jobFunc = [=](void* splitData, uint splitCount) {
            DataType* castData = static_cast<DataType*>(splitData);
            const uint leftCount = splitCount / 2u;
            const uint rightCount = splitCount - leftCount;
            parallel_for<DataType, SplitterType>(func, castData, leftCount);
            parallel_for<DataType, SplitterType>(func, castData + leftCount, rightCount);
        };
        addJob(jobFunc, data, count);
    }
    else
    {
        addJob(func, data, count);
    }
}

} // atlas


#endif // JOBMANAGER_H
