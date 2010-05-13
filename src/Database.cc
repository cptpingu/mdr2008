#include <cassert>
#include "Utils.hh"
#include "Database.hh"
#include "SQLiteQuery.hh"
#include "SQLiteException.hh"

namespace Index
{
  /*!
  ** Create an index database object.
  */
  Database::Database()
  {
  }

  /*!
  ** Destruct an index database.
  */
  Database::~Database()
  {
  }

  /*!
  ** Create database if it doesn't exists yet.
  **
  ** @param filename SQLite3 database
  */
  void
  Database::open(const std::string& filename)
  {
    bool exists = Utils::fileExists(filename);
    _db.open(filename.c_str());
    if (!exists)
      createDatabase();
  }

  /*!
  ** Close the database.
  */
  void
  Database::close()
  {
    _db.close();
  }

  /*!
  ** Create the structure of the new database.
  */
  void
  Database::createDatabase()
  {
    static const std::string cmd =
      std::string("CREATE TABLE Document(id_doc INTEGER PRIMARY KEY AUTOINCREMENT, filename TEXT, type INTEGER, hash TEXT, date TEXT, length INTEGER);") +
      std::string("CREATE TABLE Word(id_doc INTEGER, id_term INTEGER, weight REAL, real_count INTEGER, stem_count INTEGER, score REAL);") +
      std::string("CREATE TABLE Term(id_term INTEGER PRIMARY KEY AUTOINCREMENT, real_term TEXT, stem_term TEXT);") +
      std::string("CREATE TABLE Search(id_search INTEGER PRIMARY KEY AUTOINCREMENT, sentence TEXT);") +
      std::string("CREATE TABLE Result(id_search INTEGER, id_doc INTEGER, rank REAL);") +
      std::string("CREATE TABLE WhiteList(expression TEXT);") +
      std::string("INSERT INTO WhiteList VALUES('.*\\.txt$');") +
      std::string("INSERT INTO WhiteList VALUES('.*\\.htm$');") +
      std::string("INSERT INTO WhiteList VALUES('.*\\.html$');") +
      std::string("CREATE TABLE BlackList(expression TEXT);") +
      std::string("INSERT INTO BlackList VALUES('.*\\.php$');");

    _db.execDML(cmd.c_str());
  }

  /*!
  ** Dump a table on standard output.
  **
  ** @param tableName The name of the table
  */
  void
  Database::dump(const std::string& tableName)
  {
    const std::string cmd = std::string("SELECT * FROM ") + tableName + std::string(";");
    SQLite::Query q = _db.execQuery(cmd.c_str());

    std::cout << "== Dump of " << tableName << " ==" << std::endl;
    for (int fld = 0; fld < q.numFields(); fld++)
      std::cout << q.fieldName(fld) << "(" << q.fieldDeclType(fld) << ")|";
    std::cout << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    while (!q.eof())
    {
      for (int fld = 0; fld < q.numFields(); fld++)
	std::cout << q.fieldValue(fld) << "|";
      std::cout << std::endl;
      q.nextRow();
    }
  }

  /*!
  ** Dump all table contained within the sqlite database.
  */
  void
  Database::dumpAll()
  {
    SQLite::Query q = _db.execQuery("SELECT * FROM sqlite_master;");
    while (!q.eof())
    {
      std::cout << std::endl;
      if (q.fieldValue(1) != std::string("sqlite_sequence"))
	dump(q.fieldValue(1));
      q.nextRow();
    }
  }

  /*!
  ** Get a document by filename.
  ** The document must be unique!
  **
  ** @param filename The filename
  **
  ** @return A document filled with all information, else an id doc equal to 0
  */
  const Column::Document
  Database::getDocumentByFilename(const std::string& filename)
  {
    assert(filename != "");
    std::ostringstream cmd;
    cmd << "SELECT * FROM Document WHERE filename = '" << filename << "';";
    return getDocument(cmd.str());
  }

  /*!
  ** Get a document by id.
  ** The document must be unique!
  **
  ** @param idDoc The document id
  **
  ** @return A document filled with all information, else an id doc equal to 0
  */
  const Column::Document
  Database::getDocumentById(const unsigned int idDoc)
  {
    assert(idDoc != 0);
    std::ostringstream cmd;
    cmd << "SELECT * FROM Document WHERE id_doc = '" << idDoc << "';";
    return getDocument(cmd.str());
  }

