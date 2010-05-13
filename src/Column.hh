#ifndef COLUMN_HH_
# define COLUMN_HH_

# include <iostream>

namespace Index
{
  namespace File
  {
    enum type
      {
	TEXT = 1,
	HTML = 2
      };
  }

  namespace Column
  {
    struct Result
    {
      unsigned int	idSearch;
      unsigned int	idDoc;
      double		rank;
    };

    struct Document
    {
      unsigned int	id;
      std::string	filename;
      File::type	type;
      std::string	hash;
      std::string	date;
      unsigned int	length;
    };

    struct Word
    {
      unsigned int	idDocument;
      unsigned int	idTerm;
      double		weight;
      unsigned int	realCount;
      unsigned int	stemCount;
      double		score;
    };

    struct Term
    {
      unsigned int	id;
      std::string	realTerm;
      std::string	stemTerm;
    };

    struct DocumentWord : public Document, public Word
    {
    };

    struct WordTerm : public Word, public Term
    {
    };

    struct DocumentWordTerm : public Document, public Word, public Term
    {
    };

    struct DocumentResult : public Document, public Result
    {
      bool operator<(const Index::Column::DocumentResult& docRes);
      bool operator==(const Index::Column::DocumentResult& docRes);
    };

    static bool docExists(const Column::Document& doc);
    static bool wordExists(const Column::Word& word);
    static bool termExists(const Column::Term& term);
  }
}

std::ostream& operator<<(std::ostream& o, const Index::Column::Document& doc);
std::ostream& operator<<(std::ostream& o, const Index::Column::Word& word);
std::ostream& operator<<(std::ostream& o, const Index::Column::Term& term);
std::ostream& operator<<(std::ostream& o, const Index::Column::Result& res);
std::ostream& operator<<(std::ostream& o, const Index::Column::DocumentWord& documentword);
std::ostream& operator<<(std::ostream& o, const Index::Column::WordTerm& wordterm);
std::ostream& operator<<(std::ostream& o, const Index::Column::DocumentWordTerm& documentwordterm);
std::ostream& operator<<(std::ostream& o, const Index::Column::DocumentResult& docRes);

# include "Column.hxx"

#endif /* !COLUMN_HH_ */
