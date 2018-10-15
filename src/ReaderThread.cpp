/*
** JG - hnStat
*/

#include <iostream>

#include "DistinctQueryContainer.hh"
#include "TopXQueryContainer.hh"
#include "ReaderThread.hh"
#include "File.hh"

ReaderThread::ReaderThread(SearchFilter const &filter)
  : _filter(filter)
{
  if (_filter.isDistinctQuery())
    _container.reset(new DistinctQueryContainer(_filter.getLowerTimestamp(),
						_filter.getUpperTimestamp()));
  else
    _container.reset(new TopXQueryContainer(_filter.getLowerTimestamp(),
					    _filter.getUpperTimestamp(),
					    _filter.getTopQueriesNumber()));
}

ReaderThread::~ReaderThread()
{
}

std::shared_ptr<IQueryContainer> ReaderThread::operator()(off_t start_off,
							  off_t end_off)
{
  File file(_filter.getDatafileName());
  std::string line;
  bool filter_lower_timestamp = !_filter.getLowerTimestamp().empty();
  bool filter_upper_timestamp = !_filter.getUpperTimestamp().empty();
  std::string timestamp;
  std::string query;
  std::size_t tab_idx;

  if (!file.isOpened() || !file.setOffset(start_off))
    return nullptr;

  while (start_off < end_off && file.getLine(line))
    {
      tab_idx = line.find('\t');

      if (tab_idx == std::string::npos || tab_idx + 1 >= line.size())
	{
	  std::cerr << "Warning: invalid line ignored: [" << line << "]" << std::endl;
	  goto incr_and_cont;
	}

      // If the search contains a filter on the query's timestamp
      if (filter_lower_timestamp || filter_upper_timestamp)
	{
	  timestamp = line.substr(0, tab_idx);
	  /*
	  ** Check if the timestamp of the current parsed
	  ** query matches the range supplied by command line
	  */
	  if ((filter_lower_timestamp
	       && SearchFilter::s_compare_timestamps(timestamp,
						     _filter.getLowerTimestamp()) < 0)
	      || (filter_upper_timestamp
		  && SearchFilter::s_compare_timestamps(timestamp,
							_filter.getUpperTimestamp()) > 0))
	    {
	      // Timestamp does not match, skip line
	      goto incr_and_cont;
	    }
	}

      query = line.substr(tab_idx + 1);
      _container->ingestQuery(query);

    incr_and_cont:
      start_off += line.size();
    }
  return _container;
}
