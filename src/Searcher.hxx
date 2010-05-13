namespace Search
{
  /*!
  ** Clean all search query.
  */
  inline void
  Searcher::clean()
  {
    _docFound.clear();
  }

  /*!
  ** Get the document list of all items found.
  **
  ** @return All document found
  */
  inline const Searcher::array&
  Searcher::getDocumentList() const
  {
    return _docFound;
  }

  /*!
  ** Evaluate recursively all node of the AST, getting all needed information.
  **
  ** @param i The iterator of the AST
  */
  inline void
  Searcher::evaluateRequest(iter_t const& i)
  {
    Index::Database& db = Index::Database::getInstance();

    // Unary operand like "+" or "-"
    if (i->value.id() == spirit::parser_id(Request::NodeId::factorID))
    {
      if (*i->value.begin() == '-')
      {
	
      }
      if (*i->value.begin() == '+')
      {
	
      }

      evaluateRequest(i->children.begin());
      return;
    }

    // Normal string expression
    if (i->value.id() == spirit::parser_id(Request::NodeId::string_exprID))
    {
      std::string str(i->value.begin(), i->value.end());
      _docFound = db.getCompleteDocuments(" real_term = '" + str + "'");

      return;
    }

    // Escaped string expression with ""
    if (i->value.id() == spirit::parser_id(Request::NodeId::escaped_stringID))
    {
      // FIXME : delete first and last char (ie the ")
      std::string str(i->value.begin(), i->value.end());

      return;
    }

    // Or operator, ie "|" symbol
    if (i->value.id() == spirit::parser_id(Request::NodeId::termID))
    {
      if (*i->value.begin() == '|')
      {
	evaluateRequest(i->children.begin());
	array tmp = _docFound;
	evaluateRequest(i->children.begin() + 1);
	ArrayUtils::arrayMerge(_docFound, tmp);
	return;
      }
      assert(false);
    }

    // And operator, ie "&" symbol, or " " ("and" is taken by default)
    if (i->value.id() == spirit::parser_id(Request::NodeId::expressionID))
    {
      if (*i->value.begin() == '&' || *i->value.begin() == ' ')
      {
	evaluateRequest(i->children.begin());
	array tmp = _docFound;
	evaluateRequest(i->children.begin() + 1);
	ArrayUtils::arrayFilter(_docFound, tmp);
	return;
      }
      assert(false);
    }

    // Handle simple date like ":date(xx/xx/xx)"
    if (i->value.id() == spirit::parser_id(Request::NodeId::dateID))
    {
      std::string str(i->value.begin(), i->value.end());

      return;
    }

    // Handle complex date like ":date(>xx/xx/xx)" or ":date(xx/xx/xx-xx/xx/xx)"
    if (i->value.id() == spirit::parser_id(Request::NodeId::date_exprID))
    {
      std::string str(i->value.begin(), i->value.end());

      return;
    }

    assert(false);
  }
}
