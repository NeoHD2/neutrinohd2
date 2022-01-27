LANGS = en de

LANGMO = $(LANGS:=.mo)
LANGPO = $(LANGS:=.po)

EXTRA_DIST = $(LANGPO)

neutrino-hd2.pot:
	$(XGETTEXT) --default-domain=@PACKAGE_NAME@ --directory=$(top_srcdir)/src --keyword=_ --keyword=N_ --from-code=utf-8 --files-from=$(srcdir)/POTFILES.in --no-wrap --add-comments="TRANSLATORS:" -d @PACKAGE_NAME@ -s -o $@ $^
	$(SED) --in-place $@ --expression=s/CHARSET/UTF-8/
	
%.po: @PACKAGE_NAME@.pot
	if [ -f $@ ]; then \
		$(MSGMERGE) --backup=none --no-wrap --no-location -s -U $@ $< && touch $@; \
		$(MSGATTRIB) --no-wrap --no-obsolete $@ -o $@; \
	else \
		$(MSGINIT) -l $@ -o $@ -i $< --no-translator; \
	fi

.po.mo:
	$(MSGFMT) -o $@ $<

BUILT_SOURCES = $(LANGMO)
CLEANFILES = $(LANGMO) neutrino-hd2.pot

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
	
	
