/*
** JG - hnStat
*/

#ifndef TOPXQUERYCONTAINER_HH_
# define TOPXQUERYCONTAINER_HH_

# include <unordered_map>
# include <cstdint>
# include <memory>
# include <string>
# include <map>

# include "IQueryContainer.hh"

// Container for the query type 'top X'
class TopXQueryContainer : public IQueryContainer
{
public:
  // Receives the query's timestamps, and the number of queries requested
  TopXQueryContainer(std::string const &lower_ts_bound,
		     std::string const &upper_ts_bound,
		     std::uint64_t top_queries_nb);
  virtual ~TopXQueryContainer();

  virtual void ingestQuery(std::string const &query);
  virtual void merge(std::shared_ptr<IQueryContainer> thread_container);
  virtual void postProcessData();
  virtual void printContent() const;

  std::unordered_map<std::string, std::uint64_t> const &getDataMap() const
  { return _datamap; }

private:
  std::string const _lower_ts_bound;
  std::string const _upper_ts_bound;
  std::uint64_t _top_queries_nb;

  std::unordered_map<std::string, std::uint64_t> _datamap;
  std::multimap<std::uint64_t, std::string, std::greater<std::uint64_t>> _data_multimap;
};

#endif // !TOPXQUERYCONTAINER_HH_