  /*!
  ** Get a word using an idDocument and an idTerm.
  ** The word must be unique!
  **
  ** @param  idDocument The document id
  ** @param  idTerm The term id
  **
  ** @return A word filled with all information, else an id_doc and an idTerm equal to 0
  */
  const Column::Word
  Database::getWordByIds(const unsigned int idDocument, const unsigned int idTerm)
  {
    assert(idDocument != 0);
    assert(idTerm != 0);
    std::ostringstream cmd;
    cmd << "SELECT * FROM Word WHERE id_doc = " << idDocument <<
      " AND id_term = " << idTerm << ";";
    return getWord(cmd.str());
  }

  /*!
  ** Get a term by id.
  ** The term must be unique!
  **
  ** @param idTerm The term id
  **
  ** @return A term filled with all information, else an id term equal to 0
  */
  const Column::Term
  Database::getTermById(const unsigned int idTerm)
  {
    assert(idTerm != 0);
    std::ostringstream cmd;
    cmd << "SELECT * FROM Term WHERE id_term = '" << idTerm << "';";
    return getTerm(cmd.str());
  }

  /*!
  ** Get a term by name.
  ** The term must be unique!
  **
  ** @param termName The term name
  **
  ** @return A term filled with all information, else an id term equal to 0
  */
  const Column::Term
  Database::getTermByName(const std::string& termName)
  {
    assert(termName != "");
    std::ostringstream cmd;
    cmd << "SELECT * FROM Term WHERE real_term = '" << termName << "';";
    return getTerm(cmd.str());
  }

  /*!
  ** Add or update a document. Document is added if doc.id == 0,
  ** else an update operation is performed.
  **
  ** @param doc The document to add or update
  */
  void
  Database::addOrUpdateDocument(const Column::Document& doc)
  {
    std::ostringstream cmd;

    // Check if already exists, then add or update
    if (!Column::docExists(doc))
      cmd << "INSERT INTO Document(filename, type, hash, date, length) VALUES(" <<
	'\'' << doc.filename << '\''<< "," <<
	doc.type << "," <<
	'\'' << doc.hash << '\'' << "," <<
	'\'' << doc.date << '\'' << "," <<
	doc.length << ");";
    else
      cmd << "UPDATE Document SET " <<
	"filename = " << '\'' << doc.filename << '\''<< "," <<
	"type = " << doc.type << "," <<
	"hash = " << '\'' << doc.hash << '\'' << "," <<
	"date = " << '\'' << doc.date << '\'' << "," <<
	"length = " << doc.length <<
	" WHERE id_doc = " << doc.id << ";";

    _db.execDML(cmd.str().c_str());
  }

  /*!
  ** Add a word.
  **
  ** @param word The word to add
  */
  void
  Database::addWord(const Column::Word& word)
  {
    std::ostringstream cmd;

    cmd << "INSERT INTO Word(id_doc, id_term, weight, real_count, stem_count, score) VALUES(" <<
      word.idDocument << "," <<
      word.idTerm << "," <<
      word.weight << "," <<
      word.realCount << "," <<
      word.stemCount << "," <<
      word.score << ");";

    _db.execDML(cmd.str().c_str());
  }

  /*!
  ** Update a word.
  **
  ** @param word The word to update
  */
  void
  Database::updateWord(const Column::Word& word)
  {
    std::ostringstream cmd;

    cmd << "UPDATE Word SET " <<
      "weight = " << word.weight << "," <<
      "real_count = " << word.realCount << "," <<
      "stem_count = " << word.stemCount << "," <<
      "score = " << word.score <<
      " WHERE id_doc = " << word.idDocument <<
      " AND id_term = " << word.idTerm <<";";

    _db.execDML(cmd.str().c_str());
  }

