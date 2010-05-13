#ifndef SQLITETABLE_HH_
# define SQLITETABLE_HH_

# include "SQLite.hh"

namespace SQLite
{
  class Table
  {
  public:
    Table();
    Table(const Table& rTable);
    Table(char** paszResults, int nRows, int nCols);
    virtual ~Table();

  public:
    Table& operator=(const Table& rTable);
    int numFields();
    int numRows();
    const char* fieldName(int nCol);
    const char* fieldValue(int nField);
    const char* fieldValue(const char* szField);
    int getIntField(int nField, int nNullValue=0);
    int getIntField(const char* szField, int nNullValue=0);
    double getFloatField(int nField, double fNullValue=0.0);
    double getFloatField(const char* szField, double fNullValue=0.0);
    const char* getStringField(int nField, const char* szNullValue="");
    const char* getStringField(const char* szField, const char* szNullValue="");
    bool fieldIsNull(int nField);
    bool fieldIsNull(const char* szField);
    void setRow(int nRow);
    void finalize();

  private:
    void checkResults();

  private:
    int		_mnCols;
    int		_mnRows;
    int		_mnCurrentRow;
    char**	_mpaszResults;
  };
}
#endif /* !SQLITETABLE_HH_ */
