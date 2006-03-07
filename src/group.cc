//
// $Id$
//	
// Laidout, for laying out
// Copyright (C) 2004-2006 by Tom Lechner
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Please consult http://www.laidout.org about where to send any
// correspondence about this software.
//
/****************** group.cc ********************/

#include "group.h"
#include "drawdata.h"
#include <lax/refcounter.h>
#include <lax/strmanip.h>

using namespace LaxFiles;

using namespace Laxkit;
extern RefCounter<SomeData> datastack;

#include <iostream>
using namespace std;
//--------------------- Group ------------------------------

/*! \class Group
 * \brief Basically a group of objects
 * 
 * You must not access objs straight. Always go through the n(), e(), etc.
 * This is so the local in the stack can be utilized to either delete or
 * checkin and out of datastack.
 * 
 * ***perhaps migrate Group to GroupData class of Laxkit?
 */
/*! \fn int Group::findindex(Laxkit::SomeData *d)
 * \brief Just does: return objs.findindex(d);
 */
//class Group : public ObjectContainer,
//			  virtual public Laxkit::SomeData 
//			  
//{
// protected:
//	Laxkit::PtrStack<Laxkit::SomeData> objs;
// public:
//	int locked, visible, prints;
//	Group() { locked=0; visible=prints=1; }
//	virtual ~Group();
//	virtual const char *whattype() { return "Group"; }
//	virtual Laxkit::SomeData *findobj(Laxkit::SomeData *d,int *n=NULL);
//	virtual int findindex(Laxkit::SomeData *d) { return objs.findindex(d); }
//	virtual int push(Laxkit::SomeData *obj,int local);
//	virtual int pushnodup(Laxkit::SomeData *obj,int local);
//	virtual int remove(int i);
//	virtual int popp(Laxkit::SomeData *d,int *local=NULL);
//	virtual void flush();
//	virtual void swap(int i1,int i2) { objs.swap(i1,i2); }
//	virtual void slide(int i1,int i2);
//	virtual void dump_out(FILE *f,int indent);
//	virtual void dump_in_atts(LaxFiles::Attribute *att);
//	virtual int pointin(flatpoint pp,int pin=1);
//	virtual void FindBBox();
//	//virtual int contains(SomeData *d,FieldPlace &place);
//	//virtual Laxkit::SomeData *getObject(FieldPlace &place,int offset=0);
//	//virtual int nextObject(FieldPlace &place, FieldPlace &first, int curlevel, Laxkit::SomeData **d=NULL);
//	virtual Laxkit::SomeData *e(int i);
//	virtual int n() { return objs.n; }
//	virtual Laxkit::anObject *object_e(int i) { return e(i); }
//};

//! Destructor, calls flush() to checkin any datastack objects.
Group::~Group() 
{
	flush();
}

//! Append all the bboxes of the objects.
void Group::FindBBox()
{
	maxx=minx-1; maxy=miny-1;
	if (!objs.n) return;
	for (int c=0; c<objs.n; c++) {
		addtobounds(objs.e[c]->m(),objs.e[c]);
	}
}

