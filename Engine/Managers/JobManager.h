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

    void wait();


private:
    moodycamel::BlockingConcurrentQueue<Job, ConcurrentQueueTraits> _jobQueue;
    vector<thread> _runners;
    uint _cpuCount{0};
    atomic<uint> _pendingTasks{0};
    atomic_flag _running;
};


} // atlas


#endif // JOBMANAGER_H
