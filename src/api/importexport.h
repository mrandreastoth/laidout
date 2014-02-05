//
// $Id$
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
// Copyright (C) 2009 by Tom Lechner
//
#ifndef IMPORTEXPORTFUNC_H
#define IMPORTEXPORTFUNC_H

#include <lax/errorlog.h>

#include "../styles.h"


namespace Laidout {


ObjectDef *makeImportObjectDef();
int ImportFunction(ValueHash *context, 
					 ValueHash *parameters,
					 Value **value_ret,
					 Laxkit::ErrorLog &log);



ObjectDef *makeExportObjectDef();
int ExportFunction(ValueHash *context, 
					 ValueHash *parameters,
					 Value **value_ret,
					 Laxkit::ErrorLog &log);


} // namespace Laidout

#endif 



