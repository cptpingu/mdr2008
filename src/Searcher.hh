#ifndef SEARCHER_HH_
# define SEARCHER_HH_

# include <iostream>
# include <list>
# include "Column.hh"
# include "Database.hh"
# include "Configuration.hh"
# include "RequestParser.hh"
# include "DateUtils.hh"
# include "ArrayUtils.hh"

namespace Search
{
  class Searcher
  {
    typedef std::list< ::Index::Column::DocumentResult> array;
    typedef array::iterator iter;
    typedef array::const_iterator citer;

  public:
    Searcher();
    ~Searcher();

  public:
    void search(const std::string& request);
    void displayFoundDocument(std::ostream& o = std::cout) const;

  public:
    void clean();
    const array& getDocumentList() const;

  private:
    void evaluateRequest(iter_t const& i);

  private:
    array		_docFound;
  };
}

# include "Searcher.hxx"

#endif /* !SEARCHER_HH_ */
