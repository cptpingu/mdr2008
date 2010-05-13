namespace Stemmer
{
  /*!
  ** Get the position of the very first vowel of a word.
  **
  ** @param word The given word
  **
  ** @return Last vowel position
  */
  inline uint
  French::getFirstVowelPos(const std::wstring& word)
  {
    uint length = word.length();

    for (uint i = 0; i < length; i++)
      if (isVowel(word[i]))
	return i;

    return length - 1;
  }

  /*!
  ** Get the position of the very last vowel of a word.
  **
  ** @param word The given word
  **
  ** @return Last vowel position
  */
  inline uint
  French::getLastVowelPos(const std::wstring& word)
  {
    uint length = word.length();

    for (uint i = length - 1; i != 0; i--)
      if (isVowel(word[i]))
	return i;

    return length - 1;
  }

  /*!
  ** Clean all values.
  */
  inline void
  French::reset()
  {
    _rv = L"";
    _r1 = L"";
    _r2 = L"";
  }

  /*!
  ** Check if given character is a french vowel.
  ** Uppercase is not consider as a vowel.
  **
  ** @param c The chararacter to test
  **
  ** @return If given character is a french vowel
  */
  inline bool
  French::isVowel(const wchar_t c)
  {
    return VOWELS.find_first_of(c) != std::wstring::npos;
  }

  /*!
  ** If the word begins with two vowels, RV is the region after
  ** the third letter, otherwise the region after the first vowel
  ** not at the beginning of the word, or the end of the word if
  ** these positions cannot be found. (Exceptionally, par, col or
  ** tap, at the begining of a word is also taken to define RV as
  ** the region to their right.)
  **
  ** For example,
  **
  **    a i m e r     a d o r e r     v o l e r    t a p i s
  **         |...|         |.....|       |.....|        |...|
  */
  inline void
  French::computeRV()
  {
    uint length = _stem.length();

    // Handle par, col and tap exception
    const std::wstring prefix = _stem.substr(0, 3);
    if (prefix == L"par" || prefix == L"col" || prefix == L"tap")
    {
      _rv = _stem.substr(3);
      return;
    }

    // First 2 letters are vowels
    if (length >= 3 && isVowel(_stem[0]) && isVowel(_stem[1]))
    {
      _rv = _stem.substr(2);
      return;
    }

    // Search for rv region, finding second vowel if exists
    uint first_vowels_pos = getFirstVowelPos(_stem);
    uint last_vowels_pos = getLastVowelPos(_stem);
    for (uint i = first_vowels_pos + 1; i < length; i++)
    {
      if (isVowel(_stem[i]) && i != last_vowels_pos)
      {
	_rv = _stem.substr(i + 1);
	return;
      }
    }

    _rv = _stem;
  }

  /*!
  ** R1 is the region after the first non-vowel following a vowel,
  ** or the end of the word if there is no such non-vowel.
  **
  ** For example:
  **
  **    f a m e u s e m e n t
  **         |......R1.......|
  **
  */
  inline void
  French::computeR1()
  {
    uint length = _stem.length();
    uint pos = getFirstVowelPos(_stem);
    uint resPos = pos + 1;
    for (uint i = pos + 1; i < length; i++)
      if (!isVowel(_stem[i]))
      {
	resPos = i + 1;
	break;
      }

    _r1 = _stem.substr(resPos);
  }

  /*!
  ** R2 is the region after the first non-vowel following a vowel in R1,
  ** or the end of the word if there is no such non-vowel.
  **
  ** For example:
  **
  **    f a m e u s e m e n t
  **               |...R2....|
  **
  */
  inline void
  French::computeR2()
  {
    uint length = _r1.length();
    uint pos = getFirstVowelPos(_r1);
    uint resPos = pos + 1;
    for (uint i = pos + 1; i < length; i++)
      if (!isVowel(_r1[i]))
      {
	resPos = i + 1;
	break;
      }

    _r2 = _r1.substr(resPos);
  }

  /*!
  ** Try to get the given suffix position in the stem.
  **
  ** @param suffix The suffix to search
  **
  ** @return Position of the suffix, ie last occurence of
  ** given string, or std::wstring::npos if not found
  */
  inline uint
  French::getSuffixPos(const std::wstring& suffix)
  {
    uint pos = _stem.rfind(suffix);
    return (_stem.length() - pos == suffix.length()) ? pos : std::wstring::npos;
  }

  /*!
  ** Try to get the given suffix position in RV.
  **
  ** @param suffix The suffix to search
  **
  ** @return Position of the suffix, ie last occurence of
  ** given string, or std::wstring::npos if not found
  */
  inline uint
  French::getSuffixPosInRV(const std::wstring& suffix)
  {
    uint pos = _rv.rfind(suffix);
    return (_rv.length() - pos == suffix.length()) ? pos : std::wstring::npos;
  }

  /*!
  ** Check if given string can be a suffix for the current stem.
  **
  ** @param suffix The word to check
  **
  ** @return If the given string can be a suffix
  */
  inline bool
  French::isSuffixe(const std::wstring& suffix)
  {
    return getSuffixPos(suffix) != std::wstring::npos;
  }

  /*!
  ** Check if given string can be a suffix in RV.
  **
  ** @param suffix The word to check
  **
  ** @return If the given string can be a suffix
  */
  inline bool
  French::isSuffixeInRV(const std::wstring& suffix)
  {
    return getSuffixPosInRV(suffix) != std::wstring::npos;
  }

  /*!
  ** Replace a suffix with the given string.
  **
  ** @param suffix The suffix to search
  ** @param with The string for replacement
  **
  ** @return If a replacement occured
  */
  inline bool
  French::replaceSuffixWith(const std::wstring& suffix, const std::wstring& with)
  {
    uint pos = getSuffixPos(suffix);
    if (pos != std::wstring::npos)
    {
      _stem = _stem.substr(0, pos);
      _stem += with;
      return true;
    }

    return false;
  }

  /*!
  ** Check if a suffix lies in R1.
  **
  ** @param suffix The suffix to check
  **
  ** @return If given suffix lies in R1
  */
  inline bool
  French::isInR1(const std::wstring& suffix)
  {
    return _r1.find(suffix) != std::wstring::npos;
  }

  /*!
  ** Check if a suffix lies in R2.
  **
  ** @param suffix The suffix to check
  **
  ** @return If given suffix lies in R2
  */
  inline bool
  French::isInR2(const std::wstring& suffix)
  {
    return _r2.find(suffix) != std::wstring::npos;
  }

  /*!
  ** Check if a suffix lies in RV.
  **
  ** @param suffix The suffix to check
  **
  ** @return If given suffix lies in RvV
  */
  inline bool
  French::isInRV(const std::wstring& suffix)
  {
    return _rv.find(suffix) != std::wstring::npos;
  }

  /*!
  ** Delete if in R2 means that a found suffix should be removed
  ** if it lies entirely in R2, but not if it overlaps R2 and
  ** the rest of the word.
  **
  ** @param suffix The suffix of the term to delete
  **
  ** @return If deletion occured or not
  */
  inline bool
  French::deleteIfInR1(const std::wstring& suffix)
  {
    uint pos = getSuffixPos(suffix);
    if (pos == std::wstring::npos)
      return false;

    if (isInR1(suffix) && _stem != suffix)
    {
      _stem = _stem.substr(0, pos);
      return true;
    }

    return false;
  }

  /*!
  ** Delete if in R2 means that a found suffix should be removed
  ** if it lies entirely in R2, but not if it overlaps R2 and
  ** the rest of the word.
  **
  ** @param suffix The suffix of the term to delete
  **
  ** @return If deletion occured or not
  */
  inline bool
  French::deleteIfInR2(const std::wstring& suffix)
  {
    uint pos = getSuffixPos(suffix);
    if (pos == std::wstring::npos)
      return false;

    if (isInR2(suffix) && _stem != suffix)
    {
      _stem = _stem.substr(0, pos);
      return true;
    }

    return false;
  }

  /*!
  ** Delete if in RV means that a found suffix should be removed
  ** if it lies entirely in RV, but not if it overlaps RV and
  ** the rest of the word.
  **
  ** @param suffix The suffix of the term to delete
  **
  ** @return If deletion occured or not
  */
  inline bool
  French::deleteIfInRV(const std::wstring& suffix)
  {
    uint pos = getSuffixPos(suffix);
    if (pos == std::wstring::npos)
      return false;

    if (isInRV(suffix) && _stem != suffix)
    {
      _stem = _stem.substr(0, pos);
      return true;
    }

    return false;
  }
}
