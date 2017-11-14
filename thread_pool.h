#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <iostream>
#include <string>
#include <queue>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <boost/filesystem.hpp>

using namespace std;
using boost::filesystem::path;
using boost::filesystem::file_size;

unsigned int calculate_checksum(path p);
string file_handler(path p);

class ThreadPool
{
	private:
	struct task_struct
	{
		bool in_prog;
		bool done;
		path input;
		string output;
		
		task_struct(path p):
		input(p), output(""), in_prog(false), done(false)
		{}
	};

    mutex ms;
	mutex md;
    mutex m_active;
    condition_variable active_cond;
    ostream * out_taget;
	string (*task_function)(path);
	deque<task_struct> task_deque;

    public:
    ThreadPool(string (*t_function)(path), ostream * o_taget, int t_count);
    void taskWorker();
    void pushTasks(priority_queue<path> * input_q);
    task_struct * taskGet();
    void taskDone(task_struct * task);
};

#endif

