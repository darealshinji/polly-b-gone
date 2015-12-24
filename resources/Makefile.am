EXTRA_DIST = \
	COPYING.CC0 \
	COPYING.CC-BY-2.5 \
	COPYING.CC-BY-3.0 \
	COPYING.CC-BY-SA-3.0 \
	COPYING.GPL-2.0 \
	COPYING.GPL-3.0 \
	COPYING.LGPL-2.1 \
	COPYING.LGPL-3.0

# OSX .app files
EXTRA_DIST += Info.plist Polly.icns

# freedesktop icons
EXTRA_DIST += \
	polly-b-gone_16x16.png \
	polly-b-gone_22x22.png \
	polly-b-gone_24x24.png \
	polly-b-gone_32x32.png \
	polly-b-gone_48x48.png \
	polly-b-gone_64x64.png \
	polly-b-gone_96x96.png \
	polly-b-gone_128x128.png


game_DATA_NONFREE = \
	non-free/04ptboyf.ogg \
	non-free/ceramic.jpg \
	non-free/clovers.jpg \
	non-free/concrete.jpg \
	non-free/drain.jpg \
	non-free/half.ogg \
	non-free/hidnseek.ogg \
	non-free/ivy.jpg \
	non-free/metal.jpg \
	non-free/world.xml
EXTRA_DIST += $(game_DATA_NONFREE)

game_DATA_GPL = \
	ceramic.jpg \
	clovers.jpg \
	concrete.jpg \
	drain.jpg \
	intro.ogg \
	ivy.jpg \
	loop.ogg \
	metal.jpg \
	outro.ogg \
	world.xml
EXTRA_DIST += $(game_DATA_GPL)

game_DATA_ANY = \
	normal.frag \
	normal.vert \
	title.png
EXTRA_DIST += $(game_DATA_ANY)

gamedir = $(pkgdatadir)
if NONFREE
game_DATA = $(game_DATA_NONFREE)
else
game_DATA = $(game_DATA_GPL)
endif
game_DATA += $(game_DATA_ANY)


desktopdir = $(datarootdir)/applications
desktop_DATA = polly-b-gone.desktop
EXTRA_DIST += $(desktop_DATA)


install-icons:
	for size in 16 22 24 32 48 64 96 128; do \
	  $(MKDIR_P) $(DESTDIR)$(datarootdir)/icons/hicolor/$${size}x$${size}/apps/; \
	  $(INSTALL_DATA) $(srcdir)/polly-b-gone_$${size}x$${size}.png \
	      $(DESTDIR)$(datarootdir)/icons/hicolor/$${size}x$${size}/apps/polly-b-gone.png; \
	done

install-data-local: install-icons

