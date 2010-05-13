#include "SQLiteStatement.hh"
#include "SQLiteException.hh"

namespace SQLite
{
  Statement::Statement()
  {
    _mpDB = 0;
    _mpVM = 0;
  }

  Statement::Statement(const Statement& rStatement)
  {
    _mpDB = rStatement._mpDB;
    _mpVM = rStatement._mpVM;
    // Only one object can own VM
    const_cast<Statement&>(rStatement)._mpVM = 0;
  }

  Statement::Statement(sqlite3* pDB, sqlite3_stmt* pVM)
  {
    _mpDB = pDB;
    _mpVM = pVM;
  }

  Statement::~Statement()
  {
    try
    {
      finalize();
    }
    catch (...)
    {
    }
  }

  Statement&
  Statement::operator=(const Statement& rStatement)
  {
    _mpDB = rStatement._mpDB;
    _mpVM = rStatement._mpVM;
    // Only one object can own VM
    const_cast<Statement&>(rStatement)._mpVM = 0;
    return *this;
  }

  int
  Statement::execDML()
  {
    checkDB();
    checkVM();

    const char* szError=0;

    int nRet = sqlite3_step(_mpVM);

    if (nRet == SQLITE_DONE)
    {
      int nRowsChanged = sqlite3_changes(_mpDB);

      nRet = sqlite3_reset(_mpVM);

      if (nRet != SQLITE_OK)
      {
	szError = sqlite3_errmsg(_mpDB);
	throw Exception(nRet, (char*)szError, DONT_DELETE_MSG);
      }

      return nRowsChanged;
    }
    else
    {
      nRet = sqlite3_reset(_mpVM);
      szError = sqlite3_errmsg(_mpDB);
      throw Exception(nRet, (char*)szError, DONT_DELETE_MSG);
    }
  }

  Query
  Statement::execQuery()
  {
    checkDB();
    checkVM();

    int nRet = sqlite3_step(_mpVM);

    if (nRet == SQLITE_DONE)
    {
      // no rows
      return Query(_mpDB, _mpVM, true/*eof*/, false);
    }
    else if (nRet == SQLITE_ROW)
    {
      // at least 1 row
      return Query(_mpDB, _mpVM, false/*eof*/, false);
    }
    else
    {
      nRet = sqlite3_reset(_mpVM);
      const char* szError = sqlite3_errmsg(_mpDB);
      throw Exception(nRet, (char*)szError, DONT_DELETE_MSG);
    }
  }

  void
  Statement::bind(int nParam, const char* szValue)
  {
    checkVM();
    int nRes = sqlite3_bind_text(_mpVM, nParam, szValue, -1, SQLITE_TRANSIENT);

    if (nRes != SQLITE_OK)
    {
      throw Exception(nRes,
		      (char*) "Error binding string param",
		      DONT_DELETE_MSG);
    }
  }

  void
  Statement::bind(int nParam, const int nValue)
  {
    checkVM();
    int nRes = sqlite3_bind_int(_mpVM, nParam, nValue);

    if (nRes != SQLITE_OK)
    {
      throw Exception(nRes,
		      (char*) "Error binding int param",
		      DONT_DELETE_MSG);
    }
  }

  void
  Statement::bind(int nParam, const double dValue)
  {
    checkVM();
    int nRes = sqlite3_bind_double(_mpVM, nParam, dValue);

    if (nRes != SQLITE_OK)
    {
      throw Exception(nRes,
		      (char*) "Error binding double param",
		      DONT_DELETE_MSG);
    }
  }

  void
  Statement::bind(int nParam, const unsigned char* blobValue, int nLen)
  {
    checkVM();
    int nRes = sqlite3_bind_blob(_mpVM, nParam,
				 (const void*)blobValue, nLen, SQLITE_TRANSIENT);

    if (nRes != SQLITE_OK)
    {
      throw Exception(nRes,
		      (char*) "Error binding blob param",
		      DONT_DELETE_MSG);
    }
  }

  void
  Statement::bindNull(int nParam)
  {
    checkVM();
    int nRes = sqlite3_bind_null(_mpVM, nParam);

    if (nRes != SQLITE_OK)
    {
      throw Exception(nRes,
		      (char*) "Error binding NULL param",
		      DONT_DELETE_MSG);
    }
  }

  void
  Statement::reset()
  {
    if (_mpVM)
    {
      int nRet = sqlite3_reset(_mpVM);

      if (nRet != SQLITE_OK)
      {
	const char* szError = sqlite3_errmsg(_mpDB);
	throw Exception(nRet, (char*)szError, DONT_DELETE_MSG);
      }
    }
  }

  void
  Statement::finalize()
  {
    if (_mpVM)
    {
      int nRet = sqlite3_finalize(_mpVM);
      _mpVM = 0;

      if (nRet != SQLITE_OK)
      {
	const char* szError = sqlite3_errmsg(_mpDB);
	throw Exception(nRet, (char*)szError, DONT_DELETE_MSG);
      }
    }
  }

  void
  Statement::checkDB()
  {
    if (_mpDB == 0)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Database not open",
		      DONT_DELETE_MSG);
    }
  }

  void
  Statement::checkVM()
  {
    if (_mpVM == 0)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Null Virtual Machine pointer",
		      DONT_DELETE_MSG);
    }
  }
}
