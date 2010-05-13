#include <boost/filesystem/operations.hpp>
#include <algorithm>
#include "Indexer.hh"
#include "Utils.hh"
#include "Sha1.hh"
#include "StemmerFactory.hh"
#include "Configuration.hh"

namespace Index
{
  /*!
  ** Construct an indexer object.
  */
  Indexer::Indexer()
    : _currentIdDoc(0), _weight(Weight::NO), _stem(0)
  {
    Stemmer::StemmerFactory factory;
    Configuration& cfg = Configuration::getInstance();
    _stem = factory.get(cfg.getStemmerName());
    loadBlackList();
    loadWhiteList();
    fs::path full_path(fs::initial_path<fs::path>());
    loadStopWords(full_path / cfg.getStopwordFilename());
  }

  /*!
  ** Destruct an indexer object.
  */
  Indexer::~Indexer()
  {
    delete _stem;
    cleanBlackList();
    cleanWhiteList();
  }

  /*!
  ** List a directory recursively, and apply to each file the processFile method.
  ** Hidden file, ie file beginning with ".", will be ignored.
  **
  ** @param fullPath The full boost path of the directory
  */
  void
  Indexer::listDirectory(const fs::path& fullPath) const
  {
    assert(fs::is_directory(fullPath));

    fs::directory_iterator end;
    for (fs::directory_iterator it(fullPath); it != end; ++it)
    {
      try
      {
	// Check if it's a hidden file
	if (it->leaf()[0] != '.')
	{
	  if (fs::is_directory(it->status()))
	    listDirectory(fullPath / it->leaf());
	  else
	    if (fs::is_regular(it->status()))
	      processFile(fullPath / it->leaf());
	}
      }
      catch (const std::exception & ex)
      {
	std::cerr << it->leaf() << " : " << ex.what() << std::endl;
      }
    }
  }

  /*!
  ** Index the current directory.
  */
  void
  Indexer::indexDirectory() const
  {
    fs::path full_path(fs::initial_path<fs::path>());
    indexDirectory(full_path.native_file_string());
  }

  /*!
  ** Index a directory, ie browse all on hidden files recursively in
  ** all subfolders, then process it.
  **
  ** @param dirname The name of the directory where the files to index were
  */
  void
  Indexer::indexDirectory(const std::string& dirname) const
  {
    Index::Database& db = Index::Database::getInstance();
    db.clearSearchCache();
    fs::path full_path = fs::system_complete(fs::path(dirname, fs::native));

    if (!fs::exists(full_path))
    {
      std::cerr << "Not found: " << full_path.native_file_string() << std::endl;
      return;
    }

    if (fs::is_directory(full_path))
      listDirectory(full_path);
    else
      if (fs::is_regular(full_path))
	processFile(full_path);
      else
	std::cerr << "Not a directory or a file: " << full_path.native_file_string() << std::endl;
  }

  /*!
  ** Process a file, indexing it, ie getting all information needed.
  **
  ** @param fullPath The boost full file path
  */
  void
  Indexer::processFile(const fs::path& fullPath) const
  {
    processFile(fullPath.native_file_string());
  }

  /*!
  ** Process a file, indexing it, ie getting all information needed.
  **
  ** @param fullPath The full file path
  */
  void
  Indexer::processFile(const std::string& fullPath) const
  {
    Index::Database& db = Index::Database::getInstance();
    if (_verbose)
      std::cout << "Processing : " << fullPath << std::endl;

    if (!fs::is_regular(fullPath))
    {
      Column::Document doc = db.getDocumentByFilename(fullPath);
      if (Column::docExists(doc))
	db.deleteDocument(doc, true);
      return;
    }

    if (!isWhiteListed(fullPath) || isBlackListed(fullPath))
      return;

    // First we get the SHA1 of this file
    Hash::Sha1 sha1;
    if (!sha1.hashFile(fullPath.c_str()))
      assert(false);
    const std::string hash = sha1.getStrHash();

    // Then we try to get the document in the database
    Column::Document doc = db.getDocumentByFilename(fullPath);

    // If document exists, then delete entry in database to take care of modification
    if (Column::docExists(doc))
    {
      // Check if document can be skipped
      if (hash == doc.hash)
	return;
      db.deleteDocument(doc, false);
    }

    // Get the file type : TEXT or HTML
    std::string ext = fullPath.substr(fullPath.find_last_of('.') + 1);
    File::type t = ext == "html" || ext == "htm" ? File::HTML : File::TEXT;

    // Get the file system date
    std::stringstream date;
    date << fs::last_write_time(fullPath);

    // Add the document without specifies length
    doc.filename = fullPath;
    doc.type = t;
    doc.hash = hash;
    doc.date = date.str();
    db.addOrUpdateDocument(doc);

    // Get the id of the file, then process file to extract all term,
    // and get the total term count
    doc = db.getDocumentByFilename(fullPath);
    _currentIdDoc = doc.id;
    unsigned int length = extractAllTerm(fullPath, t);
    _currentIdDoc = 0;

    // Update the previous document, with the length value.
    doc.length = length;
    db.addOrUpdateDocument(doc);

    // Update score, ie divide all score by doc number (length)
    db.updateTermScore(doc);
  }

  /*!
  ** Extract all term of a document.
  **
  ** @param fullPath The path where the document is
  ** @param type The type of document
  **
  ** @return Number of term in document, including black listed ones.
  */
  unsigned int
  Indexer::extractAllTerm(const std::string& fullPath, File::type type) const
  {
    std::stringstream buffer;
    std::ifstream file(fullPath.c_str());
    assert(file);
    buffer << file.rdbuf();
    file.close();

    Index::Database& db = Index::Database::getInstance();
    db.beginTransaction();
    unsigned int termCount = 0;
    switch (type)
    {
      case File::TEXT:
	termCount = extractAllTermFromText(buffer);
	break;
      case File::HTML:
	termCount = extractAllTermFromHTML(buffer);
	break;
      default:
	assert(false);
    }
    db.endTransaction();

    return termCount;
  }

