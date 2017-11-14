#include "thread_pool.h"

static bool cond_ready = false;


string file_handler(path p)
{
	string file_info = string(p.stem().string()) + " ";
	file_info += to_string(file_size(p)) + " ";
	
	ifstream file;
	file.open(p.c_str());
    unsigned int checksum(0), shift(0);
    for (unsigned int ch = file.get(); file; ch = file.get()) 
	{
        checksum += (ch << shift);
        shift += 8;
        if (shift == 32) shift = 0;
    }
	file.close();

	file_info += to_string(checksum) + " ";

	time_t lt = last_write_time(p);
	file_info += ctime(&lt);

    return file_info;
}


ThreadPool::ThreadPool(string (*t_function)(path), ostream * o_taget, int thread_count = 10):
task_function(t_function), out_taget(o_taget)
{
	for(int i = 0; i < thread_count; i++)
	{
		thread t(&ThreadPool::taskWorker, this);
		t.detach();
	}
}

void ThreadPool::taskWorker()
{
	while(true)
	{
		unique_lock<mutex> lock(m_active);
		active_cond.wait(lock, [] () {return cond_ready;});

		task_struct * task = taskGet();
		if (task)
		{
			string out = task_function(task->input);
			task->output = out;
			taskDone(task);
		}
		else
		{
			cond_ready = false;
		}
	}
}
	
void ThreadPool::pushTasks(priority_queue<path> * input_q)
{
	while (!input_q->empty())
	{
		task_deque.push_front(task_struct(input_q->top()));
		input_q->pop();
	}
	cond_ready = true;
	active_cond.notify_all();
}
	
ThreadPool::task_struct * ThreadPool::taskGet()
{
	std::lock_guard<std::mutex> lock(ms);
	for (auto & task_struct : task_deque)
	{
		if (!task_struct.in_prog)
		{
			task_struct.in_prog = true;
			return & task_struct;
		}
	}
	return nullptr;
}

void ThreadPool::taskDone(task_struct * task)
{
	std::lock_guard<std::mutex> lock(md);
	task->done = true;
	while (!task_deque.empty())
	{
		if (task_deque.front().done)
		{
			*out_taget << task_deque.front().output;
			task_deque.pop_front();
		}
		else
		{
			break;
		}
	}
}
