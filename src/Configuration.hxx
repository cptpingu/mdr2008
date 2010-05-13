/*!
** Get the mode.
**
** @return The mode
*/
inline const std::string&
Configuration::getMode() const
{
  return _mode;
}

/*!
** Get the database filename.
**
** @return The database filename
*/
inline const std::string&
Configuration::getDatabaseName() const
{
  return _databaseName;
}

/*!
** Get the stemmer type.
**
** @return The stemmer type
*/
inline const std::string&
Configuration::getStemmerName() const
{
  return _stemmerName;
}

/*!
** Get the stopword filename.
**
** @return The stopword filename
*/
inline const std::string&
Configuration::getStopwordFilename() const
{
  return _stopwordFilename;
}

/*!
** Check if verbose mode is activated
**
** @return If verbose mode activated
*/
inline bool
Configuration::getVerbose() const
{
  return _verbose;
}

/*!
** Set the mode.
**
** @param mode The mode
*/
inline void
Configuration::setMode(const std::string& mode)
{
  _mode = mode;
}

/*!
** Set the database name.
**
** @param dbName The database name
*/
inline void
Configuration::setDatabaseName(const std::string& dbName)
{
  _databaseName = dbName;
}

/*!
** The stemmer name.
**
** @param stemmerName The stemmer name
*/
inline void
Configuration::setStemmerName(const std::string& stemmerName)
{
  _stemmerName = stemmerName;
}

/*!
** The stop word filename.
**
** @param stopwordFilename The stop word filename
*/
inline void
Configuration::setStopwordFilename(const std::string& stopwordFilename)
{
  _stopwordFilename = stopwordFilename;
}

/*!
** Set / unset verbose mode.
**
** @param verbose If we set verbose on or off
*/
inline void
Configuration::setVerbose(const bool verbose)
{
  _verbose = verbose;
}
