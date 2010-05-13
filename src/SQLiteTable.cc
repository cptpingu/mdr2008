#include "SQLiteTable.hh"
#include "SQLiteException.hh"

namespace SQLite
{
  Table::Table()
  {
    _mpaszResults = 0;
    _mnRows = 0;
    _mnCols = 0;
    _mnCurrentRow = 0;
  }

  Table::Table(const Table& rTable)
  {
    _mpaszResults = rTable._mpaszResults;
    // Only one object can own the results
    const_cast<Table&>(rTable)._mpaszResults = 0;
    _mnRows = rTable._mnRows;
    _mnCols = rTable._mnCols;
    _mnCurrentRow = rTable._mnCurrentRow;
  }

  Table::Table(char** paszResults, int nRows, int nCols)
  {
    _mpaszResults = paszResults;
    _mnRows = nRows;
    _mnCols = nCols;
    _mnCurrentRow = 0;
  }

  Table::~Table()
  {
    try
    {
      finalize();
    }
    catch (...)
    {
    }
  }

  Table&
  Table::operator=(const Table& rTable)
  {
    try
    {
      finalize();
    }
    catch (...)
    {
    }
    _mpaszResults = rTable._mpaszResults;
    // Only one object can own the results
    const_cast<Table&>(rTable)._mpaszResults = 0;
    _mnRows = rTable._mnRows;
    _mnCols = rTable._mnCols;
    _mnCurrentRow = rTable._mnCurrentRow;
    return *this;
  }

  void
  Table::finalize()
  {
    if (_mpaszResults)
    {
      sqlite3_free_table(_mpaszResults);
      _mpaszResults = 0;
    }
  }

  int
  Table::numFields()
  {
    checkResults();
    return _mnCols;
  }

  int
  Table::numRows()
  {
    checkResults();
    return _mnRows;
  }

  const char*
  Table::fieldValue(int nField)
  {
    checkResults();

    if (nField < 0 || nField > _mnCols-1)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Invalid field index requested",
		      DONT_DELETE_MSG);
    }

    int nIndex = (_mnCurrentRow*_mnCols) + _mnCols + nField;
    return _mpaszResults[nIndex];
  }

  const char*
  Table::fieldValue(const char* szField)
  {
    checkResults();

    if (szField)
    {
      for (int nField = 0; nField < _mnCols; nField++)
      {
	if (strcmp(szField, _mpaszResults[nField]) == 0)
	{
	  int nIndex = (_mnCurrentRow*_mnCols) + _mnCols + nField;
	  return _mpaszResults[nIndex];
	}
      }
    }

    throw Exception(CPPSQLITE_ERROR,
		    (char*) "Invalid field name requested",
		    DONT_DELETE_MSG);
  }

  int
  Table::getIntField(int nField, int nNullValue/*=0*/)
  {
    if (fieldIsNull(nField))
    {
      return nNullValue;
    }
    else
    {
      return atoi(fieldValue(nField));
    }
  }

  int
  Table::getIntField(const char* szField, int nNullValue/*=0*/)
  {
    if (fieldIsNull(szField))
    {
      return nNullValue;
    }
    else
    {
      return atoi(fieldValue(szField));
    }
  }

  double
  Table::getFloatField(int nField, double fNullValue/*=0.0*/)
  {
    if (fieldIsNull(nField))
    {
      return fNullValue;
    }
    else
    {
      return atof(fieldValue(nField));
    }
  }

  double
  Table::getFloatField(const char* szField, double fNullValue/*=0.0*/)
  {
    if (fieldIsNull(szField))
    {
      return fNullValue;
    }
    else
    {
      return atof(fieldValue(szField));
    }
  }

  const char*
  Table::getStringField(int nField, const char* szNullValue/*=""*/)
  {
    if (fieldIsNull(nField))
    {
      return szNullValue;
    }
    else
    {
      return fieldValue(nField);
    }
  }

  const char*
  Table::getStringField(const char* szField, const char* szNullValue/*=""*/)
  {
    if (fieldIsNull(szField))
    {
      return szNullValue;
    }
    else
    {
      return fieldValue(szField);
    }
  }

  bool
  Table::fieldIsNull(int nField)
  {
    checkResults();
    return (fieldValue(nField) == 0);
  }

  bool
  Table::fieldIsNull(const char* szField)
  {
    checkResults();
    return (fieldValue(szField) == 0);
  }

  const char*
  Table::fieldName(int nCol)
  {
    checkResults();

    if (nCol < 0 || nCol > _mnCols-1)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Invalid field index requested",
		      DONT_DELETE_MSG);
    }

    return _mpaszResults[nCol];
  }

  void
  Table::setRow(int nRow)
  {
    checkResults();

    if (nRow < 0 || nRow > _mnRows-1)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Invalid row index requested",
		      DONT_DELETE_MSG);
    }

    _mnCurrentRow = nRow;
  }

  void
  Table::checkResults()
  {
    if (_mpaszResults == 0)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Null Results pointer",
		      DONT_DELETE_MSG);
    }
  }
}
