/*!
** Construct the singleton.
*/
template<typename T>
inline
Singleton<T>::Singleton()
{

}

/*!
** Destruct the singleton.
*/
template<typename T>
inline
Singleton<T>::~Singleton()
{
}

/*!
** Get the instance of the singleton.
**
** @return Instance of the object
*/
template<typename T>
inline T&
Singleton<T>::getInstance()
{
  static T object;
  return object;
}
