#include "Database.hh"
#include "SQLiteException.hh"
#include "Indexer.hh"
#include "Searcher.hh"
#include "Configuration.hh"
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/progress.hpp>

namespace opt = boost::program_options;

namespace
{
  /*!
  ** Get the maximum beetween to integer.
  **
  ** @param a The first integer
  ** @param b The second integer
  **
  ** @return The greatest element
  */
  inline int max(int a, int b)
  {
    return a > b ? a : b;
  }

  /*!
  ** Index given item.
  **
  ** @param item A filename or directory path.
  **
  ** @return If indexation succeed
  */
  inline int indexItems(const std::string& item)
  {
    try
    {
      Index::Database& db = Index::Database::getInstance();
      Configuration& cfg = Configuration::getInstance();
      db.open(cfg.getDatabaseName());
      Index::Indexer idx;
      idx.setVerbose(cfg.getVerbose());
      idx.indexDirectory(item);
      db.close();
    }
    catch (SQLite::Exception& ex)
    {
      std::cerr << ex.errorMessage() << std::endl;
      return 3;
    }

    return 0;
  }

  /*!
  ** Search all documents matching the given request.
  **
  ** @param expression The expression to match
  **
  ** @return 0 if document found and there is no error, else another value
  */
  inline int search(const std::string& expression)
  {
    try
    {
      Index::Database& db = Index::Database::getInstance();
      Configuration& cfg = Configuration::getInstance();
      db.open(cfg.getDatabaseName());
      Search::Searcher searcher;
      searcher.search(expression);
      searcher.displayFoundDocument();
      db.close();
    }
    catch (SQLite::Exception& ex)
    {
      std::cerr << ex.errorMessage() << std::endl;
      return 3;
    }

    return 0;
  }

  /*!
  ** Parse all option and apply correct behavior.
  **
  ** @param argc Number of argument
  ** @param argv Arguments
  **
  ** @return If error occured
  */
  inline int parseOption(int argc, char** argv)
  {
    int res = 0;
    try
    {
      // Visible option
      opt::options_description desc("Allowed options");
      desc.add_options()
	("help,h", "Produce help message.")
	("verbose,v", "Active verbose mode.")
	("mode,m", opt::value<std::string>(),
	 "Behavior mode (indexer or searcher).")
	("database-location,d", opt::value<std::string>()->default_value("mydb.data"),
	 "Location of the sqlite3 database used to store inverse index. "
	 "Default is \"mydb.data\".")
	("stemmer-type,s", opt::value<std::string>()->default_value("frenchquick"),
	 "Type of stemmer (french or frenchquick). Default is frenchquick.")
	("stopwords-file,t", opt::value<std::string>()->default_value("StopWordList.txt"),
	 "File where the stop words are (default is \"StopWordList.txt\").")
	;

      // Invisible option, used for classic unnamed options
      opt::options_description hidden("Hidden options");
      hidden.add_options()
	("items", opt::value<std::vector<std::string> >(), "hidden")
	;
      opt::options_description cmdLine;
      cmdLine.add(desc).add(hidden);

      // Used for classic unnamed options
      opt::positional_options_description p;
      p.add("items", -1);

      // The parser handler
      opt::variables_map vm;
      opt::store(opt::command_line_parser(argc, argv). options(cmdLine).
		 positional(p).run(), vm);
      opt::notify(vm);

      // Show usage, if needed
      if (vm.count("help"))
      {
	std::cout << "Usage : \n\t--mode=indexer [--database-location] "
	  "[--stemmer-type] [--stopwords-file] [--verbose] items" <<
	  "\n\t--mode=searcher [--stemmer-type] [--stop-words-file] "
	  " [--verbose] expressions" <<
	  '\n';
	std::cout << desc << std::endl;
	return 1;
      }

      // Copy all property to the configuration singleton
      Configuration& cfg = Configuration::getInstance();
      cfg.setDatabaseName(vm["database-location"].as<std::string>());
      cfg.setStemmerName(vm["stemmer-type"].as<std::string>());
      cfg.setStopwordFilename(vm["stopwords-file"].as<std::string>());
      cfg.setVerbose(vm.count("verbose") > 0);

      if (vm.count("mode"))
      {
	cfg.setMode(vm["mode"].as<std::string>());
	if (vm["mode"].as<std::string>() == "indexer")
	{
	  if (vm.count("items"))
	  {
	    std::vector<std::string> opts = vm["items"].as<std::vector<std::string> >();
	    for (std::vector<std::string>::const_iterator iter = opts.begin();
		 iter != opts.end(); ++iter)
	      res = max(res, indexItems(*iter));
	  }
	  else
	  {
	    std::cerr << "Error : You must specify which files or directories to index." << std::endl;
	    return 2;
	  }
	}
	else
	  if (vm["mode"].as<std::string>() == "searcher")
	  {
	    if (vm.count("items"))
	    {
	      std::vector<std::string> opts = vm["items"].as<std::vector<std::string> >();
	      for (std::vector<std::string>::const_iterator iter = opts.begin();
		   iter != opts.end(); ++iter)
		res = max(res, search(*iter));
	    }
	    else
	    {
	      std::cerr << "Error : You must specify at least one search request." << std::endl;
	      return 2;
	    }
	  }
	  else
	  {
	    std::cerr << vm["mode"].as<std::string>() << " : Unknow mode" << std::endl;
	    return 2;
	  }
      }
      else
      {
	std::cout << "The mode must be specified with --mode\n";
	return 2;
      }
    }
    catch (opt::error& option)
    {
      std::cerr << "Error :  " << option.what() << std::endl;
      return 2;
    }

    return res;
  }
}

/*!
** Main function.
**
** @param argc Number of argument
** @param argv Arguments
**
** @return If error occured
*/
int main(int argc, char **argv)
{
  boost::timer t;
  int res = parseOption(argc, argv);
  std::cerr << "\nTime elapsed: " << t.elapsed() << " seconds." << std::endl;
  return res;
}
