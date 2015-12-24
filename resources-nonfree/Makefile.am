EXTRA_DIST = $(NULL)

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


gamedir = $(pkgdatadir)
game_DATA = \
	04ptboyf.ogg \
	ceramic.jpg \
	clovers.jpg \
	concrete.jpg \
	drain.jpg \
	half.ogg \
	hidnseek.ogg \
	ivy.jpg \
	metal.jpg \
	normal.frag \
	normal.vert \
	title.png \
	world.xml
EXTRA_DIST += $(game_DATA)


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

