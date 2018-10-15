/*
** JG - hnStat
*/

#include <algorithm>

#include "DistinctQueryContainer.hh"
#include "TopXQueryContainer.hh"
#include "QueryProcessor.hh"
#include "File.hh"

/*
** We consider that max_threads_nb and min_thread_part_size
** have correct values, since they are either set to their
** default values, or set by the programmer.
*/
QueryProcessor::QueryProcessor(SearchFilter const &filter,
			       std::uint64_t max_threads_nb,
			       off_t min_thread_part_size)
  : _filter(filter),
    _max_threads_nb(max_threads_nb),
    _min_thread_part_size(min_thread_part_size)
{
  if (_filter.isDistinctQuery())
    _container.reset(new DistinctQueryContainer(_filter.getLowerTimestamp(),
						_filter.getUpperTimestamp()));
  else
    _container.reset(new TopXQueryContainer(_filter.getLowerTimestamp(),
					    _filter.getUpperTimestamp(),
					    _filter.getTopQueriesNumber()));
}

QueryProcessor::~QueryProcessor()
{
}

void QueryProcessor::map()
{
  std::uint64_t extra_bytes;
  File file(_filter.getDatafileName());
  off_t start_offset = 0,
    thread_part_size = _compute_thread_part_size();

  extra_bytes = _filter.getDatafileSize() % thread_part_size;
  while (start_offset < _filter.getDatafileSize())
    {
      off_t end_offset = start_offset + thread_part_size;

      if (extra_bytes > 0)
	{
	  ++end_offset;
	  --extra_bytes;
	}
      end_offset = _get_next_end_offset(file, end_offset, _filter.getDatafileSize());

      /*
      ** Launches a thread through ReaderThread::operator(),
      ** passes start_offset and end_offset as parameter,
      ** and stores the std::future resulting from the thread
      ** launch in an std::vector
      */
      _threads_futures.push_back(std::async(std::launch::async, ReaderThread(_filter),
					    start_offset, end_offset));
      start_offset = end_offset + 1;
    }
}

void QueryProcessor::reduce()
{
  /*
  ** Goes through each std::future in the std::vector, and get()s the
  ** container resulting from the thread's work in a lambda function.
  **
  ** The thread's container retrieved is then merged in the final
  ** global container through a call to _container->merge()
  */
  std::for_each(_threads_futures.begin(), _threads_futures.end(),
		[this](std::future<std::shared_ptr<IQueryContainer>> &fut){
		  _container->merge(fut.get());
		});
  _container->postProcessData();
}

void QueryProcessor::printResult() const
{
  _container->printContent();
}

std::uint64_t QueryProcessor::_compute_thread_part_size() const
{
  if (_filter.getDatafileSize() < _min_thread_part_size)
    return _filter.getDatafileSize();

  std::uint64_t threads_nb = _max_threads_nb;
  off_t thread_part_size = _filter.getDatafileSize() / threads_nb;

  while (threads_nb > 2 && thread_part_size < _min_thread_part_size)
    {
      --threads_nb;
      thread_part_size = _filter.getDatafileSize() / threads_nb;
    }
  return thread_part_size;
}

off_t QueryProcessor::_get_next_end_offset(File const &file, off_t end,
					   off_t file_size)
{
  std::string line;

  if (end >= file_size)
    return file_size;

  file.setOffset(end);
  file.getLine(line);
  return end + line.size();
}
