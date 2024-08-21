build:
	meson compile -C builddir

run: build
	./builddir/nes
