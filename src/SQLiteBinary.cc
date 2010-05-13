#include "SQLiteBinary.hh"
#include "SQLiteException.hh"

namespace SQLite
{
  Binary::Binary()
    : _mpBuf(0), _mnBinaryLen(0), _mnBufferLen(0),
      _mnEncodedLen(0), _mbEncoded(false)
  {
  }

  Binary::~Binary()
  {
    clear();
  }

  void
  Binary::setBinary(const unsigned char* pBuf, int nLen)
  {
    _mpBuf = allocBuffer(nLen);
    memcpy(_mpBuf, pBuf, nLen);
  }

  void
  Binary::setEncoded(const unsigned char* pBuf)
  {
    clear();

    _mnEncodedLen = strlen((const char*)pBuf);
    _mnBufferLen = _mnEncodedLen + 1; // Allow for NULL terminator
    _mpBuf = (unsigned char*)malloc(_mnBufferLen);

    if (!_mpBuf)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Cannot allocate memory",
		      DONT_DELETE_MSG);
    }

    memcpy(_mpBuf, pBuf, _mnBufferLen);
    _mbEncoded = true;
  }

  const unsigned char*
  Binary::getEncoded()
  {
    if (!_mbEncoded)
    {
      unsigned char* ptmp = (unsigned char*)malloc(_mnBinaryLen);
      memcpy(ptmp, _mpBuf, _mnBinaryLen);
      _mnEncodedLen = sqlite3_encode_binary(ptmp, _mnBinaryLen, _mpBuf);
      free(ptmp);
      _mbEncoded = true;
    }

    return _mpBuf;
  }

  const unsigned char*
  Binary::getBinary()
  {
    if (_mbEncoded)
    {
      // in/out buffers can be the same
      _mnBinaryLen = sqlite3_decode_binary(_mpBuf, _mpBuf);

      if (_mnBinaryLen == -1)
      {
	throw Exception(CPPSQLITE_ERROR,
			(char*) "Cannot decode binary",
			DONT_DELETE_MSG);
      }

      _mbEncoded = false;
    }

    return _mpBuf;
  }

  int
  Binary::getBinaryLength()
  {
    getBinary();
    return _mnBinaryLen;
  }

  unsigned char*
  Binary::allocBuffer(int nLen)
  {
    clear();

    // Allow extra space for encoded binary as per comments in
    // SQLite encode.c See bottom of this file for implementation
    // of SQLite functions use 3 instead of 2 just to be sure ;-)
    _mnBinaryLen = nLen;
    _mnBufferLen = 3 + (257*nLen)/254;

    _mpBuf = (unsigned char*)malloc(_mnBufferLen);

    if (!_mpBuf)
    {
      throw Exception(CPPSQLITE_ERROR,
		      (char*) "Cannot allocate memory",
		      DONT_DELETE_MSG);
    }

    _mbEncoded = false;

    return _mpBuf;
  }

  void
  Binary::clear()
  {
    if (_mpBuf)
    {
      _mnBinaryLen = 0;
      _mnBufferLen = 0;
      free(_mpBuf);
      _mpBuf = 0;
    }
  }
}
