#include <cstdlib>
# include "SQLiteBuffer.hh"

namespace SQLite
{
  Buffer::Buffer()
  {
    _mpBuf = 0;
  }

  Buffer::~Buffer()
  {
    clear();
  }

  void
  Buffer::clear()
  {
    if (_mpBuf)
    {
      sqlite3_free(_mpBuf);
      _mpBuf = 0;
    }
  }

  const char*
  Buffer::format(const char* szFormat, ...)
  {
    clear();
    va_list va;
    va_start(va, szFormat);
    _mpBuf = sqlite3_vmprintf(szFormat, va);
    va_end(va);

    return _mpBuf;
  }
}
