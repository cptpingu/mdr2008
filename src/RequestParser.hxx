#ifndef REQUESTPARSER_HXX_
# define REQUESTPARSER_HXX_

namespace Search
{
  namespace Request
  {
    /*!
    ** Evaluate recursively all node of the AST, getting all needed information.
    **
    ** @param i The iterator of the AST
    */
    inline void
    Parser::eval_expression(iter_t const& i)
    {
      if (i->value.id() == spirit::parser_id(NodeId::factorID))
      {
	_buf << " " << *i->value.begin() << "(";
	eval_expression(i->children.begin());
	_buf << ")";
	return;
      }
      if (i->value.id() == spirit::parser_id(NodeId::string_exprID))
      {
	std::string str(i->value.begin(), i->value.end());
	_buf << str;
	return;
      }
      if (i->value.id() == spirit::parser_id(NodeId::escaped_stringID))
      {
	std::string str(i->value.begin(), i->value.end());
	_buf << str;
	return;
      }
      if (i->value.id() == spirit::parser_id(NodeId::termID))
      {
	if (*i->value.begin() == '|')
	{
	  _buf << "(";
	  eval_expression(i->children.begin());
	  _buf << " | ";
	  eval_expression(i->children.begin() + 1);
	  _buf << ")";
	  return;
	}
	assert(false);
      }
      if (i->value.id() == spirit::parser_id(NodeId::expressionID))
      {
	if (*i->value.begin() == '&' || *i->value.begin() == ' ')
	{
	  _buf << "(";
	  eval_expression(i->children.begin());
	  _buf << " & ";
	  eval_expression(i->children.begin() + 1);
	  _buf << ")";
	  return;
	}
	assert(false);
      }
      if (i->value.id() == spirit::parser_id(NodeId::dateID))
      {
	_buf << ":date(";
	std::string str(i->value.begin(), i->value.end());
	_buf << str;
	_buf << ")";
	return;
      }
      if (i->value.id() == spirit::parser_id(NodeId::date_exprID))
      {
	_buf << " :date(";
	std::string str(i->value.begin(), i->value.end());
	_buf << str;
	_buf << ")";
	return;
      }

      assert(false);
    }

    /*!
    ** Evaluate an expression, launching the recursive eval_expression method.
    **
    ** @param info The AST tree to browse
    **
    ** @return The result string query, perfectly cleaned and desambiguised.
    */
    inline const std::string
    Parser::evaluate(const spirit::tree_parse_info<>& info)
    {
      _buf.str("");
      _buf.clear();
      eval_expression(info.trees.begin());
      return _buf.str();
    }

    /*!
    ** Get the syntax tree obtain from boost spirit.
    **
    ** @return The syntax tree.
    */
    inline iter_t const
    Parser::getTree() const
    {
      return _info.trees.begin();
    }
  }
}

#endif /* !REQUESTPARSER_HXX_ */
