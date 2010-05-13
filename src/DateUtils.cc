#include "DateUtils.hh"

/*!
** Convert a timestamp to a formatted date.
**
** @param timestamp The string timestamp
**
** @return A formatted date
*/
std::string
DateUtils::timestampToStringDate(const unsigned long timestamp)
{
  std::tm time;
  std::time_t ctimestamp = static_cast<std::time_t>(timestamp);
  boost::date_time::c_time::localtime(&ctimestamp, &time);
  std::ostringstream format;
  format << time.tm_mday << '/' << time.tm_mon << '/' << (time.tm_year + 1900);
  return format.str();
}

/*!
** Convert a string timestamp to a formatted date.
**
** @param timestamp The string timestamp
**
** @return A formatted date
*/
std::string
DateUtils::timestampToStringDate(const std::string timestamp)
{
  unsigned long t;
  Utils::fromString(t, timestamp);
  return timestampToStringDate(t);
}

/*!
** Convert a string date format, ie like jj/mm/yyyy to a timestamp.
** Can throw a parse error.
**
** @param date The formatted date.
**
** @return A timestamp
*/
unsigned long
DateUtils::stringDateToTimestamp(const std::string& date)
{
  // FIXME : split chain
  // FIXME : throw an error on invalid date.

  // 4 Cases :
  // * :date(xx/xx/xx)
  // * :date(> xx/xx/xx)
  // * :date(< xx/xx/xx)
  // * :date(xx/xx/xx - xx/xx/xx)
  //
  // Just get the date ? ok

  date.find_first_of('/');

  return 0/*FIXME*/;
}
