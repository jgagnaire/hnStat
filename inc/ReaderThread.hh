/*
** JG - hnStat
*/

#ifndef READERTHREAD_HH_
# define READERTHREAD_HH_

# include <sys/types.h>

# include "IQueryContainer.hh"
# include "SearchFilter.hh"

/*
** Functor class called from different threads
*/
class ReaderThread
{
public:
  ReaderThread(SearchFilter const &filter);
  ~ReaderThread();

  // Reads the datafile from start_off t end_off, populates _container and returns it
  std::shared_ptr<IQueryContainer> operator()(off_t start_off,
					      off_t end_off);
private:
  SearchFilter const &_filter;

  // Internal container specific to each thread
  std::shared_ptr<IQueryContainer> _container;
};

#endif // !READERTHREAD_HH_
