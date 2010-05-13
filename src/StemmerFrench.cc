#include "StemmerFrench.hh"
#include "Utils.hh"

namespace Stemmer
{
  /*!
  ** Construct a french stemmer.
  */
  French::French()
  {
  }

  /*!
  ** Destruct a french stemmer.
  */
  French::~French()
  {
  }

  /*!
  ** Stem the given word, using french language.
  **
  ** @param word The word to stem
  **
  ** @return The stemmed word
  */
  const std::string
  French::getStem(const std::string& word)
  {
    reset();
    std::wstring _original = Utils::my_widen(boost::to_lower_copy(word));
    _stem = _original;
    applyConsonance();
    /* FIXME : delete this debug line*/std::wstring accent = _stem;
    computeRV();
    computeR1();
    computeR2();

    bool altered = step1StandardSuffixRemoval();
    // FIXME : check this
    if (!altered)
    {
      altered = step2aDeleteVerbSuffixesBeginningWithI();
      if (!altered)
	altered = step2bDeleteOtherVerbSuffixes();
    }
    if (altered)
      altered = step3ReplaceResidualLetters();
    if (!altered)
      step4ReplaceResidualSuffixes();

    step5UnDouble();
    step6UnAccent();
    boost::to_lower(_stem);

//     std::cout << "Original is : " << Utils::my_narrow(_original) << std::endl;
//     std::cout << "Accent   is : " << Utils::my_narrow(accent) << std::endl;
//     std::cout << "Stem     is : " << Utils::my_narrow(_stem) << std::endl;
//     std::cout << "RV       is : " << Utils::my_narrow(_rv) << std::endl;
//     std::cout << "R1       is : " << Utils::my_narrow(_r1) << std::endl;
//     std::cout << "R2       is : " << Utils::my_narrow(_r2) << std::endl;
//     std::cout << std::endl;

    return Utils::my_narrow(_stem);
  }

  /*!
  ** Assume the word is in lower case. Then put into upper
  ** case u or i preceded and followed by a vowel, and y preceded
  ** or followed by a vowel. u after q is also put into upper case.
  **
  ** For example:
  **    jouer 	-> joUer
  **    ennuie 	-> ennuIe
  **    yeux 	-> Yeux
  **    quand 	-> qUand
  **
  ** @return If a modification occured
  */
  void
  French::applyConsonance()
  {
    const uint length = _stem.length();
    for (uint i = 0; i < length; i++)
    {
      switch (_stem[i])
      {
	case 'u':
	  if (i > 0 && _stem[i - 1] == 'q')
	  {
	    _stem[i] = 'U';
	    break;
	  }
	  /* No break here */
	case 'i':
	  if (i > 0 && i < length && isVowel(_stem[i - 1]) && isVowel(_stem[i + 1]))
	    _stem[i] = std::toupper(_stem[i]);
	  break;
	case 'y':
	  if ((i > 0 && isVowel(_stem[i - 1])) || (i < length && isVowel(_stem[i + 1])))
	    _stem[i] = 'Y';
	  break;
	default:
	  /* We don't care of other character, so we do nothing */
	  break;
      }
    }
  }

