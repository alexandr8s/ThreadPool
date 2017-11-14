#include <fstream>
#include <boost/filesystem.hpp>
#include "task_function.h"
#include "thread_pool.h"


#define log_file "log.txt"

using namespace std;

using boost::filesystem::path;
using boost::filesystem::is_directory;
using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;


int main()
{
    ofstream log_stream;
    log_stream.open (log_file, fstream::app);

    ThreadPool tp(& file_handler, & log_stream);

    while (true)
    {
        cout << "Enter a global path to target folder: " << endl;
        string str_path;
        getline(cin, str_path);
        path dir = path(str_path);

        if (is_directory(dir))
        {
            priority_queue<string> files;
            for (auto & file : directory_iterator(dir))
            {
                files.emplace(file.path().string()); 
            }
            tp.pushTasks(&files);
        }
        else
        {
            cout << "Provided path is not a directory - exiting" << endl;
            break;
        }
    }
    log_stream.close();
}