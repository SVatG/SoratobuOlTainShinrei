#include <stdio.h>
#include <stdlib.h>

#include "../Engine/DeflateReader.h"

static void *AllocAndReadFile(FILE *fh,size_t *size);

int main(int argc,const char **argv)
{
	if(argc!=1)
	{
		fprintf(stderr,"Usage: %s <input.gz >output.data\n",argv[0]);
		exit(1);
	}

	size_t size;
	const uint8_t *bytes=AllocAndReadFile(stdin,&size);
	if(!bytes)
	{
		fprintf(stderr,"Out of memory.\n");
		exit(1);
	}


	if(size<10 || bytes[0]!=0x1f || bytes[1]!=0x8b || bytes[2]!=0x08 || (bytes[3]!=0x00&&bytes[3]!=0x08))
	{
		fprintf(stderr,"Not a valid gzip file.\n");
		exit(1);
	}

	const uint8_t *ptr=bytes+10;
	const uint8_t *end=bytes+size;

	if(bytes[3]==0x08)
	{
		do
		{
			if(ptr>=end) 
			{
				fprintf(stderr,"Not a valid gzip file.\n");
				exit(1);
			}
		}
		while(*ptr++!=0);
	}

	DeflateReader reader;
	InitializeDeflateReader(&reader);
	ProvideDeflateInputBuffer(&reader,ptr,end-ptr);

	uint32_t lastpos=0;
	while(ReadDeflateCode(&reader))
	{
		uint32_t newpos=DeflateStreamPosition(&reader);
		if(newpos!=lastpos)
		{
			for(uint32_t i=lastpos;i<newpos;i++) fputc(ByteFromDeflateWindow(&reader,i),stdout);
			lastpos=newpos;
		}
	}

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
