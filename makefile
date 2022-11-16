HPPFILES=$(wildcard src/*.hpp)

build/ihnustc: src/main.cpp $(HPPFILES)
	g++ $< -std=c++20 -o $@ -lcurl -Os

arm/build/ihnustc: src/main.cpp $(HPPFILES)
	aarch64-linux-gnu-g++ $< -std=c++20 -o $@ arm/lib/libcurl.a -Iarm/include -static -Os

upload: arm/build/ihnustc
	rsync arm/build/ihnustc route:/root/app/ihnustc/build/

run: build/ihnustc
	exec ./$< config/config.ini
