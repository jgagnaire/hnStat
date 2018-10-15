/*
** JG - hnStat
*/

#ifndef IQUERYCONTAINER_HH_
# define IQUERYCONTAINER_HH_

# include <memory>

/*
** Interface to be inherited and implemented by each new query type
** that requires a new nature of internal container and processing.
*/
class IQueryContainer
{
public:
  virtual ~IQueryContainer() {}

  /*
  ** Receives the query read from the datafile as parameter,
  ** and ingest it in the internal member container
  */
  virtual void ingestQuery(std::string const &query) = 0;

  /*
  ** Receives a pointer to a container, and inserts the data pointed
  ** in the internal member container.
  **
  ** Called after all calls to ingestQuery()
  */
  virtual void merge(std::shared_ptr<IQueryContainer> thread_container) = 0;

  /*
  ** Applies post-processing on the internal container after merge() if needed.
  **
  ** Called after the call to merge()
  */
  virtual void postProcessData() = 0;

  // Formats and displays the content of the internal member container on stdout
  virtual void printContent() const = 0;
};

#endif // !IQUERYCONTAINER_HH_
