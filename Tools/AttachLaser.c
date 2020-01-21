#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

typedef struct ResourceBundleHeader
{
	uint32_t offset,size;
	char name[128];
} ResourceBundleHeader;

static void ReadAndAttachBundle(FILE *fh,const char *filename,ResourceBundleHeader *header);

static void WriteByte(FILE *fh,uint8_t byte);
static void WriteUInt32LE(FILE *fh,uint32_t value);
static void WriteBytes(FILE *fh,void *bytes,size_t length);

int main(int argc,const char **argv)
{
	if(argc<3)
	{
		fprintf(stderr,"Usage: %s executable archive.laser [archive2.laser ...]\n",argv[0]);
		exit(1);
	}

	// Open archive file for readwrite and seek to end.
	const char *exename=argv[1];
	FILE *fh=fopen(exename,"rb+");
	if(!fh)
	{
		fprintf(stderr,"Failed to open file \"%s\".\n",exename);
		exit(1);
	}

	fseek(fh,0,SEEK_END);

	// Read and attach all the requested bundles, and collect
	// information about all of them for the directory.
	int numbundles=argc-2;
	ResourceBundleHeader headers[numbundles];

	for(int i=0;i<numbundles;i++)
	{
		const char *filename=argv[i+2];
		fprintf(stderr,"Attaching file %s to %s... ",filename,exename);
		fflush(stderr);
		ReadAndAttachBundle(fh,filename,&headers[i]);
		fprintf(stderr,"\n");
	}

	// Write directory.
	for(int i=0;i<numbundles;i++)
	{
		WriteUInt32LE(fh,headers[i].offset);
		WriteUInt32LE(fh,headers[i].size);
		WriteBytes(fh,headers[i].name,sizeof(headers[i].name));
	}

	// Write footer.
	WriteUInt32LE(fh,numbundles);
	WriteBytes(fh,"MaGiCaNnOn",10);

	fclose(fh);

	return 0;
}

static void ReadAndAttachBundle(FILE *fh,const char *filename,ResourceBundleHeader *header)
{
	// Remember file offset for the start of the bundle.
	uint32_t offset=(uint32_t)ftell(fh);

	// Find file size.
	struct stat st;
	if(stat(filename,&st)!=0)
	{
		fprintf(stderr,"Could not find file.\n");
		exit(1);
	}

	// Open the file, then alloc and read the contents.
	FILE *in=fopen(filename,"rb");
	if(!in)
	{
		fprintf(stderr,"Failed to open file.\n");
		exit(1);
	}

	void *contents=malloc(st.st_size);
	if(!contents)
	{
		fprintf(stderr,"Out of memory.\n");
		exit(1);
	}

	if(fread(contents,1,st.st_size,in)!=st.st_size)
	{
		fprintf(stderr,"Failed to read from file.\n");
		exit(1);
	}

	fclose(in);

	WriteBytes(fh,contents,st.st_size);

	// Fill in header fields.
	header->offset=offset;
	header->size=st.st_size;

	strncpy(header->name,filename,sizeof(header->name));

	// Remove extension if necessary.
	const char *filepart;
	const char *slash=strrchr(filename,'/');
	if(slash) filepart=slash+1;
	else filepart=filename;

	const char *dot=strrchr(filepart,'.');
	if(dot && dot-filename<sizeof(header->name)) header->name[dot-filename]=0;
}


static void WriteByte(FILE *fh,uint8_t byte)
{
	WriteBytes(fh,&byte,1);
}

static void WriteUInt32LE(FILE *fh,uint32_t value)
{
	uint8_t bytes[4]={ value&0xff,(value>>8)&0xff,(value>>16)&0xff,(value>>24)&0xff };
	WriteBytes(fh,bytes,4);
}

static void WriteBytes(FILE *fh,void *bytes,size_t length)
{
	if(fwrite(bytes,1,length,fh)!=length)
	{
		fprintf(stderr,"Failed to write to archive.\n");
		exit(1);
	}
}
