#ifndef STEMMERFRENCHQUICK_HH_
# define STEMMERFRENCHQUICK_HH_

# include "Stemmer.hh"
# include <iostream>

namespace Stemmer
{
  class FrenchQuick : public Generic
  {
  public:
    FrenchQuick();
    virtual ~FrenchQuick();
    virtual const std::string getStem(const std::string& word);
  };
}

#endif /* !STEMMERFRENCHQUICK_HH_ */
