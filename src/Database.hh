#ifndef DATABASE_HH_
# define DATABASE_HH_

# include <iostream>
# include <cassert>
# include <list>
# include "Utils.hh"
# include "Column.hh"
# include "Singleton.hh"
# include "SQLiteDB.hh"

namespace Index
{
  class Database : public Singleton<Database>
  {
    friend class Singleton<Database>;

  private:
    Database();
    ~Database();

  public:
    void open(const std::string& filename);
    void close();
    void createDatabase();
    void dumpAll();
    void dump(const std::string& tableName);
    void beginTransaction();
    void endTransaction();
    void clearSearchCache();

    /*!
    ** DAO
    */
  public:
    const Column::Document getDocumentByFilename(const std::string& filename);
    const Column::Document getDocumentById(const unsigned int idDoc);
    const Column::Word getWordByIds(const unsigned int idDocument, const unsigned int idTerm);
    const Column::Term getTermById(const unsigned int idTerm);
    const Column::Term getTermByName(const std::string& termName);
    void addOrUpdateDocument(const Column::Document& doc);
    void addWord(const Column::Word& word);
    void updateWord(const Column::Word& word);
    void addOrUpdateTerm(const Column::Term& term);
    int getStemNumber(const Column::Word& word, const std::string& stem);
    void updateTermScore(const Column::Document& doc);
    void updateAllStemNumber(const Column::Word& word,
			     const std::string& stem,
			     const unsigned int stemCount);
    void deleteDocument(const Column::Document& doc, const bool erase);
    const std::list<Column::DocumentResult> getCompleteDocuments(const std::string& condition);
    unsigned int getSimilarRequest(const std::string& query);
    const std::list<Column::DocumentResult> getCachedSearchResult(const unsigned int id);
    void saveResult(const Column::Result& res, const double rank);
    void saveResults(const std::list<Column::DocumentResult>& list,
		     const std::string& sentence);

    /*!
    ** Templated DAO
    */
  public:
    template <typename Collection, typename Type>
    void fillList(Collection& list, const std::string& table);

    /*!
    ** Private inlined DAO Helpers
    */
  private:
    const Column::Document getDocument(const std::string& request);
    const Column::Word getWord(const std::string& request);
    const Column::Term getTerm(const std::string& request);
    const std::list<Column::DocumentWordTerm> getDocumentWords(const std::string& request);
    const std::list<Column::DocumentResult> getDocumentResults(const std::string& request);

  private:
    SQLite::DB	_db;
  };
}

# include "Database.hxx"

#endif /* !DATABASE_HH_ */
