pages   = geds_context_new.3.gz geds_context_free.3.gz geds_point_new.3.gz\
	  geds_point_free.3.gz geds_set_point.3.gz geds_set_length.3.gz   \
	  geds_set_curve.3.gz geds_set_subgroup.3.gz geds_set_rnd.3.gz    \
	  geds_set_signature.3.gz geds_get_signature.3.gz geds_check.3.gz \
	  geds_generate.3.gz

man3dir = /usr/share/man/man3/

all: $(pages)

%.3.gz: %.3
	gzip $<

%.3: %.man
	cp $< $@

.PHONY: install
install:
	install $(pages) $(man3dir)

.PHONY: uninstall
uninstall:
	cd $(man3dir); rm -f $(pages)

.PHONY: clean
clean:
	rm -f $(pages)

