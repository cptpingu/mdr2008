#ifndef SQLITESTATEMENT_HH_
# define SQLITESTATEMENT_HH_

# include "SQLite.hh"
# include "SQLiteQuery.hh"

namespace SQLite
{
  class Statement
  {
  public:
    Statement();
    Statement(const Statement& rStatement);
    Statement(sqlite3* pDB, sqlite3_stmt* pVM);
    virtual ~Statement();

  public:
    Statement& operator=(const Statement& rStatement);
    int execDML();
    Query execQuery();
    void bind(int nParam, const char* szValue);
    void bind(int nParam, const int nValue);
    void bind(int nParam, const double dwValue);
    void bind(int nParam, const unsigned char* blobValue, int nLen);
    void bindNull(int nParam);
    void reset();
    void finalize();

  private:
    void checkDB();
    void checkVM();

  private:
    sqlite3*		_mpDB;
    sqlite3_stmt*	_mpVM;
  };
}

#endif /* !SQLITESTATEMENT_HH_ */