  /*!
  ** Delete suffixes, when it's necessary
  */
  bool
  French::step1StandardSuffixRemoval()
  {
    bool res = isSuffixe(L"amment") || isSuffixe(L"emment") ||
      isSuffixe(L"ments") || isSuffixe(L"ment");

    // Just delete if in R2
    {
      static const std::wstring tab1[] = {L"ances", L"ance",
					  L"ismes", L"isme",
					  L"ables", L"able",
					  L"iqUes", L"iqUe",
					  L"istes", L"iste",
					  L"eux",
					  L""};
      for (uint i = 0; tab1[i] != L""; i++)
	if (deleteIfInR2(tab1[i]))
	  return true;
    }

    // Delete if in R2, and check if preceded by ic.
    // If preceded by ic, try to delete if in R2.
    // If we can't delete, else replace by iqU.
    {
      static const std::wstring tab2[] = {L"atrices", L"atrice",
					  L"ateurs", L"ateur",
					  L"ations", L"ation",
					  L""};
      for (uint i = 0; tab2[i] != L""; i++)
	if (deleteIfInR2(tab2[i]))
	{
	  uint length = _stem.length();
	  if (length > 2 && _stem[length - 2] == L'i' && _stem[length - 1] == L'c')
	  {
	    _stem = _stem.substr(0, length - 2);
	    if (!isInR2(std::wstring(L"ic") + tab2[i]))
	      _stem += L"iqU";
	  }
	  return true;
	}
    }

    // Replace with log if in R2
    {
      static const std::wstring tab3[] = {L"logies", L"logie", L""};
      for (uint i = 0; tab3[i] != L""; i++)
	// FIXME : check if isInRv or isInR2 (doc indcates R2, but I found RV better)
	if (isInRV(tab3[i]) && replaceSuffixWith(tab3[i], L"log"))
	  return true;
    }

    // Replace with u if in R2
    {
      static const std::wstring tab4[] = {L"usions", L"usion", L"utions", L"ution", L""};
      for (uint i = 0; tab4[i] != L""; i++)
	if (isInR2(tab4[i]) && replaceSuffixWith(tab4[i], L"u"))
	  return true;
    }

    // Replace with ent if in R2
    {
      static const std::wstring tab5[] = {L"ences", L"ence", L""};
      for (uint i = 0; tab5[i] != L""; i++)
	if (isInR2(tab5[i]) && replaceSuffixWith(tab5[i], L"ent"))
	  return true;
    }

    // Delete if in R1 and preceded by a non-vowel
    {
      static const std::wstring tab10[] = {L"issements", L"issement", L""};
      for (uint i = 0; tab10[i] != L""; i++)
	if (!isVowel(_stem[_stem.length() - 1 - tab10[i].length()]) && deleteIfInR1(tab10[i]))
	  return true;
    }

    // Delete if in RV
    // If preceded by iv, delete if in R2 (and if further preceded
    //   by at, delete if in R2), otherwise,
    // If preceded by eus, delete if in R2, else replace by eux if in R1, otherwise,
    // If preceded by abl or iqU, delete if in R2, otherwise,
    // If preceded by ièr or Ièr, replace by i if in RV
    {
      static const std::wstring tab6[] = {L"ements", L"ement", L""};
      for (uint i = 0; tab6[i] != L""; i++)
      {
	deleteIfInRV(tab6[i]);
	if ((isSuffixe(L"ativ") && deleteIfInR2(L"ativ")) ||
	    (isSuffixe(L"iv") && deleteIfInR2(L"iv")))
	  return true;

	if ((isSuffixe(L"eus")))
	{
	  if (deleteIfInR2(L"eus"))
	    return true;

	  if (isInR1(L"eus") && replaceSuffixWith(L"eus", L"eux"))
	    return true;
	}
	if ((isSuffixe(L"abl") && deleteIfInR2(L"abl")) ||
	    (isSuffixe(L"iqU") && deleteIfInR2(L"iqU")))
	  return true;

	if (isInRV(L"ièr") && replaceSuffixWith(L"ièr", L"i"))
	  return true;

	if (isInRV(L"Ièr") && replaceSuffixWith(L"Ièr", L"i"))
	  return true;
      }
    }

    // Delete if in R2
    // If preceded by abil, delete if in R2, else replace by abl, otherwise,
    // If preceded by ic, delete if in R2, else replace by iqU, otherwise,
    // If preceded by iv, delete if in R2
    {
      static const std::wstring tab7[] = {L"ités", L"ité", L""};
      for (uint i = 0; tab7[i] != L""; i++)
      {
	bool res = deleteIfInR2(tab7[i]);
	if (isSuffixe(L"abil"))
	{
	  if (deleteIfInR2(L"abil"))
	    return true;
	  if (replaceSuffixWith(L"abil", L"abl"))
	    return true;
	}
	if (isSuffixe(L"ic"))
	{
	  if (deleteIfInR2(L"ic"))
	    return true;
	  if (replaceSuffixWith(L"ic", L"iqU"))
	    return true;
	}
	if (isSuffixe(L"iv") && deleteIfInR2(L"iv"))
	  return true;
	if (res)
	  return true;
      }
    }

    // Delete if in R2
    // If preceded by at, delete if in R2 (and if further preceded by ic,
    // delete if in R2, else replace by iqU)
    {
      static const std::wstring tab8[] = {L"ives", L"ifs", L"ive", L"if", L""};
      for (uint i = 0; tab8[i] != L""; i++)
      {
	deleteIfInR2(tab8[i]);
	if (isSuffixe(L"at"))
	{
	  deleteIfInR2(L"at");
	  if (isSuffixe(L"ic"))
	  {
	    if (deleteIfInR2(L"ic"))
	      return true;
	    replaceSuffixWith(L"ic", L"iqU");
	  }
	  return true;
	}
      }
    }

    // Replace with eau
    {
      if (isSuffixe(L"eaux"))
	return replaceSuffixWith(L"eaux", L"eau");
    }

    // Replace with al if in R1
    {
      if (isSuffixe(L"aux"))
	if (isInR1(L"aux") && replaceSuffixWith(L"aux", L"al"))
	  return true;
    }

    // Delete if in R2, else replace by eux if in R1
    {
      static const std::wstring tab9[] = {L"euses", L"euse", L""};
      for (uint i = 0; tab9[i] != L""; i++)
      {
	if (deleteIfInR2(tab9[i]))
	  return true;
	if (isInR1(tab9[i]))
	{
	  replaceSuffixWith(tab9[i], L"eux");
	  return true;
	}
      }
    }

    // Replace with ant if in RV
    {
      // FIXME : this
      if (/*isInRV(L"amment")*/ _rv == L"amment" && replaceSuffixWith(L"amment", L"ant"))
	return true;
      if (isInRV(L"amment") && replaceSuffixWith(L"amment", L""))
	return true;
    }

    // Replace with ent if in RV
    {
      if (isInRV(L"emment") && replaceSuffixWith(L"emment", L"ent"))
	return true;
    }

    // Delete if preceded by a vowel in RV
    // FIXME : I don't understand everything, so check this
    {
      static const std::wstring tab11[] = {L"ments", L"ment", L""};
      for (uint i = 0; tab11[i] != L""; i++)
      {
	if (_rv.length() - 1 >= tab11[i].length())
	{
	  int pos = _rv.length() - 1 - tab11[i].length();
	  if (isSuffixeInRV(tab11[i]) && isVowel(_rv[pos]))
	  {
	    _stem = _stem.substr(0, getSuffixPos(tab11[i]));
	    return true;
	  }
	}
      }
    }

    // Arrived here, we assume that no modification has affected the stem
    // So we just return if amment, emment, ments or ment occured.
    return res;
  }

