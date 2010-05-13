namespace Index
{
  /*!
  ** Set verbose on / off.
  **
  ** @param activate If we activate the verbose mode
  */
  inline void
  Indexer::setVerbose(const bool activate)
  {
    _verbose = activate;
  }

  /*!
  ** Clean the black list, deleting all items.
  */
  inline void
  Indexer::cleanBlackList() const
  {
    for (iter i = _blackList.begin(); i != _blackList.end(); ++i)
      delete *i;
    _blackList.clear();
  }

  /*!
  ** Load the black list into a list.
  */
  inline void
  Indexer::loadBlackList() const
  {
    Index::Database& db = Index::Database::getInstance();
    db.fillList<reglist, regexp>(_blackList, "BlackList");
  }

  /*!
  ** Load all stop words from a file into a list.
  */
  inline void
  Indexer::loadStopWords(const std::string& filename) const
  {
    _stopWords.clear();
    std::stringstream buffer;
    std::ifstream file(filename.c_str());
    assert(file);
    buffer << file.rdbuf();
    file.close();
    std::string buf = Utils::renarrow(buffer.str());

    tokenizer tokens(buf);
    for (tokenizer::iterator tok_iter = tokens.begin();
 	 tok_iter != tokens.end(); ++tok_iter)
      _stopWords.push_back(*tok_iter);
  }

  /*!
  ** Load all stop words from a file into a list.
  */
  inline void
  Indexer::loadStopWords(const fs::path& filename) const
  {
    loadStopWords(filename.native_file_string());
  }

  /*!
  ** Clean the white list, deleting all items.
  */
  inline void
  Indexer::cleanWhiteList() const
  {
    for (iter i = _whiteList.begin(); i != _whiteList.end(); ++i)
      delete *i;
    _whiteList.clear();
  }

  /*!
  ** Load the white list into a list.
  */
  inline void
  Indexer::loadWhiteList() const
  {
    Index::Database& db = Index::Database::getInstance();
    db.fillList<reglist, regexp>(_whiteList, "WhiteList");
  }

  /*!
  ** Check if a document is black-listed
  **
  ** @param filename The file to check
  **
  ** @return If the given file is black-listed
  */
  inline bool
  Indexer::isBlackListed(const std::string& filename) const
  {
    for (citer i = _blackList.begin(); i != _blackList.end(); ++i)
      if (boost::regex_match(filename.c_str(), **i))
	return true;

    return false;
  }

  /*!
  ** Check if a document is white-listed
  **
  ** @param filename The file to check
  **
  ** @return If the given file is white-listed
  */
  inline bool
  Indexer::isWhiteListed(const std::string& filename) const
  {
    for (citer i = _whiteList.begin(); i != _whiteList.end(); ++i)
      if (boost::regex_match(filename.c_str(), **i))
	return true;

    return false;
  }

  /*!
  ** Check if a word is a stop word
  **
  ** @param word The word to check
  **
  ** @return If the given file is a stop word
  */
  inline bool
  Indexer::isStopWord(const std::string& word) const
  {
    return std::find(_stopWords.begin(), _stopWords.end(), word) != _stopWords.end();
  }

  /*!
  ** Escape all regex special char
  **
  ** @param expr The expr to escape
  */
  inline std::string
  Indexer::preg_quote (const std::string& expr) const
  {
    return boost::regex_replace (expr, QUOTE_PATTERN, "\\\\$0");
  }

  /*!
  ** Extract an expression between two given Symbol.
  ** The expression will be deleted.
  **
  ** @param text The text where the expression and symbols are
  ** @param expression The regular expression to extract
  **
  ** @return All expression matched concatenated
  */
  inline std::string
  Indexer::extractFromExpression(std::string& text,
				 const std::string& expression,
				 const unsigned int capture) const
  {
    const regexp pattern(expression, regexp::icase | regexp::perl);
    boost::smatch matches;
    std::string res;
    while (boost::regex_search(text, matches, pattern))
    {
      res += " " + matches[capture];
      deleteExpr(text, preg_quote(matches[0]), boost::format_first_only);
    }

    return res;
  }

  /*!
  ** Extract an expression between two given Symbol.
  ** The expression will be deleted.
  **
  ** @param text The text where the expression and symbols are
  ** @param beginSymbol The left symbol
  ** @param expression The regular expression to extract
  ** @param endSymbol The right symbol
  **
  ** @return All expression matched concatenated
  */
  inline std::string
  Indexer::extractFromTwoSymbols(std::string& text,
				 const std::string& beginSymbol,
				 const std::string& expression,
				 const std::string& endSymbol) const
  {
    return extractFromExpression(text, beginSymbol + "(" +
				 expression + ")" + endSymbol, 1);
  }

  /*!
  ** Delete an expression within the given text.
  **
  ** @param text The text to modify
  ** @param expr The expression to suppress
  ** @param flags The type of suppression to perform
  */
  inline void
  Indexer::deleteExpr(std::string& text,
		      const std::string& expr,
		      const boost::match_flag_type flags) const
  {
    const regexp pattern(expr, regexp::icase | regexp::perl);
    text = boost::regex_replace(text, pattern, "", flags);
  }

  /*!
  ** Get the string inside the given balises, then erase matching text.
  **
  ** @param text The given text (it will be modified !)
  ** @param balise The balise where to extract text.
  **
  ** @return The extracted string
  */
  inline std::string
  Indexer::extractFromBalises(std::string& text, const std::string& balise) const
  {
    return extractFromExpression(text,
				 "<\\s*(" + balise + ")[^>]*>\\s*"
				 "([^>]*)"
				 "\\s*<\\s*/\\s*\\1\\s*>", 2);
  }

  /*!
  ** Replace all images by theirs alt property values.
  **
  ** @param text The text to modify
  */
  inline void
  Indexer::replaceImgWithAlt(std::string& text) const
  {
    const regexp pattern(IMG_TO_ALT, regexp::icase | regexp::perl);
    boost::smatch matches;
    while (boost::regex_search(text, matches, pattern))
    {
      const regexp tmp(preg_quote(matches[0]), regexp::icase | regexp::perl);
      text = boost::regex_replace(text, tmp, std::string(matches[1]), boost::format_first_only);
    }
  }

  /*!
  ** Replace all special HTML characters in the given text.
  **
  ** @param text The text to modify.
  */
  inline void
  Indexer::replaceSpecialHTMLChar(std::string& text) const
  {
    static const std::pair<std::string, std::string> tab[] = {
      std::make_pair("&quot;", "\""), std::make_pair("&oelig;", "oe"),
      std::make_pair("&Yuml;", "y"), std::make_pair("&iexcl;", "i"),
      std::make_pair("&Agrave;", "a"), std::make_pair("&agrave;", "a"),
      std::make_pair("&Aacute;", "à"), std::make_pair("&aacute;", "à"),
      std::make_pair("&Acirc;", "â"), std::make_pair("&acirc;", "â"),
      std::make_pair("&Aelig", "ae"), std::make_pair("&aelig", "ae"),
      std::make_pair("&Ccedil;", "ç"), std::make_pair("&ccedil;", "ç"),
      std::make_pair("&Egrave;", "è"), std::make_pair("&egrave;", "è"),
      std::make_pair("&Eacute;", "é"), std::make_pair("&eacute;", "é"),
      std::make_pair("&Ecirc;", "ê"), std::make_pair("&ecirc;", "ê"),
      std::make_pair("&Euml;", "ë"), std::make_pair("&euml;", "ë"),
      std::make_pair("&Icirc;", "î"), std::make_pair("&icirc;", "î"),
      std::make_pair("&Iuml;", "ï"), std::make_pair("&Iuml;", "ï"),
      std::make_pair("&Ugrave;", "ù"), std::make_pair("&ugrave;", "ù"),
      std::make_pair("&Ucirc;", "û"), std::make_pair("&ucirc;", "û"),
      std::make_pair("&nbsp;", " "), std::make_pair("&[^&;]*;", ""),
      std::make_pair("", "")};

    for (unsigned int i = 0; tab[i].first != ""; i++)
    {
      const regexp pattern(tab[i].first, regexp::icase | regexp::perl);
      text = boost::regex_replace(text, pattern, tab[i].second);
    }
  }
}
