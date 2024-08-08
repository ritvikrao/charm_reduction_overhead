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

    public:
    Main(CkArgMsg *m)
    {
        mainProxy = thisProxy;
        arr = CProxy_ReduceChares::ckNew(CkNumPes());
        last_time = CkWallTimer() + (0.1/1000);
        //CcdCallFnAfter(start_reductions, (void *) this, 0.1);
        CcdCallFnAfter(end_time, (void *) this, 5000.0);
        begin_time = CkWallTimer();
        arr.self_computation();
    }
    void reduce()
    {
        double current_time = CkWallTimer();
        reduction_times.push_back(current_time-last_time);
        last_time = current_time;
        if(reduction_times.size()<100) arr.broadcast();
        else
        {
            double time_sum = 0.0;
            for(int i=0; i<reduction_times.size(); i++)
            {
                time_sum += reduction_times[i];
            }
            ckout << "Average reduction time: " << time_sum / 100.0 << endl;
            arr.contribute_comp_counts();
        }
    }
    void end(long counts)
    {
        double time_elapsed = CkWallTimer() - begin_time;
        ckout << "Computations per second: " << counts / time_elapsed << endl;
        ckout << "Seconds per computation: " << time_elapsed / counts << endl;
        CkExit(0);
    }
};

class ReduceChares : public CBase_ReduceChares
{
    private:
    long comp_counts = 0;

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