  /*!
  ** Search for the longest among the following suffixes and if found,
  ** delete if preceded by a non-vowel.
  **
  ** @return If at least one suffix has been removed
  */
  bool
  French::step2aDeleteVerbSuffixesBeginningWithI()
  {
    static const std::wstring tab[] = {
      L"issaIent", L"issantes",
      L"issions", L"issante", L"issants", L"iraIent",
      L"issons", L"irions", L"issiez", L"issant", L"issent", L"issais", L"issait",
      L"irais", L"isses", L"issez", L"irent", L"irons", L"iront", L"iriez", L"irait",
      L"isse", L"îmes", L"îtes", L"irai", L"iras", L"irez",
      L"ies", L"ira",
      L"it", L"is", L"ie", L"ir", L"ît",
      L"i",
      L""};
    for (uint i = 0; tab[i] != L""; i++)
    {
      if (_rv.length() - 1 >= tab[i].length())
      {
	uint pos = _rv.length() - 1 - tab[i].length();
	if (!isVowel(_rv[pos]) && deleteIfInRV(tab[i]))
	  return true;
      }
    }

    // Arrived here, we assume that no modification has affected the stem
    return false;
  }

  /*!
  ** Search for the longest among the following suffixes,
  ** and perform different actions.
  **
  ** @return If at least one suffix has been removed
  */
  bool
  French::step2bDeleteOtherVerbSuffixes()
  {
    // Delete ions if in R2
    if (deleteIfInR2(L"ions"))
      return true;

    // Delete if in RV
    {
      static const std::wstring tab[] = {
	L"eraIent",
	L"erions",
	L"erais", L"erait", L"eriez", L"erons", L"eront", L"èrent",
	L"eras", L"erai", L"erez",
	L"iez", L"ées", L"era",
	L"és", L"ez", L"ée", L"er",
	L"é",
	L""};

      for (uint i = 0; tab[i] != L""; i++)
	if (isSuffixe(tab[i]) && deleteIfInRV(tab[i]))
	  return true;
    }

    // Delete if in RV
    // If an e precede, and e his in RV, delete it too
    {
      static const std::wstring tab[] = {
	L"assions",
	L"assent", L"assiez",
	L"antes", L"asses", L"aIent",
	L"âtes", L"ante", L"ants", L"asse", L"âmes",
	L"ais", L"ait", L"ant",
	L"ât", L"as", L"ai",
	L"a",
	L""};
      for (uint i = 0; tab[i] != L""; i++)
      {
	bool res = isSuffixe(tab[i]) && deleteIfInRV(tab[i]);
	if (res)
	{
	  if (_rv.length() - 1 >= tab[i].length())
	  {
	    uint pos = _rv.length() - 1 - tab[i].length();
	    if ((_rv[pos] == L'e'))
	    {
	      _stem = _stem.substr(0, _stem.length() - 1);
	      res = true;
	    }
	  }
	  return true;
	}
      }
    }

    // Arrived here, we assume that no modification has affected the stem
    return false;
  }

