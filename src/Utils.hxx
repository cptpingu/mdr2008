/*!
** Get an element from a std::vector.
**
** @param elt The element to get
** @param tab The table
**
** @return Founded element
*/
template<typename T>
T
getEltFromArray(const T elt, const std::vector<T> tab)
{
  typename std::vector<T>::const_iterator result;

  return find(tab.begin(), tab.end(), elt);
}

/*!
** Get an element from a classic table.
**
** @param elt The element to get
** @param tab The table
**
** @return Founded element
*/
template<typename T>
T
getEltFromArray(const T elt, const T* const tab)
{
  while (*tab)
  {
    if (elt == *tab)
      return elt;
    tab++;
  }

  return 0;
}

/*!
** Check if an element is contains by the given vector.
**
** @param elt The element to get
** @param tab The table
**
** @return If element was found
*/
template<typename T>
bool
isInArray(const T elt, const std::vector<T> tab)
{
  return getEltFromArray(elt, tab) != tab.end();
}

/*!
** Check if an element is contains by the given classic table.
**
** @param elt The element to get
** @param tab The table
**
** @return If element was found
*/
template<typename T>
bool
isInArray(const T elt, const T* const tab)
{
  return getEltFromArray(elt, tab) != 0;
}

/*!
** Convert a string to a choosen type
**
** @param t The dst type
** @param s The string to convert
** @param f The stream option
**
** @return The dst type
*/
template <typename T>
bool
Utils::fromString(T& t,
		  const std::string& s,
		  std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss (s);

  return !(iss >> f >> t).fail ();
}

/*!
** Interpret special char like \n
**
** @param s The string where the special char were
**
** @return The modified string
*/
inline const std::string
Utils::activeSpecialChar(const std::string& s)
{
  unsigned int i = 0;
  unsigned int size = s.length();
  std::string stringExpr = "";

  while (i < size)
  {
    if (s[i] == '\\' && i + 1 < size)
    {
      if (s[i + 1] == 'n')
	stringExpr += '\n';
      else
	if (s[i + 1] == 'f')
	  stringExpr += '\f';
	else
	  if (s[i + 1] == 'v')
	    stringExpr += '\v';
	  else
	    if (s[i + 1] == 'r')
	      stringExpr += '\r';
	    else
	      if (s[i + 1] == 't')
		stringExpr += '\t';
	      else
		stringExpr += s[i + 1];
      i++;
    }
    else
      stringExpr += s[i];
    i++;
  }

  return stringExpr;
}

/*!
** Check if a file exists.
**
** @param filename The file to test
**
** @return If given file exists
*/
inline bool
Utils::fileExists(const std::string& filename)
{
  bool flag = false;
  std::fstream fin;

  fin.open(filename.c_str(), std::ios::in);
  if (fin.is_open())
    flag = true;
  fin.close();

  return flag;
}

/*!
** Special widen, transform weird character to a better one.
**
** @param s The string to modify
*/
inline void
Utils::rewiden(std::string& buf)
{
  static const std::pair<int, std::string> tab[] = {
    std::make_pair(-5, "â"),
    std::make_pair(-5, "à"),
    std::make_pair(-5, "ç"),
    std::make_pair(-5, "ë"),
    std::make_pair(-5, "é"),
    std::make_pair(-5, "ê"),
    std::make_pair(-5, "è"),
    std::make_pair(-5, "ï"),
    std::make_pair(-5, "î"),
    std::make_pair(-5, "ô"),
    std::make_pair(-5, "û"),
    std::make_pair(-5, "ù"),
    std::make_pair(0, "")
  };

  std::ostringstream s;
  for (std::string::iterator i = buf.begin(); i != buf.end(); ++i)
  {
    int code = (int)(*i);
    switch (code)
    {
      case -5: s << "â"; break;
      case -96: s << "à"; break;
      case -89: s << "ç"; break;
      case -85: s << "ë"; break;
      case -87: s << "é"; break;
      case -86: s << "ê"; break;
      case -88: s << "è"; break;
      case -81: s << "ï"; break;
      case -82: s << "î"; break;
      case -76: s << "ô"; break;
      case -69: s << "û"; break;
      case -71: s << "ù"; break;
      default: s << *i;
    }
  }

  buf = s.str();
}
