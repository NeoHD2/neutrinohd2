################################################################################################################################################################################################################################################################
# Makefile for building neutrinoHD2 for x86		
# build options:
#  --with-configdir=PATH   where to find the config files [PREFIX/var/tuxbox/config]
#  --with-datadir=PATH     where to find data [PREFIX/share/tuxbox]
#  --with-plugindir=PATH   where to find the plugins [PREFIX/var/tuxbox/plugins]
#  --with-dvbincludes=PATH path for dvb includes [NONE]
#  --with-driver=PATH      path for driver sources [NONE]
#  --with-boxtype          
#  --enable-keyboard-no-rc enable keyboard control, disable rc control
#  --enable-opengl         include opengl framebuffer support for x86
#  --enable-playback       include enable playback for opengl and satip
#  --enable-gstreamer      include gstreamer as player engine support
#  --enable-lcd            include lcd support
#  --enable-scart          enable scart output
#  --enable-ci             enable ci cam
#  --enable-4digits        include 4 segment lcd support
#  --enable-functionkeys   include RC functions keys support
#  --enable-lua
#  --enable-python
#
#
# build preqs
# sudo apt-get install autoconf libtool libtool-bin g++ gdb swig flex bison make texinfo subversion intltool dialog wget cmake gperf libavformat-dev libglew-dev freeglut3-dev libcurl4-gnutls-dev libfreetype6-dev libid3tag0-dev libmad0-dev libogg-dev libpng-dev libgif-dev libjpeg-dev libflac-dev libvorbis-dev libopenthreads-dev libblkid-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev libfribidi-dev libass-dev python-dev lua5.2 lua5.2-dev lua-json lua-expat lua-posix lua-socket lua-soap lua-curl
################################################################################################################################################################################################################################################################
SHELL = /bin/bash
UID := $(shell id -u)
ifeq ($(UID), 0)
warn:
	@echo "You are running as root. Do not do this, it is dangerous."
	@echo "Aborting the build. Log in as a regular user and retry."
else
LC_ALL:=C
LANG:=C
export TOPDIR LC_ALL LANG

#
#
#

BOXTYPE = generic
DEST = $(PWD)/$(BOXTYPE)

N_SRC  = $(PWD)/nhd2-exp

CFLAGS = -Wall -O2 -fno-strict-aliasing -O0 -g -ggdb3

CXXFLAGS = $(CFLAGS)

export CFLAGS CXXFLAGS

# first target is default...
default: neutrino plugins
	make run

run:
	gdb -ex run $(DEST)/bin/neutrino

neutrino: $(N_SRC)/config.status
	-rm -f $(N_SRC)/src/gui/svn_version.h
	$(MAKE) -C $(N_SRC) install

$(N_SRC)/config.status: | $(N_SRC) $(DEST)
	$(N_SRC)/autogen.sh
	set -e; cd $(N_SRC); \
		$(N_SRC)/configure \
			--prefix=$(DEST)  \
			--build=i686-pc-linux-gnu \
			--enable-silent-rules \
			--enable-maintainer-mode \
			--with-boxtype=$(BOXTYPE) \
			--with-datadir=$(DEST)/share/tuxbox \
			--with-plugindir=$(DEST)/var/tuxbox/plugins \
			--with-configdir=$(DEST)/var/tuxbox/config \
			--enable-opengl \
			--enable-gstreamer \
			--enable-playback \
			--enable-scart \
			--enable-ci \
			--enable-python \
			--enable-lua
$(DEST):
	mkdir $@

$(N_SRC):
	git pull

neutrino-checkout: $(N_SRC)

neutrino-clean:
	-$(MAKE) -C $(N_SRC) clean

neutrino-distclean:
	-$(MAKE) -C $(N_SRC) distclean
	rm -f $(N_SRC)/config.status

# plugins
PLUGINS_SRC = $(PWD)/plugins
$(PLUGINS_SRC):
	git pull

plugins-checkout: $(PLUGINS_SRC)

plugins: $(PLUGINS_SRC)/config.status $(N_SRC)/config.status
	$(MAKE) -C $(PLUGINS_SRC) install

$(PLUGINS_SRC)/config.status: $(PLUGINS_SRC) $(DEST)
	$(PLUGINS_SRC)/autogen.sh
	set -e; cd $(PLUGINS_SRC); \
		$(PLUGINS_SRC)/configure \
			--prefix=$(DEST)  \
			--build=i686-pc-linux-gnu \
			--enable-silent-rules \
			--enable-maintainer-mode \
			--without-debug \
			--with-boxtype=$(BOXTYPE) \
			--with-datadir=$(DEST)/share/tuxbox \
			--with-plugindir=$(DEST)/var/tuxbox/plugins \
			--with-configdir=$(DEST)/var/tuxbox/config \
			--enable-testing \
			--enable-python \
			--enable-lua

plugins-clean:
	-$(MAKE) -C $(PLUGINS_SRC) clean

plugins-distclean:
	-$(MAKE) -C $(PLUGINS_SRC) distclean
	rm -f $(PLUGINS)/config.status

update:
	git pull

clean: neutrino-clean plugins-clean
distclean: neutrino-distclean plugins-distclean

PHONY = neutrino-checkout plugins-checkout
.PHONY: $(PHONY)

endif
