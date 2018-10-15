/*
** JG - hnStat
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <errno.h>

#include "SearchFilter.hh"

SearchFilter::SearchFilter()
  : _distinct_query(false),
    _top_queries_nb(0)
{
}

SearchFilter::~SearchFilter()
{
}

bool SearchFilter::parseCriteria(char **args)
{
  for (std::uint8_t idx = 1; args[idx] != nullptr; ++idx)
    {
      std::string s(args[idx]);

      if (s == "distinct")
	_distinct_query = true;
      else if (s == "top")
	{
	  _distinct_query = false;
	  if (args[idx + 1] == nullptr)
	    return false; // Nothing provided after 'top'
	  _top_queries_nb = std::strtoul(args[++idx], nullptr, 10);
	}
      else if (s == "--from" || s == "--to")
	{
	  if (args[idx + 1] == nullptr)
	    return false; // Nothing provided after '--from' or '--to'

	  std::string ts(args[++idx]);
	  // Remove possible leading zeros from timestamp
	  ts.erase(0, std::min(ts.find_first_not_of('0'), ts.size() - 1));

	  if (s[2] == 'f')
	    _lower_ts_bound = ts;
	  else
	    _upper_ts_bound = ts;
	}
      else
	_datafile_name = args[idx];
    }

  /*
  ** Sanity checks on the command, timestamps and filename:
  ** - both timestamps are provided, but lower bound is greater than upper bound
  ** - no number provided to a top X query
  ** - both distinct and top X commands were provided
  ** - no filename was provided
  */
  if (!_lower_ts_bound.empty() && !_upper_ts_bound.empty()
      && SearchFilter::s_compare_timestamps(_lower_ts_bound, _upper_ts_bound) > 0)
    return false;

  return ((_distinct_query && _top_queries_nb == 0)
	  || (!_distinct_query && _top_queries_nb > 0))
    && !_datafile_name.empty();
}

bool SearchFilter::setDatafileSize()
{
  struct stat st_buf = {};

  if (::stat(_datafile_name.c_str(), &st_buf) == -1)
    {
      std::cerr << "Error: stat() failed: " << ::strerror(errno) << std::endl;
      return false;
    }

  if (!S_ISREG(st_buf.st_mode))
    {
      std::cerr << "Error: file is not a regular file" << std::endl;
      return false;
    }
  if (st_buf.st_size == 0)
    {
      std::cerr << "Error: file is empty" << std::endl;
      return false;
    }

  _datafile_size = st_buf.st_size;
  return true;
}

int SearchFilter::s_compare_timestamps(std::string const &ts1, std::string const &ts2)
{
  if (ts1.size() > ts2.size())
    return 1;

  if (ts1.size() < ts2.size())
    return -1;

  return ts1.compare(ts2);
}
