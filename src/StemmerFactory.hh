#ifndef STEMMERFACTORY_HH_
# define STEMMERFACTORY_HH_

# include "Stemmer.hh"
# include "StemmerFrench.hh"
# include "StemmerFrenchQuick.hh"

namespace Stemmer
{
  class StemmerFactory
  {
  public:
    static Generic* get(const std::string& type);
  };
}

# include "StemmerFactory.hxx"

#endif /* !STEMMERFACTORY_HH_ */
