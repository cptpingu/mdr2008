#ifndef STEMMERFRENCH_HH_
# define STEMMERFRENCH_HH_

# include "Stemmer.hh"
# include <iostream>
# include <boost/algorithm/string.hpp>
# include <string>

namespace Stemmer
{
  static const std::wstring VOWELS = L"aeiouyâàëéêèïîôûù";

  class French : public Generic
  {
    typedef std::wstring::const_iterator citer;
    typedef std::wstring::iterator iter;
    typedef unsigned int uint;

  public:
    French();
    virtual ~French();
    virtual const std::string getStem(const std::string& word);

  private:
    void reset();
    uint getFirstVowelPos(const std::wstring& word);
    uint getLastVowelPos(const std::wstring& word);
    bool isVowel(const wchar_t c);
    void computeRV();
    void computeR1();
    void computeR2();
    bool isInR1(const std::wstring& suffix);
    bool isInR2(const std::wstring& suffix);
    bool isInRV(const std::wstring& suffix);
    uint getSuffixPos(const std::wstring& suffix);
    bool isSuffixe(const std::wstring& suffix);
    uint getSuffixPosInRV(const std::wstring& suffix);
    bool isSuffixeInRV(const std::wstring& suffix);
    bool replaceSuffixWith(const std::wstring& suffix, const std::wstring& with);
    bool deleteIfInR1(const std::wstring& suffix);
    bool deleteIfInR2(const std::wstring& suffix);
    bool deleteIfInRV(const std::wstring& suffix);

  private:
    void applyConsonance();
    bool step1StandardSuffixRemoval();
    bool step2aDeleteVerbSuffixesBeginningWithI();
    bool step2bDeleteOtherVerbSuffixes();
    bool step3ReplaceResidualLetters();
    void step4ReplaceResidualSuffixes();
    void step5UnDouble();
    void step6UnAccent();


  private:
    std::wstring	_original;
    std::wstring	_stem;
    std::wstring	_rv;
    std::wstring	_r1;
    std::wstring	_r2;
  };
}

# include "StemmerFrench.hxx"

#endif /* !STEMMERFRENCH_HH_ */
