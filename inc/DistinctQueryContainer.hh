/*
** JG - hnStat
*/

#ifndef DISTINCTQUERYCONTAINER_HH_
# define DISTINCTQUERYCONTAINER_HH_

# include <unordered_set>
# include <memory>
# include <string>

# include "IQueryContainer.hh"

// Container for the query type 'distinct'
class DistinctQueryContainer : public IQueryContainer
{
public:
  // Receives the lower and upper bound timestamps as parameter
  DistinctQueryContainer(std::string const &lower_ts_bound,
			 std::string const &upper_ts_bound);
  virtual ~DistinctQueryContainer();

  virtual void ingestQuery(std::string const &query);
  virtual void merge(std::shared_ptr<IQueryContainer> thread_container);
  virtual void postProcessData() {}
  virtual void printContent() const;

  std::unordered_set<std::string> const &getDataSet() const
  { return _dataset; }

private:
  std::string const _lower_ts_bound;
  std::string const _upper_ts_bound;

  std::unordered_set<std::string> _dataset;
};

#endif // !DISTINCTQUERYCONTAINER_HH_