//------------------remove: (now in ObjectContainer
////! Step to next obect in group tree, and modify place to point to that next obj.
///*!
// * On a successful nextObject, 1 is returned, and place points to it.
// * If d!=NULL, then make *d point to what place refers to.
// * If the new place==first after change, then return 0. If we have otherwise already stepped
// * through all elements of this group, then return -2. d in these cases is set to NULL.
// * Note that place is still modified.
// *
// * Steps through from highest index to lowest. This is because the highest
// * index is usually seen first on screen, and stepping down goes to the ones
// * beneath. If this searches on a first-related level, then searching goes down
// * to 0, then starts at max, goes down to first.
// *
// * If place points to an invalid position, -1 is returned, and place is truncated
// * to where the error occurs.
// *
// * This Group must correspond to index curlevel in place. For instance, if place is a list such as
// * {obj0.spread=1, obj1.spage=4, obj2.layer=0, obj3.obj=10, obj4.obj=15}, and *this is alleged to be 
// * object 3(==index 0 of the parent object), then curlevel must be 3, and the next link
// * in the chain is this->e(10).
// *
// */
//int Group::nextObject(FieldPlace &place, FieldPlace &first, int curlevel, Laxkit::SomeData **d)//d=NULL
//{
//	if (!objs.n || curlevel<0) return 0;
//	if (place.n()-1<curlevel) {
//		if (d) *d=NULL;
//		return -1; //error! bad place
//	}
//	
//	 // if curlevel and levels indexed below in place correspond to first, then
//	 // a special check must be made when tripping through the objects, so must
//	 // figure out if place up to curlevel-1 corresponds to first
//	int atstartlevel=0;
//	int c,i;
//	if (first.n()) {
//		for (c=curlevel-1; c>0; c--) if (place.e(c)!=first(c)) break;
//		if (c<0) atstartlevel=1;
//	} //else don't care about first, any decrementing below 0 terminates the nextObject
//		
//	 
//	if (curlevel==place.n()-1) {
//		 // current position does not refer to subobjects
//		 // 
//		i=place.e(curlevel)-1;
//		
//		 // so i is the decremented (next) position
//		 // If this i refers to an element of first, then we have already wrapped
//		 // around, so return 0. Also, if i goes below 0 and we are not at an element
//		 // of first, then also we are done stepping through this group (return -2).
//		if (i==-1 && !(atstartlevel && 0==first.e(curlevel))) {
//			 // all done with this bunch, but not at first.. this extra check
//			 // for first match is because the group might only have 1 object and that 1
//			 // is a first element. That case is dealt with below.
//			place.pop();
//			if (d) *d=NULL;
//			return -2;
//		}
//		if (i==-2) { // error! place.e(curlevel) had invalid spec
//			if (d) *d=NULL;
//			return -1;
//		}
//		if (i<0) i=objs.n()-1; // we are at an element of first, so continue searching from top
//		place.e(curlevel,i);
//		if (atstartlevel && i==first.e(curlevel)) {
//			 // is at first, so assume subobjects have already been stepped through
//			place.pop();
//			return 0;
//		} // else is ok to be at this object, so continue.
//		
//		 // if objs.e(i) is group, then setup place with the top object: obj->subobj->subobj->etc.
//		Group *gg,*g; 
//		gg=objs.e[i];
//		g=dynamic_cast<Group *>(gg);
//		while (g && g->n()) {
//			place.push(i);
//			i=g->n()-1;
//			gg=g->e(i);
//			g=dynamic_cast<Group *>(gg);
//		}
//		if (d) *d=gg;
//		return 1; //success!
//	}
//
//	 // else if (curlevel!=place.n()-1) then:
//	 // place refers to a subobject then try to make the subobject increment place:
//
//	i=place.e(curlevel);
//	if (i<0 || i>=objs.n) { // error! place invalid index
//		while (curlevel!=place.n()-1) place.pop();
//		if (d) *d=NULL;
//		return -1;
//	}
//	Group *g=dynamic_cast<Group *g>(objs.e(i));
//	if (!g) { //was invalid spec, expected a group
//		while (curlevel!=place.n()-1) place.pop();
//		if (d) *d=NULL;
//		return -1;
//	}
//	
//	i=g->nextObject(place, first, curlevel+1);
//	if (i==-2) { 
//		 // was all done with sublevels, so must inc at this level
//		 // group function calling itself is perhaps a silly way to do it,
//		 // should rework this function so the 'if (curlevel==place.n()-1)' block
//		 // comes second?
//		return nextObject(place,first,curlevel);
//	}
//	if (i==0) { // first reached in sublevel, so start iterating at curlevel..
//		return nextObject(place,first,curlevel);
//	}
//	if (i==1) return 1; //successful incrementing
//	return -1; //must be error to be here, so return, do nothing more
//}
//
////! Retrieve the object at position place.
///*! If offset>0, then check position in place starting at index offset.
// *
// * If there is no object at that position, then return NULL;
// */
//Laxkit::SomeData *Group::getObject(FieldPlace &place, int offset)
//{
//	if (place.n()>=offset) return NULL;
//	if (place.e(offset)>=objs.n() || place.e(offset)<0) return NULL;
//	if (place.n()==offset+1) { // place refers to this Group
//		return objs.e(place.e(offset));
//	}
//	 // must be a subelement
//	Group *g=dynamic_cast<Group *>(objs.e(place.e(offset)));
//	if (!g) return NULL;
//	return g->getObject(place,offset+1);
//}
//
////! Return whether d is in Group somewhere.
///*! Returns place.n if the object is found, which is how deep down the
// * object is. Otherwise return 0.
// *
// * The position gets pushed onto place starting at place.n.
// */
//int Group::contains(SomeData *d,FieldPlace &place)
//{
//	int c;
//	int n=place.n;
//	for (c=0; c<objs.n; c++) {
//		if (d==objs.e[c]) {
//			place.push(c,n);
//			return place.n;
//		}
//	}
//	Group *g;
//	for (c=0; c<objs.n; c++) {
//		g=dynamic_cast<Group *>(objs.e[c]);
//		if (g && g->contains(d,place)) {
//			place.push(c,n);
//			return place.n;
//		}
//	}
//	return 0;
//}
//---^^^ in ObjectContainer, remove if tests ok...

//! Check the point against all objs.
int Group::pointin(flatpoint pp,int pin)
{ 
	if (!objs.n) return 0;
	flatpoint p(((pp-origin())*xaxis())/(xaxis()*xaxis()), 
		        ((pp-origin())*yaxis())/(yaxis()*yaxis()));
	for (int c=0; c<objs.n; c++) {
		if (objs.e[c]->pointin(p,pin)) return 1;
	}
	return 0;
}

//! Push obj onto the stack. (new objects only!)
/*! If local==1 then obj is delete'd when remove'd from objs.
 * Otherwise the obj is to be a datastack object, and is pushed, adding 1 count.
 * The object should not already exist on the datastack. No previous existence
 * check is done here. For that, use pushnodup(). (***though note that
 * datastack.push(...) is actaully a pushnodup)
 *
 * \todo *** redo datastack! make count in the object itself, this would
 * remove the problem of pushing the same object..
 */
