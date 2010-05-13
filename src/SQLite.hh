#ifndef SQLITE_HH_
# define SQLITE_HH_

# include <sqlite3.h>
# include <cstdio>
# include <cstring>
# include <cstdlib>

namespace SQLite
{
  // Named constant for passing to CppSQLite3Exception when passing it a string
  // that cannot be deleted.
  static const bool DONT_DELETE_MSG = false;

  int sqlite3_encode_binary(const unsigned char* in, int n, unsigned char* out);
  int sqlite3_decode_binary(const unsigned char* in, unsigned char* out);
}

#endif /* !SQLITE_HH_ */
