#ifndef PARSEEXCEPTION_HH_
# define PARSEEXCEPTION_HH_

namespace Search
{
  namespace Request
  {
    class ParseException
    {
    public:
      /*!
      ** Construct an exception using a C style string.
      **
      ** @param msg The C style string
      */
      ParseException(const char* msg) throw()
	: _msg(msg)
      {
      }

      /*!
      ** Construct an exception using a C++ style string.
      **
      ** @param msg The C++ style string
      */
      ParseException(const std::string& msg) throw()
	: _msg(msg)
      {
      }

      /*!
      ** Destruct this exception.
      */
      virtual ~ParseException() throw()
      {
      }

      /*!
      ** Get the error message.
      **
      ** @return A C style string error message
      */
      virtual const char* what() const throw()
      {
	return _msg.c_str();
      }

    private:
      const std::string		_msg;
    };
  }
}

#endif /* !PARSEEXCEPTION_HH_ */
