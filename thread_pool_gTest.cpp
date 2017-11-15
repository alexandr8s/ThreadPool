#include "thread_pool.h"
#include "gtest/gtest.h"


string  task_function(string str)
{
    return str + " done\n";
}

TEST(ThreadPool, Create_instance)
{
    EXPECT_NO_THROW
    ({
            ThreadPool * tp = new ThreadPool(nullptr);
            delete tp;
    });
}

TEST(ThreadPool, Push_single_task)
{
    ostringstream string_stream;
    priority_queue<string> input_q;
    input_q.emplace("Task");
    ThreadPool tp(& task_function, & string_stream);
    tp.pushTasks(& input_q);
    while(!tp.isReady());

    EXPECT_EQ("Task done\n", string_stream.str());
}

TEST(ThreadPool, Push_tasks)
{
    ostringstream string_stream;
    priority_queue<string> input_q;
    input_q.emplace("Task 1");
    input_q.emplace("Task 3");
    input_q.emplace("Task 2");
    ThreadPool tp(& task_function, & string_stream);
    tp.pushTasks(& input_q);
    while(!tp.isReady());

    EXPECT_EQ("Task 1 done\nTask 2 done\nTask 3 done\n", string_stream.str());
}

int main(int argc, char **argv)
{
  printf("Running main() from Coder_gTest.cpp\n");

  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}