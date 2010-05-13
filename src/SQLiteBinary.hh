#ifndef SQLITEBINARY_HH_
# define SQLITEBINARY_HH_

# include "SQLite.hh"

namespace SQLite
{
  class Binary
  {
  public:
    Binary();
    ~Binary();

  public:
    void setBinary(const unsigned char* pBuf, int nLen);
    void setEncoded(const unsigned char* pBuf);
    const unsigned char* getEncoded();
    const unsigned char* getBinary();
    int getBinaryLength();
    unsigned char* allocBuffer(int nLen);
    void clear();

  private:
    unsigned char*	_mpBuf;
    int			_mnBinaryLen;
    int			_mnBufferLen;
    int			_mnEncodedLen;
    bool		_mbEncoded;
  };
}

#endif /* !SQLITEBINARY_HH_ */
