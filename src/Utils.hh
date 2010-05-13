#ifndef UTILS_HH_
# define UTILS_HH_

# include <string>
# include <vector>
# include <sstream>
# include <fstream>
# include <iostream>
# include <locale>

class Utils
{
public:
  template <typename T>
  static T getEltFromArray(const T elt, const std::vector<T> tab);
  template <typename T>
  static T getEltFromArray(const T elt,  const T* const tab);
  template <typename T>
  static bool isInArray(const T elt, const std::vector<T> tab);
  template <typename T>
  static bool isInArray(const T elt, const T* const tab);

public:
  static std::string charToString(char c);
  template <typename T>
  static bool fromString(T& t, const std::string& s,
			 std::ios_base& (*f)(std::ios_base&) = std::dec);
  static const std::string stringFill(const char c, const unsigned int size);
  static const std::string boolToString(const bool t);
  static bool stringToBool(const std::string& t);
  static const std::string intToString(const int i);
  static unsigned int stringToInt(const std::string s);
  static double stringToDouble(const std::string s);
  static const std::string activeSpecialChar(const std::string& s);
  static bool fileExists(const std::string& filename);
  static std::string narrow(const std::wstring& ws);
  static std::wstring widen(const std::string& s);
  static std::string my_narrow(const std::wstring& ws);
  static std::wstring my_widen(const std::string& s);
  static void rewiden(std::string& s);
  static std::string renarrow(const std::string& s);
};

# include "Utils.hxx"

#endif /* !UTILS_HH_ */
