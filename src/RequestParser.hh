#ifndef REQUESTPARSER_HH_
# define REQUESTPARSER_HH_

# include <boost/spirit/utility/chset.hpp>
# include <boost/spirit/core.hpp>
# include <boost/spirit/utility/confix.hpp>
# include <boost/spirit/utility/escape_char.hpp>
# include <boost/spirit/tree/ast.hpp>
# include <boost/spirit/tree/parse_tree.hpp>
# include <boost/spirit/tree/parse_tree_utils.hpp>
# include <boost/spirit/tree/tree_to_xml.hpp>
# include <iostream>
# include <sstream>
# include <string>
# include "ParseException.hh"

namespace spirit = boost::spirit;

typedef char const*                              iterator_t;
typedef spirit::tree_match<iterator_t>           parse_tree_match_t;
typedef parse_tree_match_t::const_tree_iterator  iter_t;

namespace Search
{
  namespace Request
  {
    namespace NodeId
    {
      static const int factorID = 1;
      static const int termID = 2;
      static const int expressionID = 3;
      static const int dateID = 4;
      static const int date_exprID = 5;
      static const int string_exprID = 6;
      static const int escaped_stringID = 7;
    }

    class Parser
    {
    public:
      Parser(const std::string& query);
      ~Parser();
      void parseQuery();
      const std::string toXML() const;
      const std::string toString();
      iter_t const getTree() const;

    private:
      const std::string evaluate(const spirit::tree_parse_info<>& info);
      void eval_expression(iter_t const& i);


    private:
      std::string			_query;
      spirit::tree_parse_info<>		_info;
      std::ostringstream		_buf;

    private:
      struct calculator : public spirit::grammar<calculator>
      {
	template <typename ScannerT>
	struct definition
	{
	  definition(calculator const& /*self*/)
	  {
	    using spirit::space_p;
	    using spirit::as_lower_d;
	    using spirit::ch_p;
	    using spirit::confix_p;
	    using spirit::digit_p;
	    using spirit::lexeme_d;
	    using spirit::alnum_p;
	    using spirit::c_escape_ch_p;
	    using spirit::root_node_d;
	    using spirit::leaf_node_d;
	    using spirit::inner_node_d;
	    using spirit::discard_node_d;
	    using spirit::nothing_p;
	    using spirit::anychar_p;
	    using spirit::chseq_p;
	    using spirit::graph_p;
	    using spirit::print_p;

	    expression
	      = term >> *(
			  (
			   discard_node_d[*space_p] >> root_node_d[ch_p('&')]
			   | root_node_d[space_p]
			   )
			  >> discard_node_d[*space_p]
			  >> term
			  )
	      ;

	    term
	      = factor >> *(
			    discard_node_d[*space_p]
			    >> root_node_d[ch_p('|')]
			    >> discard_node_d[*space_p]
			    >> factor
			    )
	      ;

	    date
	      = leaf_node_d[
			    digit_p >> !digit_p >> '/' >> digit_p >> !digit_p >> '/' >>
			    digit_p >> digit_p >> !(digit_p >> digit_p)
			    | "now"
			    | "tomorrow"
			    | "yesterday"
			    ]
	      ;

	    date_expr
	      = leaf_node_d[
			    discard_node_d[
					   *space_p >> as_lower_d[":date"]
					   >> *space_p >> '(' >> *space_p
					   ]
			    >> (date
				>> discard_node_d[*space_p]
				>> !('-' >> discard_node_d[*space_p] >> date)
				| '<' >> discard_node_d[*space_p] >> date
				| '>' >> discard_node_d[*space_p] >> date
				)
			    >> discard_node_d[*space_p >> ')']
			    ]
	      ;

	    string_expr = leaf_node_d[(+(~space_p
#ifndef EAT_TOKEN_TO_AVOID_ERROR
					 & ~ch_p('+')
 					 & ~ch_p('-')
// 					 & ~ch_p('(')
// 					 & ~ch_p(')')
#endif
					 & ~ch_p('&')
					 & ~ch_p('|')
					 ))];
	    escaped_string = leaf_node_d[confix_p('"', *c_escape_ch_p, '"')];

	    factor
	      = string_expr
	      | escaped_string
	      | inner_node_d[
			     '(' >> discard_node_d[*space_p]
			     >> expression
			     >> discard_node_d[*space_p] >> ')'
			     ]
	      | (root_node_d[ch_p('-') | ch_p('+')]
		 >> discard_node_d[*space_p] >> factor)
	      | date_expr
	      ;
 	  }

	  spirit::rule<ScannerT, spirit::parser_context<>, spirit::parser_tag<NodeId::expressionID> >		expression;
	  spirit::rule<ScannerT, spirit::parser_context<>, spirit::parser_tag<NodeId::termID> >			term;
	  spirit::rule<ScannerT, spirit::parser_context<>, spirit::parser_tag<NodeId::factorID> >		factor;
	  spirit::rule<ScannerT, spirit::parser_context<>, spirit::parser_tag<NodeId::date_exprID> >		date_expr;
	  spirit::rule<ScannerT, spirit::parser_context<>, spirit::parser_tag<NodeId::dateID> >			date;
	  spirit::rule<ScannerT, spirit::parser_context<>, spirit::parser_tag<NodeId::string_exprID> >		string_expr;
	  spirit::rule<ScannerT, spirit::parser_context<>, spirit::parser_tag<NodeId::escaped_stringID> >	escaped_string;

	  spirit::rule<ScannerT, spirit::parser_context<>, spirit::parser_tag<NodeId::expressionID> > const&
	  start() const
	  {
	    return expression;
	  }
	};
      };
    };
  }
}

# include "RequestParser.hxx"

#endif /* !REQUESTPARSER_HH_ */
