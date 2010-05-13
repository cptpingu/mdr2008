#ifndef STEMMER_HH_
# define STEMMER_HH_

# include <iostream>

namespace Stemmer
{
  class Generic
  {
  public:
    Generic();
    virtual ~Generic();
    virtual const std::string getStem(const std::string& word) = 0;
  };
}

#endif /* !STEMMER_HH_ */
