ThreadPool test project

Main task:

1. Each line in the log file should represents: short filename, size in human readable form, data of creation in human readable form (TIP: you may use BOOST::FILESYSTEM).
2. Component should only append lines into the log file.
3. Lines should be sorted by file names in alphabetical order.
4. In additional, it needs to calculate a per-byte sum (any other checksum is acceptable too) in 4 bytes variable (DWORD) for each file.
5. The calculation of checksum should be done asynchronously.
6. Using thread pool is a desirable solution (!!!).
7. Calculated information for each file should be logged as soon as possible, instead of at the end of processing of all files (sort order should be preserved).


Compile with boost:
g++ main.cpp thread_pool.cpp task_function.cpp -std=c++11 -I ../boost_1_65_1 -L ../boost_1_65_1/stage/lib/ -l boost_filesystem -l boost_system
