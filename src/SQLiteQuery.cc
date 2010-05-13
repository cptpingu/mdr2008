#include "SQLiteQuery.hh"
#include "SQLiteException.hh"

namespace SQLite
{
  Query::Query()
  {
    _mpVM = 0;
    _mbEof = true;
    _mnCols = 0;
    _mbOwnVM = false;
  }

  Query::Query(const Query& rQuery)
  {
    _mpVM = rQuery._mpVM;
    // Only one object can own the VM
    const_cast<Query&>(rQuery)._mpVM = 0;
    _mbEof = rQuery._mbEof;
    _mnCols = rQuery._mnCols;
    _mbOwnVM = rQuery._mbOwnVM;
  }

  Query::Query(sqlite3* pDB,
	       sqlite3_stmt* pVM,
	       bool bEof,
	       bool bOwnVM/*=true*/)
  {
    _mpDB = pDB;
    _mpVM = pVM;
    _mbEof = bEof;
    _mnCols = sqlite3_column_count(_mpVM);
    _mbOwnVM = bOwnVM;
  }

  Query::~Query()
  {
    try
    {
      finalize();
    }
    catch (...)
    {
    }
  }

  Query&
  Query::operator=(const Query& rQuery)
  {
    try
    {
      finalize();
    }
    catch (...)
    {
    }
    _mpVM = rQuery._mpVM;
    // Only one object can own the VM
    const_cast<Query&>(rQuery)._mpVM = 0;
    _mbEof = rQuery._mbEof;
    _mnCols = rQuery._mnCols;
    _mbOwnVM = rQuery._mbOwnVM;
    return *this;
  }

  int
  Query::numFields()
  {
    checkVM();
    return _mnCols;
  }

  const char*
  Query::fieldValue(int nField)
  {
    checkVM();

    if (nField < 0 || nField > _mnCols-1)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Invalid field index requested",
		      DONT_DELETE_MSG);
    }

    return (const char*)sqlite3_column_text(_mpVM, nField);
  }

  const char*
  Query::fieldValue(const char* szField)
  {
    int nField = fieldIndex(szField);
    return (const char*)sqlite3_column_text(_mpVM, nField);
  }

  int
  Query::getIntField(int nField, int nNullValue/*=0*/)
  {
    if (fieldDataType(nField) == SQLITE_NULL)
    {
      return nNullValue;
    }
    else
    {
      return sqlite3_column_int(_mpVM, nField);
    }
  }

  int
  Query::getIntField(const char* szField, int nNullValue/*=0*/)
  {
    int nField = fieldIndex(szField);
    return getIntField(nField, nNullValue);
  }

  double
  Query::getFloatField(int nField, double fNullValue/*=0.0*/)
  {
    if (fieldDataType(nField) == SQLITE_NULL)
    {
      return fNullValue;
    }
    else
    {
      return sqlite3_column_double(_mpVM, nField);
    }
  }

  double
  Query::getFloatField(const char* szField, double fNullValue/*=0.0*/)
  {
    int nField = fieldIndex(szField);
    return getFloatField(nField, fNullValue);
  }

  const char*
  Query::getStringField(int nField, const char* szNullValue/*=""*/)
  {
    if (fieldDataType(nField) == SQLITE_NULL)
    {
      return szNullValue;
    }
    else
    {
      return (const char*)sqlite3_column_text(_mpVM, nField);
    }
  }

  const char*
  Query::getStringField(const char* szField, const char* szNullValue/*=""*/)
  {
    int nField = fieldIndex(szField);
    return getStringField(nField, szNullValue);
  }

  const unsigned char*
  Query::getBlobField(int nField, int& nLen)
  {
    checkVM();

    if (nField < 0 || nField > _mnCols-1)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Invalid field index requested",
		      DONT_DELETE_MSG);
    }

    nLen = sqlite3_column_bytes(_mpVM, nField);
    return (const unsigned char*)sqlite3_column_blob(_mpVM, nField);
  }

  const unsigned char*
  Query::getBlobField(const char* szField, int& nLen)
  {
    int nField = fieldIndex(szField);
    return getBlobField(nField, nLen);
  }

  bool
  Query::fieldIsNull(int nField)
  {
    return (fieldDataType(nField) == SQLITE_NULL);
  }

  bool
  Query::fieldIsNull(const char* szField)
  {
    int nField = fieldIndex(szField);
    return (fieldDataType(nField) == SQLITE_NULL);
  }

  int
  Query::fieldIndex(const char* szField)
  {
    checkVM();

    if (szField)
    {
      for (int nField = 0; nField < _mnCols; nField++)
      {
	const char* szTemp = sqlite3_column_name(_mpVM, nField);

	if (strcmp(szField, szTemp) == 0)
	{
	  return nField;
	}
      }
    }

    throw Exception(CPPSQLITE_ERROR,
		    (char*) "Invalid field name requested",
		    DONT_DELETE_MSG);
  }

  const char*
  Query::fieldName(int nCol)
  {
    checkVM();

    if (nCol < 0 || nCol > _mnCols-1)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Invalid field index requested",
		      DONT_DELETE_MSG);
    }

    return sqlite3_column_name(_mpVM, nCol);
  }

  const char*
  Query::fieldDeclType(int nCol)
  {
    checkVM();

    if (nCol < 0 || nCol > _mnCols-1)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Invalid field index requested",
		      DONT_DELETE_MSG);
    }

    return sqlite3_column_decltype(_mpVM, nCol);
  }

  int
  Query::fieldDataType(int nCol)
  {
    checkVM();

    if (nCol < 0 || nCol > _mnCols-1)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Invalid field index requested",
		      DONT_DELETE_MSG);
    }

    return sqlite3_column_type(_mpVM, nCol);
  }

  bool
  Query::eof()
  {
    checkVM();
    return _mbEof;
  }

  void
  Query::nextRow()
  {
    checkVM();

    int nRet = sqlite3_step(_mpVM);

    if (nRet == SQLITE_DONE)
    {
      // no rows
      _mbEof = true;
    }
    else if (nRet == SQLITE_ROW)
    {
      // more rows, nothing to do
    }
    else
    {
      nRet = sqlite3_finalize(_mpVM);
      _mpVM = 0;
      const char* szError = sqlite3_errmsg(_mpDB);
      throw Exception(nRet,
		      (char*)szError,
		      DONT_DELETE_MSG);
    }
  }

  void
  Query::finalize()
  {
    if (_mpVM && _mbOwnVM)
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
  Query::checkVM()
  {
    if (_mpVM == 0)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Null Virtual Machine pointer",
		      DONT_DELETE_MSG);
    }
  }
}
