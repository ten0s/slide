SUBDIRS = lib gi src
PREFIX ?= $(CURDIR)/install

all:
	@for subdir in $(SUBDIRS); do \
		cd $$subdir            && \
		$(MAKE) all            && \
		cd ..                   ; \
	done

.PHONY: install
install:
	echo $(PREFIX)
	@for subdir in $(SUBDIRS);           do \
		cd $$subdir                      && \
		$(MAKE) install PREFIX=$(PREFIX) && \
		cd ..                             ; \
	done

.PHONY: clean
clean:
	@for subdir in $(SUBDIRS); do \
		cd $$subdir            && \
		$(MAKE) clean          && \
		cd ..                   ; \
	done
