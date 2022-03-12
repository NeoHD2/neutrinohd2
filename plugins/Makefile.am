AUTOMAKE_OPTIONS = gnu

SUBDIRS = \
	test \
	netzkino \
	youtube \
	systeminfo \
	tuxcom \
	fileplayer \
	fileexplorer \
	mediaportal \
	icecast \
	upnp \
	internetradio \
	tsbrowser \
	moviebrowser \
	picviewer \
	audioplayer \
	nfilm \
	ntvshows

#testing
if ENABLE_TESTING
SUBDIRS += satipclient \
	opkg_manager \
	tuxcal
endif

if ENABLE_PYTHON
SUBDIRS += python
endif

if ENABLE_LUA
SUBDIRS += lua \
	netzkino_hd \
	mtv \
	media_one \
	arte_concert \
	plutotv
endif




