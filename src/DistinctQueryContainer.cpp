/*
** JG - hnStat
*/

#include <iostream>

#include "DistinctQueryContainer.hh"

DistinctQueryContainer::DistinctQueryContainer(std::string const &lower_ts_bound,
					       std::string const &upper_ts_bound)
  : _lower_ts_bound(lower_ts_bound),
    _upper_ts_bound(upper_ts_bound)
{
}

DistinctQueryContainer::~DistinctQueryContainer()
{
}

void DistinctQueryContainer::ingestQuery(std::string const &query)
{
  _dataset.insert(query);
}

void DistinctQueryContainer::merge(std::shared_ptr<IQueryContainer> thread_container)
{
  std::shared_ptr<DistinctQueryContainer> th_container_ptr;

  th_container_ptr = std::dynamic_pointer_cast<DistinctQueryContainer>(thread_container);
  if (th_container_ptr == nullptr)
    return ; // Downcast failed, programer's mistake?

  _dataset.insert(th_container_ptr->getDataSet().begin(),
		  th_container_ptr->getDataSet().end());
}

void DistinctQueryContainer::printContent() const
{
  std::cout << _dataset.size() << std::endl;
}
