include Makefile.rules

all:
	cd src && $(MAKE) && cd ..

clean:
	rm -f *.o *.~ *.core *.Dstore *.log *.ml *.err *\#* *.tmp
	cd src && $(MAKE) clean && cd ..

distclean: clean
	cd src && $(MAKE) distclean && cd ..
	cd doc && rm -rf html latex man refman.pdf && cd ..
	rm -f $(EXE) Makefile.rules Makefile.deps

rights:
	chmod 640 AUTHORS
	chmod 750 configure
	chmod 640 README
	chmod 640 TODO
	chmod 640 ChangeLog
	find . -type d -print -exec chmod 750 {} \;
	find . -name "*.[ch]" -print -exec chmod 640 {} \;
	find . -name "*.cc" -print -exec chmod 640 {} \;
	find . -name "*.hh" -print -exec chmod 640 {} \;
	find . -name "*.sh" -print -exec chmod 750 {} \;
	find . -name "Makefile" -print -exec chmod 750 {} \;

dist: distclean rights
	rm -f *.tar.bz2 && \
	tar pcjvf $(LOGIN)-$(PROJ).tar.bz2 * && \
	chmod 640 $(LOGIN)-$(PROJ).tar.bz2 && \
	rm -Rf $(LOGIN)-$(PROJ) && mkdir $(LOGIN)-$(PROJ) && \
	chmod 750 $(LOGIN)-$(PROJ) && \
	mv $(LOGIN)-$(PROJ).tar.bz2 $(LOGIN)-$(PROJ) && \
	cd $(LOGIN)-$(PROJ) && tar pxjvf $(LOGIN)-$(PROJ).tar.bz2 && \
	rm -f *.tar.bz2 && cd .. && \
	tar pcjvf $(LOGIN)-$(PROJ).tar.bz2  $(LOGIN)-$(PROJ) && \
	chmod 640 $(LOGIN)-$(PROJ).tar.bz2 && \
	rm -Rf  $(LOGIN)-$(PROJ)

tar: dist

distcheck: dist
	tar pxjvf $(LOGIN)-$(PROJ).tar.bz2
	cd $(LOGIN)-$(PROJ) && ./configure && make clean all check; \
	cd .. && rm -rf $(LOGIN)-$(PROJ)

doc:
	doxygen doc/Doxyfile && cd doc/latex && $(MAKE) && cp refman.pdf ..

check:
	bash check/checker.sh

install: all
	cp $(EXE) /bin/

.PHONY: doc check
