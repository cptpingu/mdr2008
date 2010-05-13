#include "SQLiteDB.hh"
#include "SQLiteException.hh"

namespace SQLite
{
  DB::DB()
  {
    _mpDB = 0;
    _mnBusyTimeoutMs = 60000; // 60 seconds
  }

  DB::DB(const DB& db)
  {
    _mpDB = db._mpDB;
    _mnBusyTimeoutMs = 60000; // 60 seconds
  }

  DB::~DB()
  {
    close();
  }

  DB& DB::operator=(const DB& db)
  {
    _mpDB = db._mpDB;
    _mnBusyTimeoutMs = 60000; // 60 seconds
    return *this;
  }

  void
  DB::open(const char* szFile)
  {
    int nRet = sqlite3_open(szFile, &_mpDB);

    if (nRet != SQLITE_OK)
    {
      const char* szError = sqlite3_errmsg(_mpDB);
      throw Exception(nRet, (char*)szError, DONT_DELETE_MSG);
    }

    setBusyTimeout(_mnBusyTimeoutMs);
  }

  void
  DB::close()
  {
    if (_mpDB)
    {
      sqlite3_close(_mpDB);
      _mpDB = 0;
    }
  }

  Statement
  DB::compileStatement(const char* szSQL)
  {
    checkDB();

    sqlite3_stmt* pVM = compile(szSQL);
    return Statement(_mpDB, pVM);
  }

  bool
  DB::tableExists(const char* szTable)
  {
    char szSQL[128];
    sprintf(szSQL,
	    "select count(*) from sqlite_master where type='table' and name='%s'",
	    szTable);
    int nRet = execScalar(szSQL);
    return (nRet > 0);
  }

  int
  DB::execDML(const char* szSQL)
  {
    checkDB();

    char* szError=0;
    int nRet = sqlite3_exec(_mpDB, szSQL, 0, 0, &szError);
    if (nRet == SQLITE_OK)
      return sqlite3_changes(_mpDB);

    throw Exception(nRet, szError);
  }

  Query
  DB::execQuery(const char* szSQL)
  {
    checkDB();

    sqlite3_stmt* pVM = compile(szSQL);
    int nRet = sqlite3_step(pVM);

    // no rows
    if (nRet == SQLITE_DONE)
      return Query(_mpDB, pVM, true/*eof*/);

    // at least 1 row
    if (nRet == SQLITE_ROW)
      return Query(_mpDB, pVM, false/*eof*/);

    nRet = sqlite3_finalize(pVM);
    const char* szError= sqlite3_errmsg(_mpDB);
    throw Exception(nRet, (char*)szError, DONT_DELETE_MSG);
  }

  int
  DB::execScalar(const char* szSQL)
  {
    Query q = execQuery(szSQL);

    if (q.eof() || q.numFields() < 1)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Invalid scalar query",
		      DONT_DELETE_MSG);
    }

    if (q.fieldValue(0) == 0 || std::string(q.fieldValue(0)) == "")
      return 0;

    return atoi(q.fieldValue(0));
  }

  Table
  DB::getTable(const char* szSQL)
  {
    checkDB();

    char* szError=0;
    char** paszResults=0;
    int nRet;
    int nRows(0);
    int nCols(0);

    nRet = sqlite3_get_table(_mpDB, szSQL, &paszResults, &nRows, &nCols, &szError);
    if (nRet == SQLITE_OK)
      return Table(paszResults, nRows, nCols);

    throw Exception(nRet, szError);
  }

  sqlite_int64
  DB::lastRowId()
  {
    return sqlite3_last_insert_rowid(_mpDB);
  }

  void
  DB::setBusyTimeout(int nMillisecs)
  {
    _mnBusyTimeoutMs = nMillisecs;
    sqlite3_busy_timeout(_mpDB, _mnBusyTimeoutMs);
  }

  void
  DB::checkDB()
  {
    if (!_mpDB)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Database not open",
		      DONT_DELETE_MSG);
    }
  }

  sqlite3_stmt*
  DB::compile(const char* szSQL)
  {
    checkDB();

    char* szError=0;
    const char* szTail=0;
    sqlite3_stmt* pVM;

    int nRet = sqlite3_prepare(_mpDB, szSQL, -1, &pVM, &szTail);
    if (nRet != SQLITE_OK)
      throw Exception(nRet, szError);

    return pVM;
  }
}
