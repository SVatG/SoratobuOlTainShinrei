#include "Object.h"
#include "Game.h"

#include <stdlib.h>
#include <limits.h>
#include <assert.h>




Object *NewObject(ObjectList *list,size_t objsize,
ObjectRunFunction runfunc,ObjectHitFunction hitfunc,ObjectDrawFunction drawfunc)
{
//	assert(objsize<=AllocatorMemberSize(&game.allocator));
	assert(objsize<=MaximumObjectSize);

//	Object *self=malloc(objsize);
	Object *self=AllocMember(&game.allocator);
	if(!self) return NULL;

	game.objectcounter++;
	self->dogtag=game.objectcounter;

	self->x=self->y=0;;
	self->vx=self->vy=0;
	self->power=1;
	self->hitradius=0;
	self->birthtime=game.timer;
	self->minx=self->miny=self->maxx=self->maxy=INT32_MIN;

	self->runfunc=runfunc;
	self->hitfunc=hitfunc;
	self->drawfunc=drawfunc;

	AddObjectToList(list,self);

	return self;
}

void FreeObject(Object *self)
{
	self->dogtag=0;
	FreeMember(&game.allocator,self);
//	free(self);
}




void AimObjectAtPosition(Object *obj,int32_t x,int32_t y,int32_t vel)
{
	int32_t dx=XCoordDifference(x,obj->x);
	int32_t dy=YCoordDifference(y,obj->y);
	int32_t r=isqrt(isq(dx)+isq(dy));
	SetObjectVelocity(obj,idiv(imul(dx,vel),r),idiv(imul(dy,vel),r));
}

/*void ChangeObjectPosition(Object *obj,int32_t deltax,int32_t deltay)
{
	obj->x+=deltax;
	obj->y+=deltay;
}

void ChangeObjectVelocity(Object *obj,ivec2_t deltav)
{
	obj->v=ivec2add(obj->v,deltav);
}

void RandomizeObjectVelocity(Object *obj,int32_t amount)
{
	int angle=RandomInteger()%4096;
	int32_t speed=RandomInteger()%amount;
	ChangeObjectVelocity(obj,ivec2cyl(speed,angle));
}

void StaggerObjectPosition(Object *self,ivec2_t vel)
{
	int rand=RandomInteger()%Fix(1);
	self->r=ivec2add(self->r,ivec2mul(vel,rand));
}

void StaggerObjectMotion(Object *self)
{
	StaggerObjectPosition(self,self->v);
}*/





void InitializeObjectList(ObjectList *self)
{
	self->first=NULL;
}

void CleanupObjectList(ObjectList *self)
{
	ObjectIterator iterator;
	Object *obj;
	InitializeObjectIteratorForList(&iterator,self);

	while((obj=NextObject(&iterator)))
	{
		RemoveCurrentObject(&iterator);
		FreeObject(obj);
	}
}




void AddObjectToList(ObjectList *self,Object *obj)
{
	obj->next=self->first;
	self->first=obj;
}




void RunObjectList(ObjectList *self)
{
	ObjectIterator iterator;
	Object *obj;
	InitializeObjectIteratorForList(&iterator,self);

	while((obj=NextObject(&iterator)))
	{
		obj->lastx=obj->x;
		obj->lasty=obj->y;

		if(obj->runfunc)
		{
			if(!obj->runfunc(obj))
			{
				RemoveCurrentObject(&iterator);
				FreeObject(obj);
				continue;
			}
		}
	}
}

void MoveObjectList(ObjectList *self)
{
	ObjectIterator iterator;
	Object *obj;
	InitializeObjectIteratorForList(&iterator,self);

	while((obj=NextObject(&iterator)))
	{
		obj->x+=obj->vx;
		obj->y+=obj->vy;
	}
}

void RunAndMoveObjectList(ObjectList *self)
{
	RunObjectList(self);
	MoveObjectList(self);
}




void CalculateExtentsForObjectList(ObjectList *self)
{
	ObjectIterator iterator;
	Object *obj;
	InitializeObjectIteratorForList(&iterator,self);

	while((obj=NextObject(&iterator)))
	{
//		obj->minx=obj->x-obj->hitradius;
//		obj->miny=obj->y-obj->hitradius;
//		obj->maxx=obj->x+obj->hitradius;
//		obj->maxy=obj->y+obj->hitradius;
		int32_t x=WrappedXCoordForWorldCoord(obj->x);
		int32_t y=WrappedXCoordForWorldCoord(obj->y);
		obj->minx=x-obj->hitradius;
		obj->miny=y-obj->hitradius;
		obj->maxx=x+obj->hitradius;
		obj->maxy=y+obj->hitradius;
	}
}

void CalculateExactExtentsForObjectList(ObjectList *self)
{
	ObjectIterator iterator;
	Object *obj;
	InitializeObjectIteratorForList(&iterator,self);

	while((obj=NextObject(&iterator)))
	{
//		obj->minx=imin(obj->x,obj->lastx)-obj->hitradius;
//		obj->miny=imin(obj->y,obj->lasty)-obj->hitradius;
//		obj->maxx=imax(obj->x,obj->lastx)+obj->hitradius;
//		obj->maxy=imax(obj->y,obj->lasty)+obj->hitradius;
	}
}



