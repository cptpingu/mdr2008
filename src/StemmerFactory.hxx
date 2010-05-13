namespace Stemmer
{
  /*!
  ** Instanciate correct stemmer depending on given type.
  **
  ** @param type The type of stemmer to instanciate
  **
  ** @return An instance of correct stemmer
  */
  inline Generic*
  StemmerFactory::get(const std::string& type)
  {
    if (type == "french")
      return new Stemmer::French();

    if (type == "frenchquick")
      return new Stemmer::FrenchQuick();

    assert(false);
    return 0;
  }
}
