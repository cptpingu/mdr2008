namespace Index
{
  /*!
  ** Begin a transaction
  */
  inline void
  Database::beginTransaction()
  {
    _db.execDML("begin transaction;");
  }

  /*!
  ** End a transaction
  */
  inline void
  Database::endTransaction()
  {
    _db.execDML("commit transaction;");
  }

  /*!
  ** Get a document using a request.
  **
  ** @param request The request to execute
  **
  ** @return A document filled with all information, else an id doc equal to 0
  */
  inline const Column::Document
  Database::getDocument(const std::string& request)
  {
    SQLite::Query q = _db.execQuery(request.c_str());
    Column::Document doc = {0, "", File::TEXT, "", "", 0};
    if (!q.eof())
    {
      for (int fld = 0; fld < q.numFields(); fld++)
      {
	if (std::string(q.fieldName(fld)) == "id_doc")
	  doc.id = Utils::stringToInt(q.fieldValue(fld));
	else
	  if (std::string(q.fieldName(fld)) == "filename")
	    doc.filename = q.fieldValue(fld);
	  else
	    if (std::string(q.fieldName(fld)) == "type")
	    {
	      unsigned int t = Utils::stringToInt(q.fieldValue(fld));
	      doc.type = t == File::HTML ? File::HTML : File::TEXT;
	    }
	    else
	      if (std::string(q.fieldName(fld)) == "hash")
		doc.hash = q.fieldValue(fld);
	      else
		if (std::string(q.fieldName(fld)) == "date")
		  doc.date = q.fieldValue(fld);
		else
		  if (std::string(q.fieldName(fld)) == "length")
		    doc.length = Utils::stringToInt(q.fieldValue(fld));
		  else
		    assert(false);
      }
      q.nextRow();
    }
    assert(q.eof());

    return doc;
  }

  /*!
  ** Get a word using a request.
  **
  ** @param request The request to execute
  **
  ** @return A word filled with all information, else an id_doc and an idTerm equal to 0
  */
  inline const Column::Word
  Database::getWord(const std::string& request)
  {
    SQLite::Query q = _db.execQuery(request.c_str());
    Column::Word word = {0, 0, 0, 0, 0, 0.0};
    if (!q.eof())
    {
      for (int fld = 0; fld < q.numFields(); fld++)
      {
	if (std::string(q.fieldName(fld)) == "id_doc")
	  word.idDocument = Utils::stringToInt(q.fieldValue(fld));
	else
	  if (std::string(q.fieldName(fld)) == "id_term")
	    word.idTerm = Utils::stringToInt(q.fieldValue(fld));
	  else
	    if (std::string(q.fieldName(fld)) == "weight")
	      word.weight = Utils::stringToDouble(q.fieldValue(fld));
	    else
	      if (std::string(q.fieldName(fld)) == "real_count")
		word.realCount = Utils::stringToInt(q.fieldValue(fld));
	      else
		if (std::string(q.fieldName(fld)) == "stem_count")
		  word.stemCount = Utils::stringToInt(q.fieldValue(fld));
		else
		  if (std::string(q.fieldName(fld)) == "score")
		    word.score = Utils::stringToDouble(q.fieldValue(fld));
		  else
		    assert(false);
      }
      q.nextRow();
    }
    assert(q.eof());

    return word;
  }

  /*!
  ** Get a document word using a request.
  **
  ** @param request The request to execute
  **
  ** @return A document filled with all information, else an id doc equal to 0
  */
  inline const std::list<Column::DocumentWordTerm>
  Database::getDocumentWords(const std::string& request)
  {
    SQLite::Query q = _db.execQuery(request.c_str());
    std::list<Column::DocumentWordTerm> docList;

    while (!q.eof())
    {
      Column::DocumentWordTerm doc;
      for (int fld = 0; fld < q.numFields(); fld++)
      {

	if (std::string(q.fieldName(fld)) == "id_doc")
	  doc.Column::Document::id = Utils::stringToInt(q.fieldValue(fld));
	else if (std::string(q.fieldName(fld)) == "filename")
	  doc.filename = q.fieldValue(fld);
	else if (std::string(q.fieldName(fld)) == "type")
	{
	  unsigned int t = Utils::stringToInt(q.fieldValue(fld));
	  doc.type = t == File::HTML ? File::HTML : File::TEXT;
	}
	else if (std::string(q.fieldName(fld)) == "hash")
	  doc.hash = q.fieldValue(fld);
	else if (std::string(q.fieldName(fld)) == "date")
	  doc.date = q.fieldValue(fld);
	else if (std::string(q.fieldName(fld)) == "length")
	  doc.length = Utils::stringToInt(q.fieldValue(fld));
	else if (std::string(q.fieldName(fld)) == "id_term")
	  doc.idTerm = Utils::stringToInt(q.fieldValue(fld));
	else if (std::string(q.fieldName(fld)) == "weight")
	  doc.weight = Utils::stringToDouble(q.fieldValue(fld));
	else if (std::string(q.fieldName(fld)) == "real_count")
	  doc.realCount = Utils::stringToInt(q.fieldValue(fld));
	else if (std::string(q.fieldName(fld)) == "stem_count")
	  doc.stemCount = Utils::stringToInt(q.fieldValue(fld));
	else if (std::string(q.fieldName(fld)) == "score")
	  doc.score = Utils::stringToDouble(q.fieldValue(fld));
	else if (std::string(q.fieldName(fld)) == "real_term")
	  doc.realTerm = q.fieldValue(fld);
	else if (std::string(q.fieldName(fld)) == "stem_term")
	  doc.stemTerm = q.fieldValue(fld);
	else
	  assert(false);
      }
      docList.push_back(doc);
      q.nextRow();
    }
    assert(q.eof());

    return docList;
  }

  /*!
  ** Get a document result using a request.
  **
  ** @param request The request to execute
  **
  ** @return A document filled with all information, else an id doc equal to 0
  */
  inline const std::list<Column::DocumentResult>
  Database::getDocumentResults(const std::string& request)
  {
    SQLite::Query q = _db.execQuery(request.c_str());
    std::list<Column::DocumentResult> docList;

    Column::DocumentResult doc;
    while (!q.eof())
    {
      for (int fld = 0; fld < q.numFields(); fld++)
      {
	if (std::string(q.fieldName(fld)) == "id_doc")
	{
	  doc.Column::Document::id = Utils::stringToInt(q.fieldValue(fld));
	  doc.idDoc = Utils::stringToInt(q.fieldValue(fld));
	}
	else if (std::string(q.fieldName(fld)) == "filename")
	  doc.filename = q.fieldValue(fld);
	else if (std::string(q.fieldName(fld)) == "type")
	{
	  unsigned int t = Utils::stringToInt(q.fieldValue(fld));
	  doc.type = t == File::HTML ? File::HTML : File::TEXT;
	}
	else if (std::string(q.fieldName(fld)) == "hash")
	  doc.hash = q.fieldValue(fld);
	else if (std::string(q.fieldName(fld)) == "date")
	  doc.date = q.fieldValue(fld);
	else if (std::string(q.fieldName(fld)) == "length")
	  doc.length = Utils::stringToInt(q.fieldValue(fld));
	else if (std::string(q.fieldName(fld)) == "id_search")
	  doc.idSearch = Utils::stringToInt(q.fieldValue(fld));
	else if (std::string(q.fieldName(fld)) == "rank")
	  doc.rank = Utils::stringToDouble(q.fieldValue(fld));
      }
      docList.push_back(doc);
      q.nextRow();
    }
    assert(q.eof());

    return docList;
  }

  /*!
  ** Get a term using a request.
  **
  ** @param request The request to execute
  **
  ** @return A term filled with all information, else an id term equal to 0
  */
  inline const Column::Term
  Database::getTerm(const std::string& request)
  {
    SQLite::Query q = _db.execQuery(request.c_str());
    Column::Term term = {0, "",""};
    if (!q.eof())
    {
      for (int fld = 0; fld < q.numFields(); fld++)
      {
	if (std::string(q.fieldName(fld)) == "id_term")
	  term.id = Utils::stringToInt(q.fieldValue(fld));
	else
	  if (std::string(q.fieldName(fld)) == "real_term")
	    term.realTerm = q.fieldValue(fld);
	  else
	    if (std::string(q.fieldName(fld)) == "stem_term")
	      term.stemTerm = q.fieldValue(fld);
	    else
	      assert(false);
      }
      q.nextRow();
    }
    assert(q.eof());

    return term;
  }

  /*!
  ** Update all score for all word in a document, adjusting
  ** it with the given document length.
  **
  ** @param doc The document where the words are
  */
  inline void
  Database::updateTermScore(const Column::Document& doc)
  {
    std::ostringstream cmd;
    cmd << "UPDATE Word SET " <<
      "score = 100 * (score / " << doc.length <<
      ") WHERE id_doc = " << doc.id << ";";

    _db.execDML(cmd.str().c_str());
  }

  /*!
  ** Fill the black list.
  **
  ** @param list The list to fill
  */
  template <typename Collection, typename Type>
  inline void
  Database::fillList(Collection& list, const std::string& table)
  {
    const std::string cmd = "SELECT expression FROM " + table + ";";
    SQLite::Query q = _db.execQuery(cmd.c_str());
    while (!q.eof())
    {
      for (int fld = 0; fld < q.numFields(); fld++)
	if (std::string(q.fieldName(fld)) == "expression")
	  list.push_back(new Type(q.fieldValue(fld)));
      q.nextRow();
    }
  }

  /*!
  ** Check if similar request already done before.
  **
  ** @param query The query to check
  **
  ** @return If a similar request was found
  */
  inline unsigned int
  Database::getSimilarRequest(const std::string& query)
  {
    const std::string cmd = "SELECT id_search FROM Search WHERE"
      " sentence = '" + query + "';";
    SQLite::Query q = _db.execQuery(cmd.c_str());
    unsigned int i = 0;
    if (!q.eof())
    {
      for (int fld = 0; fld < q.numFields(); fld++)
	if (std::string(q.fieldName(fld)) == "id_search")
	  i = Utils::stringToInt(q.fieldValue(fld));
      q.nextRow();
    }
    assert(q.eof());

    return i;
  }

  /*!
  ** Get all doc associated to the given id.
  **
  ** @param id The id of cached search result
  **
  ** @return List of all documents previously cached
  */
  inline const std::list<Column::DocumentResult>
  Database::getCachedSearchResult(const unsigned int id)
  {
    std::stringstream cmd;
    cmd << "SELECT * FROM Search JOIN Result ON Search.id_search = Result.id_search"
      " JOIN Document ON Result.id_doc = Document.id_doc"
      " WHERE Search.id_search = " << id << ";";
    return getDocumentResults(cmd.str());
  }

  /*!
  ** Save the result in database, hence cache the search.
  **
  ** @param res The result to cache
  */
  inline void
  Database::saveResult(const Column::Result& res,
		       const double rank)
  {
    std::stringstream cmd;
    cmd << "INSERT INTO Result (id_search, id_doc, rank) "
      " VALUES(" <<
      res.idSearch << ", " <<
      res.idDoc << ", " <<
      rank << "); ";
    _db.execDML(cmd.str().c_str());
  }

  /*!
  ** Save the results in database, hence cache all search.
  **
  ** @param list The list of found document
  */
  inline void
  Database::saveResults(const std::list<Column::DocumentResult>& list,
			const std::string& sentence)
  {
    // Save the search
    std::stringstream cmd;
    cmd << "INSERT INTO Search(id_search, sentence) "
      "VALUES(NULL, '" << sentence << "');";
    _db.execDML(cmd.str().c_str());

    // Save all results attached to this search
    unsigned int id = getSimilarRequest(sentence);
    Column::Result res;
    res.idSearch = id;
    beginTransaction();
    for (std::list<Column::DocumentResult>::const_iterator iter = list.begin();
	 iter != list.end(); ++iter)
    {
      res.idDoc = iter->Column::Document::id;
      saveResult(res, iter->rank);
    }
    endTransaction();
  }

  /*!
  ** Clear all search cache, deleting all data in Search and Result tables.
  */
  inline void
  Database::clearSearchCache()
  {
    _db.execDML("DELETE FROM Search;");
    _db.execDML("DELETE FROM Result;");
  }
}
