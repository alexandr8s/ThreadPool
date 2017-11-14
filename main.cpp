#include <fstream>
#include "thread_pool.h"

#define log_file "log.txt"

using namespace std;

using boost::filesystem::is_directory;
using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;


int main()
{
    while (true)
    {
        ofstream log_stream;
        log_stream.open (log_file);
        ThreadPool tp(& file_handler, & log_stream, 10);

        cout << "Enter a global path to target folder: " << endl;
        string str_path;
        getline(cin, str_path);
        path p = path(str_path);
  
        if (is_directory(p))
        {
            priority_queue<path> files;
            for (directory_entry & de : directory_iterator(p))
            {
                files.emplace(de.path()); 
            }
            tp.pushTasks(&files);
        }
        else
        {
            cout << "Provided path " << p << " is not a directory - exiting" << endl;
            break;
        }
        log_stream.close();
    }
}