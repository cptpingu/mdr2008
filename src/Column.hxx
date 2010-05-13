namespace Index
{
  namespace Column
  {
    /*!
    ** Check that the given document exists in database.
    **
    ** @param doc The document
    **
    ** @return If given document exists
    */
    inline bool
    docExists(const Column::Document& doc)
    {
      return doc.id != 0;
    }

    /*!
    ** Check that the given word exists in database.
    **
    ** @param word The word
    **
    ** @return If given word exists
    */
    inline bool
    wordExists(const Column::Word& word)
    {
      return word.idTerm != 0 || word.idDocument != 0;
    }

    /*!
    ** Check that the given term exists in database.
    **
    ** @param term The term
    **
    ** @return If given term exists
    */
    inline bool
    termExists(const Column::Term& term)
    {
      return term.id != 0;
    }
  }
}
