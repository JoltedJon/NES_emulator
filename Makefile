build:
	meson compile -C builddir

run: build
	./builddir/nes

clean:
	meson compile --clean -C builddir
