include ../Makefile.rules

SRC=	main.cc			\
	Utils.cc		\
	DateUtils.cc		\
	ArrayUtils.cc		\
	Singleton.cc		\
	Sha1.cc			\
	Database.cc		\
	Configuration.cc	\
	Indexer.cc		\
	Searcher.cc		\
	RequestParser.cc	\
	Stemmer.cc		\
	StemmerFrench.cc	\
	StemmerFrenchQuick.cc	\
	Column.cc		\
	SQLite.cc		\
	SQLiteBinary.cc		\
	SQLiteBuffer.cc		\
	SQLiteDB.cc		\
	SQLiteException.cc	\
	SQLiteQuery.cc		\
	SQLiteStatement.cc	\
	SQLiteTable.cc


HEADER=$(SRC:.cc=.hh)
EXTRAHEADER=	Utils.hxx		\
		Column.hxx		\
		Database.hxx		\
		Configuration.hxx	\
		Indexer.hxx		\
		Searcher.hxx		\
		ParseException.hh	\
		RequestParser.hxx	\
		StemmerFactory.hh	\
		StemmerFactory.hxx	\
		StemmerFrench.hxx	\
		Singleton.hxx

TARGET=../$(EXE)

OBJ=$(SRC:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJ) Makefile.deps
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $(TARGET)

Makefile.deps: $(SRC) $(HEADER) $(EXTRAHEADER)
	$(CXX) -MM $(SRC) > Makefile.deps

clean:
	rm -f *.o *.~ *.core *.Dstore *.log *.ml *.err *\#*

distclean:
	rm -f $(EXE)
	rm -f Makefile.deps Makefile.rules

-include Makefile.deps