int Group::push(Laxkit::SomeData *obj,int local)
{
	if (!obj) return -1;
	if (local==0) datastack.push(obj,1,obj->object_id,1); //this is pushondup'ing
	return objs.push(obj,(local==1?1:0));
}

//! Push obj onto the stack only if it is not already there.
/*! If the item is already on the stack, then its count in datastack
 * is incremented, otherwise the datastack count is not incremented.
 * If local==1 then obj is delete'd when remove'd from this->objs.
 */
int Group::pushnodup(Laxkit::SomeData *obj,int local)
{
	if (!obj) return -1;
	int c=objs.pushnodup(obj,(local==1?1:0));
	//if was there, then do not increase count, otherwise:
	if (c<0 && local==0) datastack.push(obj,1,obj->object_id,1);
	return c;
}

//! Pop d, but do not checkin to datastack.
/*! Returns 1 for item popped, 0 for not.
 */
int Group::popp(Laxkit::SomeData *d,int *local)
{
	return objs.popp(d,local);
}

//! Remove item with index i. Return 1 for item removed, 0 for not.
int Group::remove(int i)
{
	int local;
	SomeData *obj;
	objs.pop(obj,i,&local);
	if (obj) {
		if (local==0) datastack.checkin(obj);
		else if (local==1) delete obj;
		else if (local==2) delete[] obj;
		return 1;
	}
	return 0;
}

//! Pops item i1, then pushes it so that it is in position i2. 
/*! Return 1 for slide happened, else 0.
 */
int Group::slide(int i1,int i2)
{
	if (i1<0 || i1>=objs.n || i2<0 || i2>=objs.n) return 0;
	int local;
	SomeData *obj;
	objs.pop(obj,i1,&local);
	objs.push(obj,local,i2);
	return 1;
}

//! Checkin any datastack objects, then call objs.flush().
void Group::flush()
{
	for (int c=0; c<objs.n; c++) {
		if (objs.islocal[c]==0) datastack.checkin(objs.e[c]);
	}
	objs.flush();
}

//! Return object with index i in stack.
Laxkit::SomeData *Group::e(int i)
{
	if (i<0 || i>=objs.n) return NULL;
	return objs.e[i];
}

/*! Recognizes locked, visible, prints, then tries to parse elements...
 * Discards all else.
 * The objs should have been flushed before coming here.
 */
void Group::dump_in_atts(LaxFiles::Attribute *att)
{
	locked=visible=prints=0;
	char *name,*value;
	for (int c=0; c<att->attributes.n; c++)  {
		name=att->attributes.e[c]->name;
		value=att->attributes.e[c]->value;
		if (!strcmp(name,"locked")) {
			locked=BooleanAttribute(value);
		} else if (!strcmp(name,"visible")) {
			visible=BooleanAttribute(value);
		} else if (!strcmp(name,"prints")) {
			prints=BooleanAttribute(value);
		} else if (!strcmp(name,"object")) {
			int n;
			char **strs=splitspace(value,&n);
			if (strs) {
				// could use the number as some sort of object id?
				// currently out put was like: "object 2 ImageData"
				//***strs[0]==that id
				SomeData *data=newObject(n>1?strs[1]:(n==1?strs[0]:NULL));
				if (data) {
					data->dump_in_atts(att->attributes[c]);
					push(data,0);
				}
				deletestrs(strs,n);
			} else {
				cout <<"*** readin blank object for Group..."<<endl;
			}
		} else { 
			cout <<"Group dump_in:*** unknown attribute!!"<<endl;
		}
	}
}

//! Write out the objects.
void Group::dump_out(FILE *f,int indent)
{
	char spc[indent+1]; memset(spc,' ',indent); spc[indent]='\0';
	if (locked) fprintf(f,"%slocked\n",spc);
	if (visible) fprintf(f,"%svisible\n",spc);
	if (prints) fprintf(f,"%sprints\n",spc);
	for (int c=0; c<objs.n; c++) {
		fprintf(f,"%sobject %d %s\n",spc,c,objs.e[c]->whattype());
		objs.e[c]->dump_out(f,indent+2);
	}
}
//! Find d somewhere within this (it can be this also). Searches in subobjects too.
/*! if n!=NULL, then increment n each time findobj is called. So say an object
 * is in group->group->group->objs, then n gets incremented 3 times. If object
 * is in this group, then do not increment n at all.
 *
 * Return the object if it is found, otherwise NULL.
 */
Laxkit::SomeData *Group::findobj(Laxkit::SomeData *d,int *n)
{
	if (d==this) return d;
	int c;
	for (c=0; c<objs.n; c++) {
		if (objs.e[c]==d) return d;
	}
	Group *g;
	for (c=0; c<objs.n; c++) {
		g=dynamic_cast<Group *>(objs.e[c]);
		if (!g) continue;
		if (n) *n++;
		if (g->findobj(d,n)) {
			return d;
		}
		if (n) *n--;
	}
	return NULL;
}


