#ifndef SQLITEBUFFER_HH_
# define SQLITEBUFFER_HH_

# include "SQLite.hh"

namespace SQLite
{
  class Buffer
  {
  public:

    Buffer();
    ~Buffer();

    const char* format(const char* szFormat, ...);
    void clear();
    operator const char*()
    {
      return _mpBuf;
    }

  private:

    char*	_mpBuf;
  };
}

#endif /* !SQLITEBUFFER_HH_ */
