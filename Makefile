clean:
	rm -f objects/*o
	rm -f bin/*

libs:
	mkdir -p objects
	g++ -o objects/scan.o -c src/ini/scan.cpp
	g++ -o objects/ini.o -c src/ini/ini.cpp

scan: libs
	mkdir -p bin
	g++ -o objects/iniscan.o -c src/cmd/scan.cpp -I src/ini
	g++ -o bin/iniscan objects/scan.o objects/ini.o objects/iniscan.o

parse: libs
	mkdir -p bin
	g++ -o objects/iniparse.o -c src/cmd/parse.cpp -I src/ini
	g++ -o bin/iniparse objects/scan.o objects/ini.o objects/iniparse.o


all: libs scan parse
