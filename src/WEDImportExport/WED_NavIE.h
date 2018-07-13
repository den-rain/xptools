#ifndef WED_NavIE_H
#define WED_NavIE_H

#include "NavDefs.h"

class	WED_Thing;
class	WED_Archive;
class	IResolver;
class	WED_Document;
class	WED_MapPane;
class	WED_Airport;

int WED_CanImportNavData(IResolver * resolver);
void WED_DoImportNavData(WED_Document * resolver, WED_Archive * archive, WED_MapPane * pane);

int WED_CanExportNavData(IResolver * resolver);
void WED_DoExportNavData(WED_Document * resolver, WED_MapPane * pane);


#endif //WED_NavIE_H