all: TestDeflateReader PNGToPPM

TestDeflateReader: TestDeflateReader.c ../Engine/DeflateReader.h ../Engine/BitStreamReader.h
	gcc -std=c99 -g -o $@ $<

PNGToPPM: PNGToPPM.c ../Engine/PNG.c ../Engine/PNG.h ../Engine/DeflateReader.h ../Engine/BitStreamReader.h
	gcc -O1 -std=c99 -g -o $@ PNGToPPM.c ../Engine/PNG.c

