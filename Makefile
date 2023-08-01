SUBDIRS = lib gi src
PREFIX ?= $(CURDIR)/install
VERSION := $(shell util/get-version)
ISCC := "/c/Program Files (x86)/Inno Setup 6/ISCC.exe"

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

readme:
	sed -E -e "s/\{\{version\}\}/${VERSION}/g" README.template    > README.md
	sed -E -e "s/\{\{version\}\}/${VERSION}/g" README-ru.template > README-ru.md

windows-package:
	$(MAKE) windows-prepare
	$(MAKE) windows-zip
	$(MAKE) windows-installer

windows-prepare:
	rm -rf                  slide-${VERSION}-win-x64/
	mkdir -p                slide-${VERSION}-win-x64/bin/
	cp install/bin/*.exe    slide-${VERSION}-win-x64/bin/
	cp LICENSE              slide-${VERSION}-win-x64/
	cp README.md            slide-${VERSION}-win-x64/
	cp README-ru.md         slide-${VERSION}-win-x64/
	windows/copy-mingw64-deps.sh slide-${VERSION}-win-x64/
	node util/notice-mingw64.js  slide-${VERSION}-win-x64/bin/ > slide-${VERSION}-win-x64/NOTICE

windows-zip:
	zip -r slide-${VERSION}-win-x64.zip slide-${VERSION}-win-x64/

windows-installer:
	sed -E -e "s/\{\{version\}\}/${VERSION}/g" windows/installer.iss | ${ISCC} //O"." //F"slide-${VERSION}-win-x64-setup" -
	# Zip to upload to GH Releases
	zip slide-${VERSION}-win-x64-setup.zip slide-${VERSION}-win-x64-setup.exe
