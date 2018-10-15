/*
** JG - hnStat
*/

#include <iostream>
#include <memory>

#include "QueryProcessor.hh"
#include "SearchFilter.hh"

int main(int, char **av)
{
  SearchFilter filter;

  if (!filter.parseCriteria(av))
    {
      std::cerr << "Usage: hnStat [distinct|top N] [--from TIMESTAMP]"
	" [--to TIMESTAMP] [file]" << std::endl;
      return -1;
    }
  if (!filter.setDatafileSize())
    return -2;

  QueryProcessor queryproc(filter);

  queryproc.map();
  queryproc.reduce();
  queryproc.printResult();

  return 0;
}