  /*!
  ** Replace final Y with i or final ç with c
  **
  ** @return If at least one suffix has been removed
  */
  bool
  French::step3ReplaceResidualLetters()
  {
    uint last = _stem.length() - 1;

    if (_stem[last] == L'Y')
    {
      _stem[last] = L'i';
      return true;
    }

    if (_stem[last] == L'ç')
    {
      _stem[last] = L'c';
      return true;
    }

    // Arrived here, we assume that no modification has affected the stem
    return false;
  }

  /*!
  ** If the word ends s, not preceded by a, i, o, u, è or s, delete it.
  ** In the rest of step 4, all tests are confined to the RV region.
  ** Search for the longest among the following suffixes, and perform
  ** the action indicated.
  */
  void
  French::step4ReplaceResidualSuffixes()
  {
    if (_stem.length() < 1)
      return;

    // If the word ends s, not preceded by a, i, o, u, è or s, delete it.
    {
      uint length = _stem.length();
      uint pos = length - 2;
      if (_stem[length - 1] == L's' &&
	  _stem[pos] != L'a' && _stem[pos] != L'i' &&
	  _stem[pos] != L'o' && _stem[pos] != L'u' &&
	  _stem[pos] != L'è' && _stem[pos] != L's')
	_stem = _stem.substr(0, length - 1);
    }

    // Delete if in R2 and preceded by s or t
    {
      if (_rv.length() - 1 > 3)
      {
	uint pos = _rv.length() - 1 - 3;
	if (pos != 0 && isSuffixeInRV(L"ion") &&
	    (_rv[pos] == L's' || _rv[pos] == L't'))
	  deleteIfInR2(L"ion");
      }
    }

    // Replace with i
    {
      static const std::wstring tab[] = {
	L"Ière", L"ière", L"Ier", L"ier",
	L""};
      for (uint i = 0; tab[i] != L""; i++)
	if (isSuffixeInRV(tab[i]))
	  replaceSuffixWith(tab[i], L"i");
    }

    // Delete e
    {
      uint length = _stem.length();
      if (/*isSuffixeInRV(L"e") &&*/ isSuffixe(L"e"))
	_stem = _stem.substr(0, length - 1);
    }

    // Delete ë if preceded by gu
    {
      uint length = _stem.length();
      if (isSuffixeInRV(L"guë"))
	_stem = _stem.substr(0, length - 1);
      //replaceSuffixWith("ë", "");
      // FIXME : check line above, _stem = _stem.substr(0, length - 1);
    }
  }

  /*!
  ** If the word ends enn, onn, ett, ell or eill, delete the last letter.
  */
  void
  French::step5UnDouble()
  {
    uint length = _stem.length();
    static const std::wstring tab[] = {
      L"enn", L"onn", L"ett", L"ell", L"eill",
      L""};
    for (uint i = 0; tab[i] != L""; i++)
      if (isSuffixe(tab[i]))
	_stem = _stem.substr(0, length - 1);
  }

  /*!
  ** If the words ends é or è followed by at least one non-vowel, remove
  ** the accent from the e.
  */
  void
  French::step6UnAccent()
  {
    uint length = _stem.length();
    if (length < 2)
      return;

    if ((_stem[length - 2] == L'é' || _stem[length - 2] == L'è') &&
	!isVowel(_stem[length - 1]))
      _stem[length - 2] = L'e';
  }
}
