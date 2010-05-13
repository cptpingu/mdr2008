#include "Searcher.hh"
#include "ParseException.hh"
#include "Column.hh"

namespace Search
{
  /*!
  ** Construct a search object.
  */
  Searcher::Searcher()
  {
  }

  /*!
  ** Destruct the search object.
  */
  Searcher::~Searcher()
  {
    clean();
  }

  /*!
  ** Display to screen all results.
  */
  void
  Searcher::displayFoundDocument(std::ostream& o) const
  {
    Configuration& cfg = Configuration::getInstance();
    o << "Found Document (" << _docFound.size() << "):" << std::endl;

    if (cfg.getVerbose())
      for (citer i = _docFound.begin(); i != _docFound.end(); ++i)
      {
	::operator<<(o, *i);
	o << std::endl;
      }
    else
      for (citer i = _docFound.begin(); i != _docFound.end(); ++i)
	o << i->filename << " - " <<
	  DateUtils::timestampToStringDate(i->date) <<
	  " - " << i->rank << "%" << std::endl;
  }

  /*!
  ** Find and stock all expression found.
  **
  ** @param request The document search request
  */
  void
  Searcher::search(const std::string& request)
  {
    //     const std::string dbg = ":date(34/34/34-34/34/34) + "
    //       ":date(> 45/45/45) :date(< 45/45/45) myexpr + rere OR "
    //       "dedede AND (dedede OR dedede AND dede) - word + \"Some expre + dede toto\"";

    std::string clean = "";
    Request::Parser parser(request);
    try
    {
      //Request::Parser parser(dbg);
      parser.parseQuery();
      clean = parser.toString();
      Configuration& cfg = Configuration::getInstance();
      if (cfg.getVerbose())
      {
	std::cout << parser.toXML() << std::endl;
	std::cout << "Clean request : " << clean << std::endl << std::endl;
      }
    }
    catch (const Request::ParseException& ex)
    {
      std::cerr << "An error occured when parsing request." << std::endl <<
	"Last tokens match are : " << ex.what() << std::endl;
      return;
    }

    // Check if a similar search was already done.
    // If so, just get previous result.
    Index::Database& db = Index::Database::getInstance();
    unsigned int id = db.getSimilarRequest(clean);
    if (id == 0)
    {
      evaluateRequest(parser.getTree());
      _docFound.sort();
      db.saveResults(_docFound, clean);
    }
    else
      _docFound = db.getCachedSearchResult(id);
  }
}
