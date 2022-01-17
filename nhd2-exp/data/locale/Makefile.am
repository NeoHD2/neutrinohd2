installdir = $(DATADIR)/neutrino/locale

install_DATA = \
	bayrisch.locale \
	bosanski.locale \
	ch-baslerdeutsch.locale \
	ch-berndeutsch.locale \
	deutsch.locale \
	dutch.locale \
	english.locale \
	hungarian.locale \
	francais.locale \
	italiano.locale \
	nederlands.locale \
	polski.locale \
	portugues.locale \
	russkij.locale \
	suomi.locale \
	svenska.locale \
	arabic.locale

###	
LANGS = ru de

LANGMO = $(LANGS:=.mo)
LANGPO = $(LANGS:=.po)

EXTRA_DIST = $(LANGPO)

.po.mo:
	$(MSGFMT) -o $@ $<

BUILT_SOURCES = $(LANGMO)
CLEANFILES = $(LANGMO)

dist-hook: $(LANGPO)

install-data-local: $(LANGMO)
	for lang in $(LANGS); do \
		$(mkinstalldirs) $(DESTDIR)$(DATADIR)/neutrino/locale/$$lang/LC_MESSAGES; \
		$(INSTALL_DATA) $$lang.mo $(DESTDIR)$(DATADIR)/neutrino/locale/$$lang/LC_MESSAGES/@PACKAGE_NAME@.mo; \
	done

uninstall-local:
	for lang in $(LANGS); do \
		$(RM) $(DESTDIR)$(DATADIR)/neutrino/locale/@PACKAGE_NAME@-$$lang.po; \
		$(RM) $(DESTDIR)$(DATADIR)/neutrino/locale/$$lang/LC_MESSAGES/@PACKAGE_NAME@.mo; \
	done
	
	
