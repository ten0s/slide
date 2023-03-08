SUBDIRS = lib gi src
PREFIX ?= $(CURDIR)/install
VERSION := $(shell util/get-version)

all:
	@for subdir in $(SUBDIRS); do \
		cd $$subdir            && \
		$(MAKE) all            && \
		cd ..                   ; \
	done

.PHONY: install
install:
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

roll-patch:
	@util/set-version `util/roll-version patch $(VERSION)`
	$(MAKE) roll

roll-minor:
	@util/set-version `util/roll-version minor $(VERSION)`
	$(MAKE) roll

roll-major:
	@util/set-version `util/roll-version major $(VERSION)`
	$(MAKE) roll

roll:
	@echo "New version: $(VERSION)"
	git add lib/slide_version.hpp
	@echo 'Now run: $$ make commit-push'

commit-push:
	git commit -m "Roll ${VERSION}"
	git tag "${VERSION}"
	git push origin `git branch --show-current 2>/dev/null` --tags
