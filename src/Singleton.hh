#ifndef SINGLETON_HH_
# define SINGLETON_HH_

template<typename T>
class Singleton
{
protected:
  Singleton();
  virtual ~Singleton();

public:
  static T&	getInstance();
};

# include "Singleton.hxx"

#endif /* !SINGLETON_HH_ */