static int MinYCompare(const void *a,const void *b)
{
	Object *first=*(Object **)a;
	Object *second=*(Object **)b;
	if(first->miny==second->miny) return 0;
	else if(first->miny<second->miny) return -1;
	else return 1;
}

void QSortObjectList(ObjectList *self)
{
	if(!self->first||!self->first->next) return;

	int count=0;

	Object *obj=self->first;
	while(obj) { count++; obj=obj->next; }

	Object **array=malloc(count*sizeof(Object *));
	obj=self->first;
	for(int i=0;i<count;i++) { array[i]=obj; obj=obj->next; }

	qsort(array,count,sizeof(Object *),MinYCompare);

	for(int i=0;i<count-1;i++) array[i]->next=array[i+1];
	array[count-1]->next=NULL;

	self->first=array[0];

	free(array);
}

void InsertionSortObjectList(ObjectList *self)
{
	// TODO: implement!
	QSortObjectList(self);
}

void MergeObjectLists(ObjectList *first,ObjectList *second)
{
	if(!second->first) return;
	if(!first->first)
	{
		first->first=second->first;
		second->first=NULL;
		return;
	}

	Object *obj=first->first;
	while(obj->next) obj=obj->next;

	obj->next=second->first;
	second->first=NULL;
}

void MergeSortedObjectLists(ObjectList *first,ObjectList *second)
{
	if(!second->first) return;
	if(!first->first)
	{
		first->first=second->first;
		second->first=NULL;
		return;
	}

	Object *obj1=first->first;
	Object *obj2=second->first;
	Object **lastptr=&first->first;

	while(obj1&&obj2)
	{
		if(obj1->miny<obj2->miny)
		{
			*lastptr=obj1;
			lastptr=&obj1->next;
			obj1=obj1->next;
		}
		else
		{
			*lastptr=obj2;
			lastptr=&obj2->next;
			obj2=obj2->next;
		}
	}

	if(!obj1) *lastptr=obj2;
	else if(!obj2) *lastptr=obj1;

	second->first=NULL;
}



static bool SquareCollisionTest(Object *obj1,Object *obj2)
{
	return true;
}

static bool RegularCollisionTest(Object *obj1,Object *obj2)
{
//	return ivec2dist64(obj2->r,obj1->r)<=isq64(obj2->hitradius+obj1->hitradius);
	return true;
}

void CollideSortedObjectLists(ObjectList *list1,ObjectList *list2)
{
	return CollideSortedObjectListsWithCallback(list1,list2,RegularCollisionTest);
}

/*static bool ExactCollisionTest(Object *obj1,Object *obj2)
{
	// TODO: Use wrapped coordinates in case this function is ever used.
	int32_t x0=obj2->lastx-obj1->lastx;
	int32_t y0=obj2->lasty-obj1->lasty;
	int32_t dx=obj2->x-obj1->x-x0;
	int32_t dy=obj2->y-obj1->y-y0;
	int32_t r=obj2->hitradius+obj1->hitradius;

	int64_t c=isq64(x0)+isq64(y0)-isq64(r);
	if(c<=0) return true; // Start point is inside collision range.

	int64_t b=2*(imul64(x0,dx)+imul64(y0,dy));
	if(b>0) return false; // Heading away from collision.

	int64_t a=isq64(dx)+isq64(dy);
	if(a+b+c<=0) return true; // End point is inside collision range.

	int64_t discr=isq64(b)-4*imul64(a,c);
	if(discr<0) return false; // Misses along the entire line.

	int64_t test1=-b-2*a;
	if(test1<0) return true;

	int64_t test2=-b-2*a-isqrt64(discr);
	if(test2<0) return true; // The point of intersection is before the end point.

	return false;
}*/

/*void CollideSortedObjectListsExactly(ObjectList *list1,ObjectList *list2)
{
	return CollideSortedObjectListsWithCallback(list1,list2,ExactCollisionTest);
}*/

