/*
** JG - hnStat
*/

#ifndef SEARCHFILTER_HH_
# define SEARCHFILTER_HH_

# include <sys/types.h>
# include <cstdint>
# include <string>

class SearchFilter
{
public:
  SearchFilter();
  ~SearchFilter();

  /*
  ** Parses the command-line arguments, and performs some basic sanity checks.
  **
  ** Populates the following member variables according to the parsed values:
  **  _distinct_query
  **  _top_queries_nb
  **  _lower_ts_bound
  **  _upper_ts_bound
  **  _datafile_name
  **
  ** args: The second argument given to the main() function
  **
  ** return: true if the given command-line arguments are correct,
  **         false otherwise.
  */
  bool parseCriteria(char **args);

  /*
  ** Gets information about the file given as argument on the command line,
  ** and performs sanity checks.
  **
  ** Populates the _datafile_size member variable with the file size.
  **
  ** return: true if the provided file is correct,
  **         false otherwise
  */
  bool setDatafileSize();

  // Returns true if the command is a distinct query, false otherwise
  bool isDistinctQuery() const { return _distinct_query; }

  // Returns the number of queries to output for a top queries command
  std::uint64_t getTopQueriesNumber() const { return _top_queries_nb; }

  // Returns the lower bound timestamp used for the search
  std::string const &getLowerTimestamp() const { return _lower_ts_bound; }

  // Returns the upper bound timestamp
  std::string const &getUpperTimestamp() const { return _upper_ts_bound; }

  // Returns the name of the file to be searched
  std::string const &getDatafileName() const { return _datafile_name; }

  // Returns the size of the file to be searched
  off_t getDatafileSize() const { return _datafile_size; }

  /*
  ** Compares two timestamps.
  **
  ** Leaves the timestamps as std::string for the comparison,
  ** so that no std::string-to-time_t conversion overhead is
  ** added to each comparison.
  **
  ** ts1: the first timestamp to compare
  ** ts2: the second timestamp
  **
  ** return:  a positive value if ts1 is greater than ts2,
  **          0 if ts1 and ts2 are equal,
  **          a negative value if ts1 is smaller than ts2
  */
  static int s_compare_timestamps(std::string const &ts1, std::string const &ts2);

private:
  bool _distinct_query;
  std::uint64_t _top_queries_nb;
  std::string _lower_ts_bound;
  std::string _upper_ts_bound;
  std::string _datafile_name;
  off_t _datafile_size;
};

#endif // !SEARCHFILTER_HH_
