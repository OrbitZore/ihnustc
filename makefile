HPPFILES=$(wildcard src/*.hpp)
VERSION=1.0

build/ihnustc: src/main.cpp $(HPPFILES)
	g++ $< -std=c++20 -o $@ -lcurl -Os

arm/build/ihnustc: src/main.cpp $(HPPFILES)
	aarch64-linux-gnu-g++ $< -std=c++20 -o $@ arm/lib/libcurl.a -Iarm/include -static -Os

upload: arm/build/ihnustc
	rsync arm/build/ihnustc route:/root/app/ihnustc/build/

run: build/ihnustc
	exec ./$< config/config.ini

build/release/ihnustc-linux-x86_64-v$(VERSION).gz: build/ihnustc
	gzip -9 -c $< > $@

build/release/ihnustc-linux-arm64-static-v$(VERSION).gz: arm/build/ihnustc
	gzip -9 -c $< > $@

pack: build/release/ihnustc-linux-arm64-static-v$(VERSION).gz build/release/ihnustc-linux-x86_64-v$(VERSION).gz