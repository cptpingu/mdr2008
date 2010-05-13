#include <cassert>
#include "Utils.hh"

/*!
** Convert a char to a string.
**
** @param c The char to convert
**
** @return A string from the given char
*/
std::string
Utils::charToString(char c)
{
  char tab[2] = { 0, 0};

  tab[0] = c;

  return tab;
}

/*!
** Create a string of given size constitued by the given character.
**
** @param c The character use to fill the string
** @param size The size the string will have
**
** @return A string constituted with given arguments
*/
const std::string
Utils::stringFill(const char c, const unsigned int size)
{
  const std::string fill(size, c);
  return fill;
}

/*!
** Convert a boolean to string.
**
** @param t The boolean
**
** @return A string corresponding to boolean
*/
const std::string
Utils::boolToString(const bool t)
{
  if (t)
    return "true";
  return "false";
}

/*!
** Convert a string to a boolean.
**
** @param t The string
**
** @return True if "true, and "false if false.
*/
bool
Utils::stringToBool(const std::string& t)
{
  if (t == "true")
    return true;
  if (t == "false")
    return false;

  assert(false);
  return false;
}

/*!
** Convert an int to a string.
**
** @param i The argument
**
** @return A string corresponding to the given integer
*/
const std::string
Utils::intToString(const int i)
{
  std::ostringstream out;
  out << i;
  return out.str();
}

/*!
** Convert a string to an int.
**
** @param s The string to convert
**
** @return An integer value of passed string
*/
unsigned int
Utils::stringToInt(const std::string s)
{
  unsigned int i = 0;
  fromString<unsigned int>(i, s);
  return i;
}

/*!
** Convert a string to double.
**
** @param s The string to convert
**
** @return A double value of passed string
*/
double
Utils::stringToDouble(const std::string s)
{
  double i = 0;
  fromString<double>(i, s);
  return i;
}

/*!
** Transform a wide UTF-8 string to an UNICODE string one.
**
** @param ws The wide UTF-8 string
**
** @return An UNICODE string
*/
std::string
Utils::narrow(const std::wstring& ws)
{
  std::vector<char> buffer(ws.size());
  std::locale loc("fr_FR.utf8");
  std::use_facet< std::ctype<wchar_t> >(loc).narrow(ws.data(), ws.data() + ws.size(), '?', &buffer[0]);

  return std::string(&buffer[0], buffer.size());
}
std::string
Utils::my_narrow(const std::wstring& ws)
{
  std::ostringstream s;
  unsigned int length = ws.length();
  for (unsigned int i = 0; i < length; i++)
  {
    switch (ws[i])
    {
      case L'â': s << "â"; break;
      case L'à': s << "à"; break;
      case L'ç': s << "ç"; break;
      case L'ë': s << "ë"; break;
      case L'é': s << "é"; break;
      case L'ê': s << "ê"; break;
      case L'è': s << "è"; break;
      case L'ï': s << "ï"; break;
      case L'î': s << "î"; break;
      case L'ô': s << "ô"; break;
      case L'û': s << "û"; break;
      case L'ù': s << "ù"; break;
      default: s << static_cast<unsigned char>(ws[i]);
    }
  }

  return s.str();
}

/*!
** Renarrow a string.
** It's an ugly ugly hack to make things works.
** Fucking encoding !
**
** @param s The string to renarrow
**
** @return The renarrowed string
*/
std::string
Utils::renarrow(const std::string& s)
{
  std::ostringstream ws;
  unsigned int length = s.length();
  for (unsigned int i = 0; i < length; i++)
  {
    if (i + 1 < length && s[i] == -61)
    {
      switch (s[i + 1])
      {
	case -94: ws << "â"; break;
	case -96: ws << "à"; break;
	case -89: ws << "ç"; break;
	case -85: ws << "ë"; break;
	case -87: ws << "é"; break;
	case -86: ws << "ê"; break;
	case -88: ws << "è"; break;
	case -81: ws << "ï"; break;
	case -82: ws << "î"; break;
	case -76: ws << "ô"; break;
	case -69: ws << "û"; break;
	case -71: ws << "ù"; break;
      }
      i++;
    }
    else
    {
      switch (s[i])
      {
	case -30: ws << "â"; break;
	case -28: ws << "ä"; break;
	case -32: ws << "à"; break;
	case -25: ws << "ç"; break;
	case -21: ws << "ë"; break;
	case -23: ws << "é"; break;
	case -22: ws << "ê"; break;
	case -24: ws << "è"; break;
	case -17: ws << "ï"; break;
	case -18: ws << "î"; break;
	case -12: ws << "ô"; break;
	case -5:  ws << "û"; break;
	case -7: ws << "ù"; break;
	default : ws << s[i];
      }
    }
  }

  return ws.str();
}


/*!
** Transform an UNICODE string to a wide UTF-8 string one.
**
** @param ws The UNICODE string
**
** @return A wide UTF-8 string
*/
std::wstring
Utils::widen(const std::string& s)
{
  std::vector<wchar_t> buffer(s.size());
  std::locale loc("fr_FR.utf8");
  std::use_facet< std::ctype<wchar_t> >(loc).widen(s.data(), s.data() + s.size(), &buffer[0]);

  return std::wstring(&buffer[0], buffer.size());
}
std::wstring
Utils::my_widen(const std::string& s)
{
  std::wostringstream ws;
  unsigned int length = s.length();
  for (unsigned int i = 0; i < length; i++)
  {
    if (i + 1 < length && s[i] == -61)
    {
      switch (s[i + 1])
      {
	case -94: ws << L'â'; break;
	case -96: ws << L'à'; break;
	case -89: ws << L'ç'; break;
	case -85: ws << L'ë'; break;
	case -87: ws << L'é'; break;
	case -86: ws << L'ê'; break;
	case -88: ws << L'è'; break;
	case -81: ws << L'ï'; break;
	case -82: ws << L'î'; break;
	case -76: ws << L'ô'; break;
	case -69: ws << L'û'; break;
	case -71: ws << L'ù'; break;
      }
      i++;
    }
    else
      ws << static_cast<wchar_t>(s[i]);
  }

  return ws.str();
}
