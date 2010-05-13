#ifndef ARRAYUTILS_HH_
# define ARRAYUTILS_HH_

# include "Column.hh"
# include <list>

class ArrayUtils
{
  typedef std::list< ::Index::Column::DocumentResult> array;
  typedef array::iterator iter;
  typedef array::const_iterator citer;

public:
  static void arrayMerge(array& dst, const array& src);
  static void arrayFilter(array& dst, const array& src);
};

#endif /* !ARRAYUTILS_HH_ */
