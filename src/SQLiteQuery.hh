#ifndef SQLITEQUERY_HH_
# define SQLITEQUERY_HH_

# include "SQLite.hh"

namespace SQLite
{
  class Query
  {
  public:

    Query();
    Query(const Query& rQuery);
    Query(sqlite3* pDB,
	  sqlite3_stmt* pVM,
	  bool bEof,
	  bool bOwnVM=true);
    virtual ~Query();

  public:
    Query& operator=(const Query& rQuery);
    int numFields();
    int fieldIndex(const char* szField);
    const char* fieldName(int nCol);
    const char* fieldDeclType(int nCol);
    int fieldDataType(int nCol);
    const char* fieldValue(int nField);
    const char* fieldValue(const char* szField);
    int getIntField(int nField, int nNullValue=0);
    int getIntField(const char* szField, int nNullValue=0);
    double getFloatField(int nField, double fNullValue=0.0);
    double getFloatField(const char* szField, double fNullValue=0.0);
    const char* getStringField(int nField, const char* szNullValue="");
    const char* getStringField(const char* szField, const char* szNullValue="");
    const unsigned char* getBlobField(int nField, int& nLen);
    const unsigned char* getBlobField(const char* szField, int& nLen);
    bool fieldIsNull(int nField);
    bool fieldIsNull(const char* szField);
    bool eof();
    void nextRow();
    void finalize();

  private:
    void checkVM();

  private:
    sqlite3*		_mpDB;
    sqlite3_stmt*	_mpVM;
    bool		_mbEof;
    int			_mnCols;
    bool		_mbOwnVM;
  };
}

#endif /* !SQLITEQUERY_HH_ */
