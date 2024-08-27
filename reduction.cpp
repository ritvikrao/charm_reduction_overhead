#include "reductiontest.decl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


CProxy_Main mainProxy;
CProxy_ReduceChares arr;

void start_reductions(void *obj, double time)
{
    arr.contribute_reduction();
}

void end_time(void *obj, double time)
{
    arr.contribute_comp_counts();
}

class Main : public CBase_Main
{
    private:
    std::vector<double> reduction_times;
    double last_time;
    double begin_time;
    long reduction_count = 0;

    public:
    Main(CkArgMsg *m)
    {
        mainProxy = thisProxy;
        arr = CProxy_ReduceChares::ckNew(CkNumPes());
        last_time = CkWallTimer() + (0.1/1000);
        CcdCallFnAfter(start_reductions, (void *) this, 0.1);
        CcdCallFnAfter(end_time, (void *) this, 5000.0);
        begin_time = CkWallTimer();
        arr.self_computation();
    }
    void reduce()
    {
        reduction_count++;
        double current_time = CkWallTimer();
        reduction_times.push_back(current_time-last_time);
        last_time = current_time;
        arr.broadcast();
    }
    void end(long counts)
    {
        double time_elapsed = CkWallTimer() - begin_time;
        double time_sum = 0.0;
        for(int i=0; i<reduction_times.size(); i++)
        {
            time_sum += reduction_times[i];
        }
        ckout << "Number of reductions: " << reduction_count << endl;
        ckout << "Time elapsed: " << time_elapsed << endl;
        ckout << "Average time between reductions: " << time_sum / 100.0 << endl;
        ckout << "Computations per second: " << counts / time_elapsed << endl;
        ckout << "Seconds per computation: " << time_elapsed / counts << endl;
        CkExit(0);
    }
};

class ReduceChares : public CBase_ReduceChares
{
    private:
    long comp_counts = 0;
    double reduction_contribution_time = 0.0;

    public:
    ReduceChares(){
    }
    void self_computation()
    {
        comp_counts++;
        usleep(10);
        arr[thisIndex].self_computation();
    }
    void contribute_reduction()
    {
        CkCallback cb(CkReductionTarget(Main, reduce), mainProxy);
        contribute(0,NULL,CkReduction::nop, cb);
    }
    void broadcast()
    {
        contribute_reduction();
    }
    void contribute_comp_counts()
    {
        CkCallback cb(CkReductionTarget(Main, end), mainProxy);
        contribute(sizeof(long),&comp_counts,CkReduction::sum_long, cb);
    }

};

#include "reductiontest.def.h"