  /*!
  ** Extract all term contained in a string stream in simple text format.
  **
  ** @param file The text of the file
  **
  ** @return Numbers of term found
  */
  unsigned int
  Indexer::extractAllTermFromText(std::stringstream& file) const
  {
    std::string line;
    int termCount = 0;

    _weight = Weight::DEFAULT;
    while (std::getline(file, line))
      termCount += extractLineTerm(Utils::renarrow(line));
    _weight = Weight::NO;

    return termCount;
  }

  /*!
  ** Extract all term contained in a string stream in HTML format.
  **
  ** @param file The text of the file
  **
  ** @return Numbers of term found
  */
  unsigned int
  Indexer::extractAllTermFromHTML(std::stringstream& file) const
  {
    std::string line;
    int termCount = 0;
    std::string allFile = Utils::renarrow(file.str());
    std::string extracted;

    // Delete all comment and script
    deleteExpr(allFile, COMMENT, boost::match_default);
    deleteExpr(allFile, SCRIPT, boost::match_default);

    //Extract <title> balise
    _weight = Weight::TITLE;
    extracted = extractFromBalises(allFile, TITLE);
    replaceSpecialHTMLChar(extracted);
    termCount += extractLineTerm(extracted);

    _weight = Weight::H_TITLE;
    //Extract <h1>, <h2>, etc... balises
    extracted = extractFromBalises(allFile, HEAD_TITLE);
    replaceSpecialHTMLChar(extracted);
    termCount += extractLineTerm(extracted);

    _weight = Weight::KEYWORDS;
    // Extract a <meta name="keywords"> balise
    extracted = extractFromExpression(allFile, KWORDS, 1);
    replaceSpecialHTMLChar(extracted);
    termCount += extractLineTerm(extracted);

    _weight = Weight::DESCRIPTION;
    // Extract a <meta name="description"> balise
    extracted = extractFromExpression(allFile, DESC, 1);
    replaceSpecialHTMLChar(extracted);
    termCount += extractLineTerm(extracted);

    // Replace img balise with it alt property
    replaceImgWithAlt(allFile);

    // Now delete all balises
    deleteExpr(allFile, HTML_BALISES, boost::match_default);
    replaceSpecialHTMLChar(allFile);
    termCount += extractLineTerm(extracted);

    _weight = Weight::DEFAULT;
    termCount += extractLineTerm(allFile);
    _weight = Weight::NO;

    return termCount;
  }

  /*!
  ** Extract all term contained within a single line.
  **
  ** @param line The line where the terms are
  **
  ** @return Number of term found including black listed ones.
  */
  unsigned int
  Indexer::extractLineTerm(const std::string& line) const
  {
    int termCount = 0;
    tokenizer tokens(line);
    std::string tmp;
    for (tokenizer::iterator tok_iter = tokens.begin();
	 tok_iter != tokens.end(); ++tok_iter)
    {
      tmp = *tok_iter;
      if (tmp.length() > 1 && !std::ispunct(tmp[0]) && !isStopWord(tmp))
      {
	commitWordAndTerm(tmp);
	termCount++;
      }
    }

    return termCount;
  }

  /*!
  ** Add the word to the word list, and also update term list.
  **
  ** @param word The word to commit
  */
  void
  Indexer::commitWordAndTerm(std::string& word) const
  {
    assert(_currentIdDoc != 0);
    assert(_weight != Weight::NO);

    // Lowerize the word.
    // Ugly hack with static_cast, but compiler will failed without it !
    std::transform(word.begin(), word.end(), word.begin(),
		   static_cast<int(*)(int)>(std::tolower));

    Column::Term term = commitTerm(word);
    assert(termExists(term));

    // If term already exists, then just increments realCount and stemCount.
    // If term doesn't exists, stem the term and check if another stem was found,
    // then increments stemCount.
    Index::Database& db = Index::Database::getInstance();
    Column::Word w = db.getWordByIds(_currentIdDoc, term.id);
    if (!Column::wordExists(w))
    {
      w.idDocument = _currentIdDoc;
      w.idTerm = term.id;
      w.weight = _weight;
      w.realCount = 1;
      w.score = (w.weight * (w.realCount * Weight::REAL + w.stemCount * Weight::STEM));
      db.addWord(w);
    }
    else
    {
      w.realCount++;
      w.weight = ((w.weight * w.realCount) + _weight) / (1 + w.realCount);
      w.score = (w.weight * (w.realCount * Weight::REAL + w.stemCount * Weight::STEM));
      db.updateWord(w);
    }

    db.updateAllStemNumber(w, term.stemTerm,  db.getStemNumber(w, term.stemTerm) + 1);
  }

  /*!
  ** Add the term if it doesn't exists yet.
  ** Try to find the stem of this word.
  **
  ** @param term The term to add
  **
  ** @return The id of the committed term
  */
  Column::Term
  Indexer::commitTerm(const std::string& term) const
  {
    Index::Database& db = Index::Database::getInstance();
    Column::Term t = db.getTermByName(term);
    if (!Column::termExists(t))
    {
      t.realTerm = term;
      t.stemTerm = _stem->getStem(term);
      db.addOrUpdateTerm(t);
      t = db.getTermByName(term);
    }

    return t;
  }
}
