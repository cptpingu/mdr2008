#ifndef SQLITEEXCEPTION_HH_
# define SQLITEEXCEPTION_HH_

# include "SQLite.hh"
# include <iostream>

namespace SQLite
{
  static const int CPPSQLITE_ERROR = 1000;

  class Exception
  {
  public:

    Exception(const int nErrCode,
	      char* szErrMess,
	      bool bDeleteMsg = true);
    Exception(const Exception& e);
    virtual ~Exception();

    static const char* errorCodeAsString(int nErrCode);
    int errorCode()
    {
      return _mnErrCode;
    }
    const char* errorMessage()
    {
      return _mpszErrMess;
    }

  private:
    int		_mnErrCode;
    char*	_mpszErrMess;
  };
}

#endif /* !SQLITEEXCEPTION_HH_ */
