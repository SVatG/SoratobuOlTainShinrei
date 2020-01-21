#include "ResourceBundle.h"

#include "../Graphics/Bitmap.h"
#include "../Graphics/RLEBitmap.h"

#include <string.h>

bool FixUpResourceBundle(ResourceBundle *self,const char *filename)
{
	if(memcmp(self->head.magic,"LoVeSiGn",8)!=0) return false;

	self->head.next=NULL;
	strncpy(self->name,filename,sizeof(self->name));

	for(int i=0;i<self->numentries;i++)
	{
		uint8_t *base=(uint8_t *)self+self->entries[i].offset;
		switch(self->entries[i].type)
		{
			case 'B': // Bitmap
			{
				uint32_t offset=*(uint32_t *)&base[8];
				Bitmap *bitmap=(Bitmap *)base;
				bitmap->pixels=(Pixel *)(base+offset);
			}
			break;

			case 'R': // RLEBitmap
			{
				int numframes=1;
				uint32_t *offsets=(uint32_t *)base;
				while(offsets[numframes]!=0) numframes++;

				void **pointers=(void **)base;
				for(int i=numframes-1;i>=0;i--) pointers[i]=base+offsets[i];
			}
			break;
		}
	}

	return true;
}

void *DataForResourceWithName(ResourceBundle *self,const char *name,size_t *sizeptr)
{
	if(!self) return NULL;

	const char *slashptr=strrchr(name,'/');
	if(slashptr)
	{
		size_t pathlength=slashptr-name;
		if(pathlength>sizeof(self->name)) pathlength=sizeof(self->name);

		if(strncmp(name,self->name,pathlength)!=0||
		strnlen(self->name,sizeof(self->name))!=pathlength)
		{
			return DataForResourceWithName(self->head.next,name,sizeptr);
		}
	}

	const char *filepart;
	if(slashptr) filepart=slashptr+1;
	else filepart=name;

	if(self->numentries>0)
	{
		int first=0;
		int last=self->numentries-1;

		for(;;)
		{
			int mid=(first+last)/2;
			int res=strncmp(self->entries[mid].name,filepart,sizeof(self->entries[0].name));
			if(res==0)
			{
				if(sizeptr) *sizeptr=self->entries[mid].size;
				return (char *)self+self->entries[mid].offset;
			}
			else if(res<0)
			{
				if(last==mid) break;
				first=mid+1;
			}
			else
			{
				if(first==mid) break;
				last=mid-1;
			}
		}
	}

	return DataForResourceWithName(self->head.next,name,sizeptr);
}
