//
// Laidout, for laying out
// Please consult http://www.laidout.org about where to send any
// correspondence about this software.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// For more details, consult the COPYING file in the top directory.
//
// Copyright (C) 2017 by Tom Lechner
//


//#include <glib-2.0/glib.h>
#include <gegl-0.3/gegl.h>

#include "../language.h"
#include "../interfaces/nodeinterface.h"
#include "geglnodes.h"


#include <iostream>
#define DBG


using namespace std;


Laidout::PluginBase *GetPlugin()
{
	return new Laidout::GeglNodesPluginNS::GeglNodesPlugin();
}

namespace Laidout {
namespace GeglNodesPluginNS {


Laxkit::MenuInfo *GetGeglOps();


//-------------------------------- GeglLaidoutNode --------------------------

class GeglLaidoutNode : public Laidout::NodeBase
{
  protected:
	MenuInfo *op;

  public:
	static Laxkit::SingletonKeeper op_menu;

	char *operation;
	GeglNode *gegl;

	GeglLaidoutNode(const char *oper);
	virtual ~GeglLaidoutNode();

	virtual int SetOperation(const char *oper);
	virtual int UpdateProperties();
};

Laxkit::SingletonKeeper GeglLaidoutNode::op_menu;


GeglLaidoutNode::GeglLaidoutNode(const char *oper)
{
	operation = NULL;
	op = NULL; //don't delete this! it lives within op_menu
	SetOperation(oper);

	gegl = gegl_node_new();
}

GeglLaidoutNode::~GeglLaidoutNode()
{
	delete[] operation;
	g_object_unref (gegl);
}

int GeglLaidoutNode::UpdateProperties()
{
	//***
	return 1;
}

/*! Set to op, and grab the spec for the properties.
 *
 * Return 0 for success, or nonzero for error.
 */
int GeglLaidoutNode::SetOperation(const char *oper)
{
	if (oper==NULL) return 1;
	if (oper!=NULL && operation!=NULL && !strcmp(oper, operation)) return 0;

	MenuInfo *menu = GetGeglOps();

	int i = menu->findIndex(oper);
	if (i<0) return 2; //unknown op!

	MenuItem *opitem = menu->e(i);

	if (opitem->info == -1) { 
		guint n_props = 0;

		GParamSpec **specs = gegl_operation_list_properties (oper, &n_props);

		opitem->info = n_props;

		menu->SubMenu("", i);

		 //add properties to the op list: name, type, nickname, blurb
		for (unsigned int c2=0; c2<n_props; c2++) {
			const gchar *nick  = g_param_spec_get_nick  (specs[c2]);
			const gchar *blurb = g_param_spec_get_blurb (specs[c2]);
			const gchar *ptype = g_type_name(specs[c2]->value_type);

			DBG cerr << "    prop: "<<specs[c2]->name <<','<< ptype <<','<< (nick?nick:"(no nick)")<<','<< (blurb?blurb:"(no blurb)")<<endl;

			//if (!strcmp(ptype, "gboolean")) ptype = "boolean";
			//else if (!strcmp(ptype, "gint")) ptype = "int";
			//else if (!strcmp(ptype, "gdouble")) ptype = "real";
			//else if (!strcmp(ptype, "gchararray")) ptype = "string";
			//else if (!strcmp(ptype, "GeglCurve")) ptype = "CurveInfo";
			//else if (!strcmp(ptype, "GeglColor")) ptype = "";
			//else if (!strcmp(ptype, "")) ptype = "";
			//else if (!strcmp(ptype, "")) ptype = "";
			//else if (!strcmp(ptype, "")) ptype = "";
			//else {
			//	DBG cerr <<" Warning! unknown gegl property type "<<ptype<<endl;
			//}

			menu->AddItem(specs[c2]->name, 0, -1);
			menu->AddDetail(ptype, NULL);
			menu->AddDetail(nick ? nick : "", NULL);
			menu->AddDetail(blurb ? blurb : "", NULL);

			//gchar **keys = gegl_operation_list_property_keys (operations[i], specs[c2]->name, &nn);
			//
			//for (int c3=0; c3<nn; c3++) {
			//	const gchar *val = gegl_param_spec_get_property_key(specs[c2], keys[c3]);
			//	printf("      key: %s, val: %s\n", keys[c3], val);
			//}
			//
			//g_free(keys);
		}

		if (specs) g_free(specs);

		menu->EndSubMenu();
	}

	makestr(operation, oper);
	makestr(type, operation);
	makestr(Name, operation);

	op = opitem->GetSubmenu();
	MenuItem *prop;
	const char *type;
	Value *v;
	for (int c=0; c<op->n(); c++) {
		prop = op->e(c);
		type = prop->GetString(1);
		v = NULL;

		if (type) {
			if (     !strcmp(type, "gboolean"  )) v = new BooleanValue(0);
			else if (!strcmp(type, "gint"      )) v = new IntValue();
			else if (!strcmp(type, "gdouble"   )) v = new DoubleValue();
			else if (!strcmp(type, "gchararray")) v = new StringValue();
			else if (!strcmp(type, "GeglColor" )) v = new ColorValue("#000");
		}
		AddProperty(new NodeProperty(NodeProperty::PROP_Input, true, prop->name, v,1, prop->GetString(2),prop->GetString(3)));
	}

	AddProperty(new NodeProperty(NodeProperty::PROP_Input,  true, "In",  NULL,1, _("In") ,NULL));
	AddProperty(new NodeProperty(NodeProperty::PROP_Output, true, "Out", NULL,1, _("Out"),NULL));

	return 0;
}



//------------------------------- Gegl funcs ----------------------------

/*! Query what gegl ops are available. Properties are not scanned here, only operations.
 */
Laxkit::MenuInfo *GetGeglOps()
{
	MenuInfo *menu = dynamic_cast<MenuInfo*>(GeglLaidoutNode::op_menu.GetObject());
	if (menu) return menu;

	menu = new MenuInfo;
	GeglLaidoutNode::op_menu.SetObject(menu, 1);


    guint   n_operations;
    gchar **operations = gegl_list_operations (&n_operations);
	MenuItem *item;

	DBG cerr <<"gegl operations:"<<endl;

    for (unsigned int i=0; i < n_operations; i++) { 
        DBG cerr << "  " << (operations[i]?operations[i]:"?") << endl;
		menu->AddItem(operations[i], i, -1); 
		item = menu->e(menu->n()-1);
		item->info = -1;
    }

    g_free (operations); 

	return menu;
}

Laxkit::anObject *newGeglLaidoutNode(int p, Laxkit::anObject *ref)
{
	MenuInfo *menu = GetGeglOps();
	MenuItem *op   = menu->e(p);
	if (!op) return NULL;

	GeglLaidoutNode *node = new GeglLaidoutNode(op->name);
	return node;
}

void RegisterNodes(Laxkit::ObjectFactory *factory)
{
	MenuInfo *menu = GetGeglOps();

	char str[200];
	for (int c=0; c<menu->n(); c++) {
		sprintf(str, "Gegl/%s", menu->e(c)->name);
		factory->DefineNewObject(getUniqueNumber(), str, newGeglLaidoutNode, NULL, c);
	}
}



//-------------------------------- GeglNodesPlugin --------------------------

/*! class GeglNodesPlugin
 *
 */

GeglNodesPlugin::GeglNodesPlugin()
{
	DBG cerr <<"GeglNodesPlugin constructor"<<endl;
}

GeglNodesPlugin::~GeglNodesPlugin()
{
    gegl_exit ();
}

unsigned long GeglNodesPlugin::WhatYouGot()
{
	 //or'd list of PluginBase::PluginBaseContents
	return PluginBase::PLUGIN_Nodes;
		//| PLUGIN_Panes
		//| PLUGIN_ImageImporters
		//| PLUGIN_TextImporters
		//| PLUGIN_ImportFilters
		//| PLUGIN_ExportFilters
		//| PLUGIN_DrawableObjects
		//| PLUGIN_Tools
		//| PLUGIN_Configs
		//| PLUGIN_Resources
		//| PLUGIN_Impositions
		//| PLUGIN_Actions
		//| PLUGIN_Interpreters
		//| PLUGIN_CalcModules
	  ;
}

const char *GeglNodesPlugin::PluginName()
{
	return _("Gegl Nodes");
}

const char *GeglNodesPlugin::Version()
{
	return "0.1";
}

/*! Return localized description.
 */
const char *GeglNodesPlugin::Description()
{
	return _("Provides gegl image functionality to nodes.");
}

const char *GeglNodesPlugin::Author()
{
	return "Laidout";
}

const char *GeglNodesPlugin::ReleaseDate()
{
	return "2017 12 01";
}

const char *GeglNodesPlugin::License()
{
	return "GPL3";
}


/*! Install stuff.
 */
int GeglNodesPlugin::Initialize()
{
	DBG cerr << "GeglNodesPlugin initializing..."<<endl;

	if (initialized) return 0;

	// do stuff

	 //initialize gegl
	gegl_init (NULL,NULL); //(&argc, &argv);


	ObjectFactory *node_factory = Laidout::NodeGroup::NodeFactory(true);

	RegisterNodes(node_factory);

	DBG cerr << "GeglNodesPlugin initialized!"<<endl;
	initialized = 1;

	return 0;
}



} //namespace GeglNodesPluginNS
} //namespace Laidout


