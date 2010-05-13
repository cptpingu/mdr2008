#ifndef CONFIGURATION_HH_
# define CONFIGURATION_HH_

# include "Singleton.hh"
# include <iostream>

class Configuration : public Singleton<Configuration>
{
  friend class Singleton<Configuration>;

public:
  const std::string& getMode() const;
  const std::string& getDatabaseName() const;
  const std::string& getStemmerName() const;
  const std::string& getStopwordFilename() const;
  bool getVerbose() const;

  void setMode(const std::string& mode);
  void setDatabaseName(const std::string& dbName);
  void setStemmerName(const std::string& stemmerName);
  void setStopwordFilename(const std::string& stopwordFilename);
  void setVerbose(const bool verbose);

private:
  std::string		_mode;
  std::string		_databaseName;
  std::string		_stemmerName;
  std::string		_stopwordFilename;
  bool			_verbose;
};

# include "Configuration.hxx"

#endif /* !CONFIGURATION_HH_ */
