#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

#include "../Graphics/Bitmap.c"
#include "../Graphics/PNG.c"
#include "../Graphics/RLEBitmap.c"
#include "../Graphics/DrawingPixels.c"
#include "../Platform/ResourceBundle.c"

static FILE *CreateArchive(const char *archivepath,int numfiles);
static void AddFileToArchive(FILE *fh,ResourceBundleEntry *entry,const char *file);
static void SortEntryList(ResourceBundleEntry *entries,int numentries);
static void WriteEntryListToArchive(FILE *fh,ResourceBundleEntry *entries,int numentries);

static void WriteByte(FILE *fh,uint8_t byte);
static void WriteUInt32LE(FILE *fh,uint32_t value);
static void WriteBytes(FILE *fh,void *bytes,size_t length);

static Bitmap *PNGToBitmap(const void *bytes,size_t length,size_t *bitmapsize);
static void *PNGToRLEBitmapFrames(const void *bytes,size_t length,int framewidth,int frameheight,size_t *rlesize);
static Bitmap *LoadPNG(const void *bytes,size_t length,size_t *bitmapsize);

int main(int argc,const char **argv)
{
	if(argc<3)
	{
		fprintf(stderr,"Usage: %s archive.laser file1 [file2:mode file3...]\n",argv[0]);
		return 1;
	}

	int numentries=argc-2;

	FILE *fh=CreateArchive(argv[1],numentries);
	ResourceBundleEntry entries[numentries];

	for(int i=0;i<numentries;i++)
	{
		const char *filename=argv[i+2];
		fprintf(stderr,"%s... ",filename);
		fflush(stderr);
		AddFileToArchive(fh,&entries[i],filename);
		fprintf(stderr,"\n");
	}

	SortEntryList(entries,numentries);
	WriteEntryListToArchive(fh,entries,numentries);

	fclose(fh);

	return 0;
}

static FILE *CreateArchive(const char *archivepath,int numfiles)
{
	// Create archive file.
	FILE *fh=fopen(archivepath,"wb");
	if(!fh)
	{
		fprintf(stderr,"Failed to create archive file \"%s\".\n",archivepath);
		exit(1);
	}

	// Write magic number and number of files field.
	WriteBytes(fh,"LoVeSiGn",8);
	WriteUInt32LE(fh,numfiles);

	// Write space for filename.
	for(int i=0;i<128;i++) WriteByte(fh,0);

	// Write dummy entry headers.
	for(int i=0;i<numfiles*sizeof(ResourceBundleEntry);i++) WriteByte(fh,0);

	return fh;
}

static void AddFileToArchive(FILE *fh,ResourceBundleEntry *entry,const char *file)
{
	// Parse the file argument into a filename and converter parameters.
	size_t namelength;
	const char *separator=strrchr(file,'?');
	if(separator) namelength=separator-file;
	else namelength=strlen(file);

	char filename[namelength+1];
	memcpy(filename,file,namelength);
	filename[namelength]=0;

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

	// Convert file, if requested.
	void *actualbytes;
	size_t actualsize;
	uint32_t type;

	if(separator)
	{
		const char *converter=separator+1;
		if(strcasecmp(converter,"bitmap")==0)
		{
			actualbytes=PNGToBitmap(contents,st.st_size,&actualsize);
			type='B';
		}
		else if(strncasecmp(converter,"rle",3)==0)
		{
			int framewidth,frameheight;
			if(sscanf(converter+3,".%dx%d",&framewidth,&frameheight)!=2 && 
			sscanf(converter+3,".%dX%d",&framewidth,&frameheight)!=2)
			{
				framewidth=frameheight=0;
			}

			actualbytes=PNGToRLEBitmapFrames(contents,st.st_size,framewidth,frameheight,&actualsize);
			type='R';
		}
		else
		{
			fprintf(stderr,"Unknown converter.\n");
			exit(1);
		}

		free(contents);
	}
	else
	{
		actualbytes=contents;
		actualsize=st.st_size;
		type=0;
	}

	// Append file data to the end of the file.
	fseek(fh,0,SEEK_END);
	long offset=ftell(fh);
	WriteBytes(fh,actualbytes,actualsize);

	// Pad to keep data aligned.
	int pad=(-actualsize)&3;
	if(pad) WriteBytes(fh,(char[3]){ 3,9,8 },pad);

	// Free data.
	free(actualbytes);

	// Store file entry information in entry.
	entry->offset=(uint32_t)offset;
	entry->size=(uint32_t)actualsize;
	entry->type=type;

	// Strip extension and path from filename, and store it.
	char *ext=strrchr(filename,'.');
	if(ext) *ext=0;

	const char *filepart;
	const char *slash=strrchr(filename,'/');
	if(slash) filepart=slash+1;
	else filepart=filename;

	strncpy(entry->name,filepart,sizeof(entry->name));
}




