#include <stdio.h>
#include <stdlib.h>

#include "../Engine/PNG.h"

static void *AllocAndReadFile(FILE *fh,size_t *size);

static void *PixelFunc(void *destination,uint8_t r,uint8_t g,uint8_t b,uint8_t a)
{
	uint8_t *destpixel=destination;
	destpixel[0]=(r*a+0xff*(0xff-a))/0xff;
	destpixel[1]=(g*a+0xff*(0xff-a))/0xff;
	destpixel[2]=(b*a+0xff*(0xff-a))/0xff;
	return destpixel+3;
}

int main(int argc,const char **argv)
{
	if(argc!=1)
	{
		fprintf(stderr,"Usage: %s <input.png >output.ppm\n",argv[0]);
		exit(1);
	}

	size_t size;
	const uint8_t *bytes=AllocAndReadFile(stdin,&size);
	if(!bytes)
	{
		fprintf(stderr,"Out of memory.\n");
		exit(1);
	}

	PNGLoader png;
	InitializePNGLoader(&png,bytes,size);

	if(!LoadPNGHeader(&png))
	{
		fprintf(stderr,"Not a valid PNG file.\n");
		exit(1);
	}

	fprintf(stderr,"PNG: %dx%d, bit depth %d, colour type %d.\n",png.width,png.height,png.bitdepth,png.colourtype);

	uint8_t *bitmap=malloc(png.width*png.height*3);
	if(!bitmap)
	{
		fprintf(stderr,"Out of memory.\n");
		exit(1);
	}

	if(!LoadPNGImageData(&png,bitmap,png.width*3,PixelFunc))
	{
		fprintf(stderr,"Error loading image data.\n");
		//exit(1);
	}

	printf("P6 %d %d 255\n",png.width,png.height);
	fwrite(bitmap,png.width*png.height*3,1,stdout);

	return 0;
}

void *AllocAndReadFile(FILE *fh,size_t *size)
{
	const int blocksize=4096;
	char *buf=malloc(blocksize);
	size_t total=0;

	for(;;)
	{
		size_t actual=fread(buf+total,1,blocksize,fh);
		total+=actual;
		if(actual<blocksize) break;
		buf=realloc(buf,total+blocksize);
	}

	*size=total;
	return buf;
}
