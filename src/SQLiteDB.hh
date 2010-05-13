#ifndef SQLITEDB_HH_
# define SQLITEDB_HH_

# include "SQLite.hh"
# include "SQLiteTable.hh"
# include "SQLiteQuery.hh"
# include "SQLiteStatement.hh"

namespace SQLite
{
  class DB
  {
  public:
    DB();
    virtual ~DB();

  public:
    void open(const char* szFile);
    void close();
    bool tableExists(const char* szTable);
    int execDML(const char* szSQL);
    Query execQuery(const char* szSQL);
    int execScalar(const char* szSQL);
    Table getTable(const char* szSQL);
    Statement compileStatement(const char* szSQL);
    sqlite_int64 lastRowId();
    void setBusyTimeout(int nMillisecs);

    void interrupt()
    {
      sqlite3_interrupt(_mpDB);
    }
    static const char* SQLiteVersion()
    {
      return SQLITE_VERSION;
    }

  private:
    DB(const DB& db);
    DB& operator=(const DB& db);
    sqlite3_stmt* compile(const char* szSQL);
    void checkDB();

  private:
    sqlite3*	_mpDB;
    int		_mnBusyTimeoutMs;
  };
}
#endif /* !SQLITEDB_HH_ */
