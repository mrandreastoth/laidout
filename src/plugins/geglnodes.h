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
#ifndef GEGL_NODES_PLUGIN_H
#define GEGL_NODES_PLUGIN_H


#include "plugin.h"


extern "C" Laidout::PluginBase *GetPlugin();


namespace Laidout {
namespace GeglNodesPluginNS {


class GeglNodesPlugin : public Laidout::PluginBase
{

  public:
	GeglNodesPlugin();
	virtual ~GeglNodesPlugin();

	virtual unsigned long WhatYouGot(); //or'd list of PluginBase::PluginBaseContents

	virtual const char *PluginName();
	virtual const char *Version();
	virtual const char *Description();
	virtual const char *Author();
	virtual const char *ReleaseDate();
	virtual const char *License();
	//virtual const LaxFiles::Attribute *OtherMeta();

	virtual int Initialize(); //install stuff
};

} //namespace GeglNodesPluginNS
} //namespace Laidout


#endif

