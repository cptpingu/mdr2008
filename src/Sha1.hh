#ifndef SHA1_HH_
# define SHA1_HH_

# include <iostream>
# include <cstdio>
# include <cstring>
# include <cstdlib>

namespace Hash
{
# if !defined(SHA1_UTILITY_FUNCTIONS) && !defined(SHA1_NO_UTILITY_FUNCTIONS)
#  define SHA1_UTILITY_FUNCTIONS
# endif

  /*!
  ** You can define the endian mode in your files, without modifying the SHA1
  ** source files. Just #define SHA1_LITTLE_ENDIAN or #define SHA1_BIG_ENDIAN
  ** in your files, before including the SHA1.h header file. If you don't
  ** define anything, the class defaults to little endian.
  */
# if !defined(SHA1_LITTLE_ENDIAN) && !defined(SHA1_BIG_ENDIAN)
#  define SHA1_LITTLE_ENDIAN
# endif

  /*
  ** Same here. If you want variable wiping, #define SHA1_WIPE_VARIABLES, if
  ** not, #define SHA1_NO_WIPE_VARIABLES. If you don't define anything, it
  ** defaults to wiping.
  */
# if !defined(SHA1_WIPE_VARIABLES) && !defined(SHA1_NO_WIPE_VARIABLES)
#  define SHA1_WIPE_VARIABLES
# endif

# ifndef UINT_32
#  ifdef _MSC_VER
#   define UINT_8  unsigned __int8
#   define UINT_32 unsigned __int32
#  else
#   define UINT_8 unsigned char
#   if (ULONG_MAX == 0xFFFFFFFF)
#    define UINT_32 unsigned long
#   else
#    define UINT_32 unsigned int
#   endif
#  endif
# endif


  typedef union
  {
    UINT_8  c[64];
    UINT_32 l[16];
  } SHA1_WORKSPACE_BLOCK;

  class Sha1
  {
  public:
# ifdef SHA1_UTILITY_FUNCTIONS
    // Two different formats for ReportHash(...)
    enum
      {
	REPORT_HEX = 0,
	REPORT_DIGIT = 1
      };
# endif

    Sha1();
    ~Sha1();

  public:
    void reset();
    // Update the hash value
    void update(UINT_8 *data, UINT_32 len);
#ifdef SHA1_UTILITY_FUNCTIONS
    bool hashFile(const char* szFileName);
#endif
    // Finalize hash and report
    void final();
    // Report functions: as pre-formatted and raw data
#ifdef SHA1_UTILITY_FUNCTIONS
    void reportHash(char szReport[], unsigned char uReportType = REPORT_HEX) const;
#endif
    void getHash(UINT_8 *puDest) const;
    std::string getStrHash() const;

  private:
    // Private SHA-1 transformation
    void transform(UINT_32 *state, UINT_8 *buffer);

  private:
    // Member variables
    UINT_32 m_state[5];
    UINT_32 m_count[2];
    UINT_32 __reserved1[1];
    UINT_8  m_buffer[64];
    UINT_8  m_digest[20];
    UINT_32 __reserved2[3];
    UINT_8 m_workspace[64];
    SHA1_WORKSPACE_BLOCK *m_block; // SHA1 pointer to the byte array above
  };
}
#endif /* !SHA1_HH_ */
