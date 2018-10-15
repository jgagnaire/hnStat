/*
** JG - hnStat
*/

#include <algorithm>
#include <iostream>

#include "TopXQueryContainer.hh"

TopXQueryContainer::TopXQueryContainer(std::string const &lower_ts_bound,
				       std::string const &upper_ts_bound,
				       std::uint64_t top_queries_nb)
  : _lower_ts_bound(lower_ts_bound),
    _upper_ts_bound(upper_ts_bound),
    _top_queries_nb(top_queries_nb)
{
}

TopXQueryContainer::~TopXQueryContainer()
{
}

void TopXQueryContainer::ingestQuery(std::string const &query)
{
  ++_datamap[query];
}

void TopXQueryContainer::merge(std::shared_ptr<IQueryContainer> thread_container)
{
  std::shared_ptr<TopXQueryContainer> th_container_ptr;

  th_container_ptr = std::dynamic_pointer_cast<TopXQueryContainer>(thread_container);
  if (th_container_ptr == nullptr)
    return ; // Downcast failed, programer's mistake?

  _datamap.insert(th_container_ptr->getDataMap().begin(),
		  th_container_ptr->getDataMap().end());
}

void TopXQueryContainer::postProcessData()
{
  std::transform(_datamap.begin(), _datamap.end(),
		 std::inserter(_data_multimap, _data_multimap.begin()),
		 [] (std::pair<std::string, std::uint64_t> const &p) {
		   /*
		   ** An optimization there could be to to somehow erase 'p'
		   ** from _datamap to save memory. But this would also add
		   ** CPU overhead since std::unordered_map::erase() has a
		   ** worst-case complexity of O(n) with n the number of elements
		   */
		   return std::pair<std::uint64_t, std::string>(p.second, p.first);
		 });
}

void TopXQueryContainer::printContent() const
{
  std::uint64_t queries_printed = 0;

  for (auto const &keyval : _data_multimap)
    {
      std::cout << keyval.second << " " << keyval.first << std::endl;
      if (++queries_printed == _top_queries_nb)
	return ;
    }
}
