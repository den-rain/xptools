#include "WED_NavIE.h"

#include "NavIO.h"
#include "WED_ToolUtils.h"
#include "WED_Thing.h"
#include "WED_Messages.h"
#include "WED_Document.h"
#include "WED_MapPane.h"

//Utils
#include "PlatformUtils.h"
#include "FileUtils.h"
#include "STLUtils.h"

#include "WED_UIDefs.h"

int WED_CanImportNavData(IResolver * resolver) {
    return 1;
}

void WED_DoImportNavData(WED_Document * resolver, WED_Archive * archive, WED_MapPane * pane) {
    char path[1024];
    strcpy(path,"earth_nav.dat");
    NavData_t nav_data;
    if (GetFilePathFromUser(getFile_Open, "Import earth_nav.dat...", "Import", FILE_DIALOG_IMPORT_NAVDAT, path, sizeof(path))) {

        
        string result = ReadNavFile(path, nav_data);
        if (!result.empty()) {
            string msg = string("The earth_nav.dat file '") + *path + string("' could not be imported:\n") + result;
            DoUserAlert(msg.c_str());
            return;
        }
    }
    else {
        string str = "Cannot open earth_nav.dat: " + string(path);
        DoUserAlert(str.c_str());
        return;
    }

    // importing into wed world
    WED_Thing * wrl = WED_GetWorld(resolver);
    wrl->StartOperation("Import earth_nav.dat");


    wrl->CommitOperation();
    pane->ZoomShowSel();
}

int WED_CanExportNavData(IResolver * resolver) {
    return 1;
}

void WED_DoExportNavData(WED_Document * resolver, WED_MapPane * pane) {
    
}