static int CompareEntryNames(const void *a,const void *b)
{
	const ResourceBundleEntry *first=a,*second=b;

	return strncmp(first->name,second->name,sizeof(first->name));
}

static void SortEntryList(ResourceBundleEntry *entries,int numentries)
{
	qsort(entries,numentries,sizeof(ResourceBundleEntry),CompareEntryNames);
}

static void WriteEntryListToArchive(FILE *fh,ResourceBundleEntry *entries,int numentries)
{
	fseek(fh,12+128,SEEK_SET);

	for(int i=0;i<numentries;i++)
	{
		WriteUInt32LE(fh,entries[i].offset);
		WriteUInt32LE(fh,entries[i].size);
		WriteUInt32LE(fh,entries[i].type);
		WriteBytes(fh,entries[i].name,sizeof(entries[i].name));
	}
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




static inline int Dither(int c,int x,int y,int bits)
{
	switch(bits)
	{
		case 5:
		{
			static const int dither[8]=
			{
				0,4,1,5,
				6,2,7,3,
			};

			int index=(x&3)|((y&1)<<2);

			if((c&7)>dither[index]) return c+8;
			else return c;
		}
		break;

		case 6:
		{
			static const int dither[4]=
			{
				0,2,
				3,1,
			};

			int index=(x&1)|((y&1)<<1);

			if((c&3)>dither[index]) return c+4;
			else return c;
		}
		break;

		default:
			return c;
		break;
	}
}

static void *PixelFunc(void *destination,uint8_t r,uint8_t g,uint8_t b,uint8_t a,int x,int y)
{
	Pixel *dest=destination;

	*dest=RGBA(
	Dither(r,x,y,RedPixelBits),
	Dither(g,x,y,GreenPixelBits),
	Dither(b,x,y,BluePixelBits),
	a);

	return dest+1;
}

Bitmap *PNGToBitmap(const void *bytes,size_t length,size_t *bitmapsize)
{
	Bitmap *bitmap=LoadPNG(bytes,length,bitmapsize);

	uintptr_t offset=(uint8_t *)bitmap->pixels-(uint8_t *)bitmap;
	bitmap->pixels=(Pixel *)offset;

	return bitmap;
}

static void *PNGToRLEBitmapFrames(const void *bytes,size_t length,int framewidth,int frameheight,size_t *rlesize)
{
	Bitmap *bitmap=LoadPNG(bytes,length,NULL);

	if(!framewidth) framewidth=bitmap->width;
	if(!frameheight) frameheight=bitmap->height;

	int framecols=bitmap->width/framewidth;
	int framerows=bitmap->height/frameheight;
	int numframes=framecols*framerows;

	size_t totalsize=8*numframes;
	uint32_t frameoffsets[numframes];

	for(int row=0;row<framerows;row++)
	for(int col=0;col<framecols;col++)
	{
		int n=row*framecols+col;
		frameoffsets[n]=(uint32_t)totalsize;

		size_t size=SizeOfRLEBitmapFromPartialBitmap(bitmap,
		col*framewidth,row*frameheight,framewidth,frameheight);
		size=(size+3)&~3; // Make sure to pad to 32 bits.

		totalsize+=size;
	}

	uint8_t *rlebytes=malloc(totalsize);

	for(int row=0;row<framerows;row++)
	for(int col=0;col<framecols;col++)
	{
		int n=row*framecols+col;
		RLEBitmap *rle=(RLEBitmap *)&rlebytes[frameoffsets[n]];
		InitializeRLEBitmapFromPartialBitmap(rle,bitmap,
		col*framewidth,row*frameheight,framewidth,frameheight);
	}

	memcpy(rlebytes,frameoffsets,4*numframes);
	memset(&rlebytes[4*numframes],0,4*numframes);

	if(rlesize) *rlesize=totalsize;

	return rlebytes;
}

static Bitmap *LoadPNG(const void *bytes,size_t length,size_t *bitmapsize)
{
	PNGLoader loader;
	InitializePNGLoader(&loader,bytes,length);

	if(!LoadPNGHeader(&loader))
	{
		fprintf(stderr,"Not a valid PNG file.\n");
		exit(1);
	}

	int bytesperrow=BytesPerRowForWidth(loader.width);
	size_t size=16+loader.height*bytesperrow;

	uint8_t *bitmapbytes=malloc(size);
	Bitmap *bitmap=(Bitmap *)bitmapbytes;
	void *pixels=bitmapbytes+16;

	InitializeBitmap(bitmap,loader.width,loader.height,bytesperrow,pixels);

	if(!LoadPNGImageData(&loader,pixels,bytesperrow,PixelFunc))
	{
		fprintf(stderr,"Failed while loading PNG data.\n");
		exit(1);
	}

	if(bitmapsize) *bitmapsize=size;

	return bitmap;
}