void CollideSortedObjectListsWithCallback(ObjectList *list1,ObjectList *list2,CollisionCallbackFunction callback)
{
	ObjectIterator iterator1,iterator2;
	InitializeObjectIteratorForList(&iterator1,list1);
	InitializeObjectIteratorForList(&iterator2,list2);

	Object *obj2=NextObject(&iterator2);
	if(!obj2) return;

	for(;;)
	{
		// Step down first list.
		Object *obj1=NextObject(&iterator1);
		if(!obj1) return;

		// Step down second list if needed until bottom passes the top of the first.
		while(obj2->maxy<obj1->miny)
		{
			obj2=NextObject(&iterator2);
			if(!obj2) return;
		}

		// If there is an overlap, check for collisions.
		if(obj1->maxy>=obj2->miny)
		{
			// Clone second iterator to temporarily walk down the second list.
			ObjectIterator iterator;
			InitializeObjectIteratorAsClone(&iterator,&iterator2);
			Object *obj=obj2;

			// Walk down second list until top passes bottom of first.
			do
			{
				// Check for collision.
				if(obj->minx<=obj1->maxx)
				if(obj->maxx>=obj1->minx)
				if(obj->hitradius && obj1->hitradius)
				if(callback(obj1,obj))
				{
					// Run collision functions and remove objects as needed.
					int power1=obj1->power;
					int power2=obj->power;
					bool remove1=obj1->hitfunc && !obj1->hitfunc(obj1,obj,power2);
					bool remove2=obj->hitfunc && !obj->hitfunc(obj,obj1,power1);

					if(remove2)
					{
						// If the object in the second list died, remove it.
						RemoveCurrentObject(&iterator);
						FreeObject(obj);
					}

					if(remove1)
					{
						// If the object in the first list died, remove it and stop the loop.
						RemoveCurrentObject(&iterator1);
						FreeObject(obj1);
						break;
					}
				}

				// Find next object in second list that isn't above first.
				do obj=NextObject(&iterator);
				while(obj && obj->maxy<obj1->miny);
			}
			while(obj && obj1->maxy>=obj->miny);

			// Check if we deleted the head of the second list.
			if(CurrentObjectWasRemoved(&iterator2))
			{
				obj2=NextObject(&iterator2);
				if(!obj2) return;
			}
		}
	}
}

int CollideObjectListAgainstSquareWithPower(ObjectList *list,int32_t x,int32_t y,int32_t radius,int totalpower)
{
	ObjectIterator iterator;
	InitializeObjectIteratorForList(&iterator,list);

	x=WrappedXCoordForWorldCoord(x);
	y=WrappedXCoordForWorldCoord(y);

	Object *obj;
	while((obj=NextObject(&iterator)) && totalpower>0)
	{
		if(obj->minx<=x+radius && obj->maxx>=x-radius)
		if(obj->miny<=y+radius && obj->maxy>=y-radius)
		if(obj->hitradius)
		{
			int power=obj->power;

			if(obj->hitfunc && !obj->hitfunc(obj,NULL,totalpower))
			{
				RemoveCurrentObject(&iterator);
				FreeObject(obj);
			}

			totalpower-=power;
		}
	}
	return totalpower;
}

int CollideObjectListAgainstCircleWithPower(ObjectList *list,int32_t x,int32_t y,int32_t radius,int totalpower)
{
	ObjectIterator iterator;
	InitializeObjectIteratorForList(&iterator,list);

	x=WrappedXCoordForWorldCoord(x);
	y=WrappedXCoordForWorldCoord(y);

	Object *obj;
	while((obj=NextObject(&iterator)) && totalpower>0)
	{
		if(obj->minx<=x+radius && obj->maxx>=x-radius)
		if(obj->miny<=y+radius && obj->maxy>=y-radius)
		if(obj->hitradius)
//		if(isq(obj->x-x)+isq(obj->y-y)<=isq(obj->hitradius+radius))
		if(isq(WrappedXCoordForWorldCoord(obj->x)-x)+
		isq(WrappedXCoordForWorldCoord(obj->y)-y)<=isq(obj->hitradius+radius))
		{
			int power=obj->power;

			if(obj->hitfunc && !obj->hitfunc(obj,NULL,totalpower))
			{
				RemoveCurrentObject(&iterator);
				FreeObject(obj);
			}

			totalpower-=power;
		}
	}
	return totalpower;
}



/*void ClipObjectList(ObjectList *self,int32_t left,int32_t right,int32_t top,int32_t bottom)
{
	ObjectIterator iterator;
	Object *obj;
	InitializeObjectIteratorForList(&iterator,self);

	while((obj=NextObject(&iterator)))
	{
		if(obj->x<left||obj->x>right||obj->y<top||obj->y>bottom)
		{
			RemoveCurrentObject(&iterator);
			FreeObject(obj);
			continue;
		}
	}
}*/

void ClipObjectListAgainstScreen(ObjectList *self,int left,int right,int top,int bottom)
{
	ObjectIterator iterator;
	Object *obj;
	InitializeObjectIteratorForList(&iterator,self);

	while((obj=NextObject(&iterator)))
	{
		int sx=ScreenXCoordForWorldCoord(obj->x);
		int sy=ScreenYCoordForWorldCoord(obj->y);
		if(sx<left||sx>right||sy<top||sy>bottom)
		{
			RemoveCurrentObject(&iterator);
			FreeObject(obj);
			continue;
		}
	}
}

void DrawObjectList(ObjectList *self,Bitmap *screen)
{
	ObjectIterator iterator;
	Object *obj;
	InitializeObjectIteratorForList(&iterator,self);

	while((obj=NextObject(&iterator)))
	{
		if(obj->drawfunc) obj->drawfunc(obj,screen);
	}
}
