/*
** JG - hnStat
*/

#ifndef QUERYPROCESSOR_HH_
# define QUERYPROCESSOR_HH_

# include <sys/types.h>
# include <unistd.h>
# include <cstdint>
# include <future>
# include <vector>
# include <memory>

# include "IQueryContainer.hh"
# include "SearchFilter.hh"
# include "ReaderThread.hh"
# include "File.hh"

/*
** Processes the query in 3 distinct steps:
**
**  - map(), which divides the file to be processed among threads
**     and launches the processing,
**
**  - reduce(), in which the data structure resulting from each thread's work
**     is retrieved and merged into this class' internal _container member variable
**
**  - printResult(), that formats and displays the request's result
**     on the standard output
*/
class QueryProcessor
{
public:
  /*
  ** Receives:
  **  - filter, the SearchFilter object representing the criteria for the request,
  **  - max_threads_nb, an integer representing the number of online CPU cores,
  **  - min_thread_part_size, an integer used as a minimal threshold for the number
  **     of bytes from the file processed by each thread of execution
  */
  QueryProcessor(SearchFilter const &filter,
		 std::uint64_t max_threads_nb = sysconf(_SC_NPROCESSORS_ONLN),
		 off_t min_thread_part_size = 4096);
  ~QueryProcessor();

  // Divides the file among threads and launches each of them
  void map();

  // Retrieves the result of each thread's work
  void reduce();

  // Formats and displays request's result
  void printResult() const;

private:
  // Computes and returns the coarse number of bytes to be processed by each thread
  std::uint64_t _compute_thread_part_size() const;

  // Returns the exact end_offset to be given to each thread, rounded to the next line 
  off_t _get_next_end_offset(File const &file, off_t end,
			     off_t file_size);

  SearchFilter const &_filter;
  std::uint64_t _max_threads_nb;
  off_t _min_thread_part_size;

  // Stores the references to each thread's work result
  std::vector<std::future<std::shared_ptr<IQueryContainer>>> _threads_futures;

  // Internal container that will eventually contain the results of each thread
  std::unique_ptr<IQueryContainer> _container;
};

#endif // !QUERYPROCESSOR_HH_
