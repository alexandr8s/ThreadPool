#include "task_function.h"

using boost::filesystem::path;
using boost::filesystem::file_size;

string file_handler(string target)
{
	path p = target;
	string file_info = "File " + p.stem().string();
	file_info += " size " + to_string(file_size(p));
	
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

	file_info += " checksum " + to_string(checksum);

	time_t lt = last_write_time(p);
	file_info += " date " + string(ctime(&lt));

    return file_info;
}