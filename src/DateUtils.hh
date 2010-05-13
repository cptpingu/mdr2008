#ifndef DATEUTILS_HH_
# define DATEUTILS_HH_

# include "Utils.hh"
# include <boost/date_time.hpp>

class DateUtils
{
public:
  static std::string timestampToStringDate(const unsigned long timestamp);
  static std::string timestampToStringDate(const std::string timestamp);
  static unsigned long stringDateToTimestamp(const std::string& date);
};

#endif /* !DATEUTILS_HH_ */
