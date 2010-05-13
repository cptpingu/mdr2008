#include "Column.hh"

/*!
** Display a term column on wanted stream.
**
** @param o The stream where to display it
** @param doc The doc to display
**
** @return The modified stream
*/
std::ostream&
operator<<(std::ostream& o, const Index::Column::Document& doc)
{
  return o << "Doc.id = " << doc.id << std::endl <<
    "Doc.filename = " << doc.filename << std::endl <<
    "Doc.type = " << doc.type << std::endl <<
    "Doc.hash = " << doc.hash << std::endl <<
    "Doc.date = " << doc.date << std::endl <<
    "Doc.length = " << doc.length << std::endl;
}

/*!
** Display a word column on wanted stream.
**
** @param o The stream where to display it
** @param word The word to display
**
** @return The modified stream
*/
std::ostream&
operator<<(std::ostream& o, const Index::Column::Word& word)
{
  return o << "Word.id_document = " << word.idDocument << std::endl <<
    "Word.id_term = " << word.idTerm << std::endl <<
    "Word.weight = " << word.weight << std::endl <<
    "Word.realCount = " << word.realCount << std::endl <<
    "Word.stemCount = " << word.stemCount << std::endl <<
    "Word.score = " << word.score << std::endl;
}

/*!
** Display a term column on wanted stream.
**
** @param o The stream where to display it
** @param term The term to display
**
** @return The modified stream
*/
std::ostream&
operator<<(std::ostream& o, const Index::Column::Term& term)
{
  return o << "Term.id = " << term.id << std::endl <<
    "Term.realTerm = " << term.realTerm << std::endl <<
    "Term.stemTerm = " << term.stemTerm << std::endl;
}

/*!
** Display a result column on wanted stream.
**
** @param o The stream where to display it
** @param term The term to display
**
** @return The modified stream
*/
std::ostream&
operator<<(std::ostream& o, const Index::Column::Result& res)
{
  return o << "Result.id_search = " << res.idSearch << std::endl <<
    "Result.id_doc = " << res.idDoc << std::endl <<
    "Result.rank = " << res.rank << std::endl;
}

/*!
** Display a documentword column on wanted stream.
**
** @param o The stream where to display it
** @param documentword The documentword to display
**
** @return The modified stream
*/
std::ostream&
operator<<(std::ostream& o, const Index::Column::DocumentWord& documentword)
{
  const Index::Column::Document doc = documentword;
  const Index::Column::Word word = documentword;
  return o << doc << word;
}

/*!
** Display a wordterm column on wanted stream.
**
** @param o The stream where to display it
** @param wordterm The wordterm to display
**
** @return The modified stream
*/
std::ostream&
operator<<(std::ostream& o, const Index::Column::WordTerm& wordterm)
{
  const Index::Column::Word word = wordterm;
  const Index::Column::Term term = wordterm;
  return o << word << term;
}

/*!
** Display a documentwordterm column on wanted stream.
**
** @param o The stream where to display it
** @param documentwordterm The documentwordterm to display
**
** @return The modified stream
*/
std::ostream&
operator<<(std::ostream& o, const Index::Column::DocumentWordTerm& documentwordterm)
{
  const Index::Column::Document doc = documentwordterm;
  const Index::Column::Word word = documentwordterm;
  const Index::Column::Term term = documentwordterm;
  return o << doc << word << term;
}

/*!
** Display a documentresult column on wanted stream.
**
** @param o The stream where to display it
** @param docRes The document result to display
**
** @return The modified stream
*/
std::ostream&
operator<<(std::ostream& o, const Index::Column::DocumentResult& docRes)
{
  const Index::Column::Document doc = docRes;
  const Index::Column::Result res = docRes;
  return o << doc << res;
}

/*!
** Used for sort the document result list.
**
** @param docRes Another document result list.
**
** @return If given document result is lesser than this
*/
bool
Index::Column::DocumentResult::operator<(const Index::Column::DocumentResult& docRes)
{
  return docRes.rank < this->rank;
}

/*!
** Used for apply unique on a result list.
**
** @param docRes Another document result list.
**
** @return If given document result is equal to our
*/
bool
Index::Column::DocumentResult::operator==(const Index::Column::DocumentResult& docRes)
{
  return docRes.filename == this->filename;
}
