#include "ArrayUtils.hh"

/*!
** Merge to array, avoiding redundancy
**
** @param dst Destination array
** @param src Array to merge
*/
void
ArrayUtils::arrayMerge(array& dst, const array& src)
{
  for (citer i = src.begin(); i != src.end(); ++i)
    dst.push_back(*i);
  dst.unique();
}

/*!
** Filter the array, deleting all entry in dst from src.
**
** @param dst Array to filter
** @param src The filter
*/
void
ArrayUtils::arrayFilter(array& dst, const array&)
{
  for (citer i = dst.begin(); i != dst.end(); ++i)
  {
    // FIXME
    //     if (src.find(*i))
    //       dst.erase(*i);
  }
}