  /*!
  ** Add or update a term. Term is added if term.id == 0,
  ** else an update operation is performed.
  **
  ** @param term The term to add or update
  */
  void
  Database::addOrUpdateTerm(const Column::Term& term)
  {
    std::ostringstream cmd;

    // Check if already exists, then add or update
    if (!Column::termExists(term))
      cmd << "INSERT INTO Term(real_term, stem_term) VALUES(" <<
	'\'' << term.realTerm << '\''<< "," <<
	'\'' << term.stemTerm << '\'' << ");";
    else
      cmd << "UPDATE Term SET " <<
	"real_term = " << '\'' << term.realTerm << '\''<< "," <<
	"stem_term = " << '\'' << term.stemTerm << '\'' <<
	" WHERE id_term = " << term.id << ";";

    _db.execDML(cmd.str().c_str());
  }

  /*!
  ** Get the number of stem the given word contains.
  **
  ** @param word The word
  **
  ** @return Number of stem
  */
  int
  Database::getStemNumber(const Column::Word& word, const std::string& stem)
  {
    std::ostringstream cmd;
    cmd << "SELECT SUM(real_count) FROM Word " <<
      " JOIN Term ON Word.id_term = Term.id_term AND id_doc = " <<
      word.idDocument << " AND stem_term = '" << stem << "';";

    return _db.execScalar(cmd.str().c_str());
  }

  /*!
  ** Update correctly all stem number found.
  **
  ** @param word The word to update
  ** @param stem The stem form of a term
  ** @param stemCount The stem number found
  */
  void
  Database::updateAllStemNumber(const Column::Word& word,
				const std::string& stem,
				const unsigned int stemCount)
  {
    std::ostringstream request;
    request << "SELECT Word.id_term FROM Word JOIN Term ON Word.id_term = Term.id_term " <<
      " WHERE id_doc = " << word.idDocument << " AND stem_term = '" <<
      stem << "'";
    SQLite::Query q = _db.execQuery(request.str().c_str());
    unsigned int id = 0;
    while (!q.eof())
    {
      for (int fld = 0; fld < q.numFields(); fld++)
      {
	if (std::string(q.fieldName(fld)) == "id_term")
	{
	  id = Utils::stringToInt(q.fieldValue(fld));
	  std::ostringstream cmd;
	  cmd << "UPDATE Word SET " <<
	    " stem_count = " << stemCount <<
	    " WHERE id_term = " << id << ";";
	  _db.execDML(cmd.str().c_str());
	}
      }
      q.nextRow();
    }
    assert(q.eof());
  }

  /*!
  ** Delete a document, and all it's associated words.
  **
  ** @param doc The document to delete.
  ** @param erase If we erase the document or just trunc it.
  */
  void
  Database::deleteDocument(const Column::Document& doc, const bool erase)
  {
    if (erase)
    {
      std::ostringstream cmd;
      cmd << "DELETE FROM Document WHERE " <<
	" id_doc = " << doc.id << ";";
      _db.execDML(cmd.str().c_str());
    }
    std::ostringstream cmd;
    cmd << "DELETE FROM Word WHERE " <<
      " id_doc = " << doc.id << ";";
    _db.execDML(cmd.str().c_str());
  }

  /*!
  ** Get a complete document list.
  **
  ** @param condition The retrieval condition
  **
  ** @return A list of document
  */
  const std::list<Column::DocumentResult>
  Database::getCompleteDocuments(const std::string& condition)
  {
    assert(condition != "");
    std::ostringstream cmd;
    cmd << "SELECT "
      "Document.id_doc, hash, filename, date, score, real_term, stem_term "
      " FROM Document JOIN Word ON Document.id_doc = Word.id_doc"
	" JOIN Term On Term.id_term = Word.id_term " <<
      " WHERE " << condition << " ORDER BY score;";
    const std::list<Column::DocumentWordTerm> lst = getDocumentWords(cmd.str());
    std::list<Column::DocumentResult> resList;
    Column::DocumentResult res;
    for (std::list<Column::DocumentWordTerm>::const_iterator i = lst.begin();
	 i != lst.end(); ++i)
    {
      res.id = i->Column::Document::id;
      res.filename = i->filename;
      res.type = i->type;
      res.hash = i->hash;
      res.date = i->date;
      res.type = i->type;
      res.length = i->length;
      res.idDoc = i->Column::Document::id;
      res.rank = i->score;
      resList.push_back(res);
    }

    return resList;
  }

}
