#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Utils.h"
#include "../Graphics/Bitmap.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Object Object;
typedef struct ObjectList ObjectList;
typedef struct ObjectIterator ObjectIterator;

typedef bool (*ObjectRunFunction)(Object *self);
typedef bool (*ObjectHitFunction)(Object *self,Object *other,int power);
typedef void (*ObjectDrawFunction)(Object *self,Bitmap *screen);

typedef bool (*CollisionCallbackFunction)(Object *first,Object *second);

struct Object
{
	Object *next;
	uint32_t dogtag;

	int32_t x,y,vx,vy;
	int power;
	int32_t hitradius;

	int32_t lastx,lasty;
	int32_t minx,maxx,miny,maxy;

	int birthtime;

	ObjectRunFunction runfunc;
	ObjectHitFunction hitfunc;
	ObjectDrawFunction drawfunc;
};

Object *NewObject(ObjectList *list,size_t objsize,
ObjectRunFunction runfunc,ObjectHitFunction hitfunc,ObjectDrawFunction drawfunc);
void FreeObject(Object *self);

static inline uint32_t ObjectDogTag(Object *self) { return self->dogtag; }
static inline bool IsObjectDead(Object *self,uint32_t dogtag) { return self->dogtag!=dogtag; }

static inline void SetObjectPosition(Object *self,int32_t x,int32_t y) { self->x=x; self->y=y; }
static inline void SetObjectVelocity(Object *self,int32_t vx,int32_t vy) { self->vx=vx; self->vy=vy; }
static inline void SetObjectVelocityAndDirection(Object *self,int32_t v,int a)
{ SetObjectVelocity(self,imul(v,icos(a)),imul(v,isin(a))); }
static inline void SetObjectPower(Object *self,int power) { self->power=power; }
static inline void SetObjectHitRadius(Object *self,int32_t hitradius) { self->hitradius=hitradius; }

void AimObjectAtPosition(Object *obj,int32_t x,int32_t y,int32_t vel);
static inline void AimObjectAtObject(Object *obj,Object *target,int32_t vel) { AimObjectAtPosition(obj,target->x,target->y,vel); }
/*void ChangeObjectPosition(Object *obj,ivec2_t deltar);
void ChangeObjectVelocity(Object *obj,ivec2_t deltav);
void RandomizeObjectVelocity(Object *obj,int32_t amount);
void StaggerObjectPosition(Object *self,ivec2_t vel);
void StaggerObjectMotion(Object *self);*/




struct ObjectList
{
	Object *first;
};

void InitializeObjectList(ObjectList *self);
void CleanupObjectList(ObjectList *self);

void AddObjectToList(ObjectList *self,Object *obj);

void RunObjectList(ObjectList *self);
void MoveObjectList(ObjectList *self);
void RunAndMoveObjectList(ObjectList *self);

void CalculateExtentsForObjectList(ObjectList *self);
void CalculateExactExtentsForObjectList(ObjectList *self);

void QSortObjectList(ObjectList *self);
void InsertionSortObjectList(ObjectList *self);
void MergeObjectLists(ObjectList *first,ObjectList *second);
void MergeSortedObjectLists(ObjectList *first,ObjectList *second);

void CollideSortedObjectLists(ObjectList *list1,ObjectList *list2);
void CollideSortedObjectListsExactly(ObjectList *list1,ObjectList *list2);
void CollideSortedObjectListsWithCallback(ObjectList *list1,ObjectList *list2,CollisionCallbackFunction callback);

int CollideObjectListAgainstSquareWithPower(ObjectList *list,int32_t x,int32_t y,int32_t radius,int totalpower);
int CollideObjectListAgainstCircleWithPower(ObjectList *list,int32_t x,int32_t y,int32_t radius,int totalpower);

//void ClipObjectList(ObjectList *self,int32_t left,int32_t right,int32_t top,int32_t bottom);
void ClipObjectListAgainstScreen(ObjectList *self,int left,int right,int top,int bottom);
void DrawObjectList(ObjectList *self,Bitmap *screen);

static inline bool IsObjectListEmpty(ObjectList *self)
{
	return self->first==NULL;
}





struct ObjectIterator
{
	Object **linktocurrent;
	Object *expectednext;
};

static inline void InitializeObjectIteratorForList(ObjectIterator *self,ObjectList *list)
{
	self->linktocurrent=&list->first;
	self->expectednext=list->first;
}

static inline void InitializeObjectIteratorAsClone(ObjectIterator *self,ObjectIterator *other)
{
	*self=*other;
}

static inline void RemoveCurrentObject(ObjectIterator *self)
{
	Object *current=*self->linktocurrent;
	if(!current) return;
	*self->linktocurrent=current->next;
}

static inline bool CurrentObjectWasRemoved(ObjectIterator *self)
{
	Object *supposedcurrent=*self->linktocurrent;
	if(!supposedcurrent) return true;
	return supposedcurrent->next!=self->expectednext;
}

static inline Object *NextObject(ObjectIterator *self)
{
	Object *supposedcurrent=*self->linktocurrent;

	// Check if the current object is valid.
	if(!CurrentObjectWasRemoved(self))
	{
		// Current object is valid. Update link from last and expected next, and then return the next object.
		if(!supposedcurrent->next) return NULL; // We have reached the final object.
		self->linktocurrent=&supposedcurrent->next;
		self->expectednext=supposedcurrent->next->next;
		return supposedcurrent->next;
	}
	else
	{
		// Current object is invalid. (Iterator has not started, or the current object was removed.)
		// Return the object pointed at by the previous link instead.
		if(!supposedcurrent) return NULL; // List was empty, or we were at the final object, and it was removed.
		self->expectednext=supposedcurrent->next;
		return supposedcurrent;
	}
}

#endif
