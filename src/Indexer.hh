#ifndef INDEXER_HH_
# define INDEXER_HH_

# include <iostream>
# include <fstream>
# include <sstream>
# include <boost/filesystem/path.hpp>
# include <boost/tokenizer.hpp>
# include <boost/regex.hpp>
# include <list>
# include "Column.hh"
# include "Stemmer.hh"
# include "Database.hh"

namespace fs = boost::filesystem;

namespace Index
{
  namespace Weight
  {
    static const double NO		= 0;
    static const double DEFAULT		= 1;
    static const double TITLE		= 3;
    static const double H_TITLE		= 2;
    static const double KEYWORDS	= 2.5;
    static const double DESCRIPTION	= 2.5;
    static const double REAL		= 0.3;
    static const double STEM		= 1;
  }

  typedef boost::regex regexp;
  typedef boost::wregex wregexp;
  static const std::string COMMENT = "<!--.*?-->";
  static const std::string TITLE = "title";
  static const std::string SCRIPT = "<\\s*(script)[^>]*>\\s*"
    "([^>]*)"
    "\\s*<\\s*/\\s*\\1\\s*>";
  static const std::string HEAD_TITLE = "h[123456]";
  static const std::string KWORDS = "<\\s*meta\\s*"
    "name\\s*=\\s*\"keywords\"\\s*"
    "content\\s*=\\s*\"([^\"]*)\"\\s*>";
  static const std::string DESC = "<\\s*meta\\s*"
    "name\\s*=\\s*\"description\"\\s*"
    "content\\s*=\\s*\"([^\"]*)\"\\s*>";
  static const std::string HTML_BALISES = "<[^>]*>";
  static const std::string IMG_TO_ALT = "<\\s*img"
    "[^>]*alt\\s*=\\s*\"([^\"]*)\"[^>]*>";
  static const regexp QUOTE_PATTERN("[]?.+*$()\\^|!#{}[-]",
				    boost::regex::perl |
				    boost::regex::icase);

  class Indexer
  {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    typedef std::list<regexp*> reglist;
    typedef std::list<std::string> wordsList;
    typedef std::list<regexp*>::iterator iter;
    typedef std::list<regexp*>::const_iterator citer;

  public:
    Indexer();
    ~Indexer();

  public:
    void indexDirectory() const;
    void indexDirectory(const std::string& dirname) const;
    void setVerbose(const bool activate);
    void processFile(const fs::path& fullPath) const;
    void processFile(const std::string& fullPath) const;
    void loadBlackList() const;
    void cleanBlackList() const;
    void loadWhiteList() const;
    void cleanWhiteList() const;
    void loadStopWords(const std::string& filename) const;
    void loadStopWords(const fs::path& filename) const;

  private:
    std::string preg_quote (const std::string& expr) const;
    std::string extractFromExpression(std::string& text,
				      const std::string& expression,
				      const unsigned int capture = 0) const;
    std::string extractFromTwoSymbols(std::string& text,
				      const std::string& beginSymbol,
				      const std::string& expression,
				      const std::string& endSymbol) const;
    std::string extractFromBalises(std::string& text, const std::string& balise) const;
    void deleteExpr(std::string& text,
		    const std::string& expr,
		    const boost::match_flag_type flags) const;
    void replaceImgWithAlt(std::string& text) const;
    void replaceSpecialHTMLChar(std::string& text) const;
    bool isStopWord(const std::string& word) const;

  private:
    void listDirectory(const fs::path& fullPath) const;
    bool isBlackListed(const std::string& filename) const;
    bool isWhiteListed(const std::string& filename) const;
    unsigned int extractAllTerm(const std::string& fullPath, File::type type) const;
    unsigned int extractAllTermFromText(std::stringstream& file) const;
    unsigned int extractAllTermFromHTML(std::stringstream& file) const;
    unsigned int extractLineTerm(const std::string& line) const;
    void commitWordAndTerm(std::string& word) const;
    Column::Term commitTerm(const std::string& term) const;

  private:
    mutable unsigned int	_currentIdDoc;
    mutable double		_weight;
    mutable reglist		_blackList;
    mutable reglist		_whiteList;
    mutable wordsList		_stopWords;
    bool			_verbose;
    Stemmer::Generic*		_stem;
  };
}

# include "Indexer.hxx"

#endif /* !INDEXER_HH_ */
