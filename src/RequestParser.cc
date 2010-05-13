#include "RequestParser.hh"

namespace Search
{
  namespace
  {
    inline void
    trim_right(std::string &source, const std::string &t)
    {
      source.erase(source.find_last_not_of(t)+1);
    }

    inline void
    trim_left(std::string &source, const std::string &t)
    {
      source.erase(0, source.find_first_not_of(t));
    }

    inline void
    trim(std::string &source, const std::string &t)
    {
      trim_left(source, t);
      trim_right(source, t);
    }
  }

  namespace Request
  {
    /*!
    ** Construct the parser with the given query.
    **
    ** @param query The query to parse
    */
    Parser::Parser(const std::string& query)
    {
      // FIXME : Replace "and" and "or" by "&" and "|", if needed only !
      // Use boost::regex_replace (see the indexer::extractHTMLToken)
      _query = query;
      trim(_query, " ");
    }

    /*!
    ** Destruct the parser.
    */
    Parser::~Parser()
    {
    }

    /*!
    ** Launch the parsing of the query.
    */
    void
    Parser::parseQuery()
    {
      calculator calc;
      _info = ast_parse(_query.c_str(), calc);
      if (!_info.full)
	throw ParseException(std::string(_info.stop));
    }

    /*!
    ** Convert AST query to a special hierarchical XML.
    **
    ** @return A XML string formatted.
    */
    const std::string
    Parser::toXML() const
    {
      std::ostringstream buf;
      std::map<spirit::parser_id, std::string> rule_names;
      rule_names[NodeId::factorID] = "factor";
      rule_names[NodeId::termID] = "term";
      rule_names[NodeId::expressionID] = "expression";
      rule_names[NodeId::dateID] = "date";
      rule_names[NodeId::date_exprID] = "date_expr";
      tree_to_xml(buf, _info.trees, _query.c_str(), rule_names);

      return buf.str();
    }

    /*!
    ** Convert the the query to a clean and minimalist string.
    **
    ** @return The cleaned query
    */
    const std::string
    Parser::toString()
    {
      return evaluate(_info);
    }
  }
}
