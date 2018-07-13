#include "NavIO.h"
#include "MemFileUtils.h"

string ReadNavFile(const char * inFileName, NavData_t & outNavData) {
    MFMemFile * f = MemFile_Open(inFileName);
    if (f == NULL)
        return string("memfile_open failed");

    string err = ReadNavFileMem(MemFile_GetBegin(f), MemFile_GetEnd(f), outNavData);
    MemFile_Close(f);
    return err;
}

string ReadNavFileMem(const char * inBegin, const char * inEnd, NavData_t & outNavData) {
    MFTextScanner * s = TextScanner_OpenMem(inBegin, inEnd);
    string ok;

    int ln = 0;
    int vers = 0;

    if (TextScanner_IsDone(s))
        ok = string("File is empty.");
    if (ok.empty()) {
        string app_win;
        if (TextScanner_FormatScan(s, "T", &app_win) != 1)
            ok = "Invalid header";
        if (app_win != "a" && app_win != "A" && app_win != "i" && app_win != "I")
            ok = string("Invalid header:") + app_win;
        TextScanner_Next(s);
        ++ln;
    }
    if (ok.empty()) {
        if (TextScanner_FormatScan(s, "i", &vers) != 1)
            ok = "Invalid version";
        if (vers != 1100) {
            if (vers > 1100)
                ok = "Format is newer than supported by this version of WED";
            else
                ok = "Illegal version";
        }
        TextScanner_Next(s);
        ++ln;
    }

    bool forceDone = false;
    while (ok.empty() && !TextScanner_IsDone(s) && !forceDone) {
        eNavRowCode rec_code;
        if (TextScanner_FormatScan(s, "i", &rec_code) != 1) {
            TextScanner_Next(s);
            ++ln;
            continue;
        }
        double p1x, p1y;
        switch (rec_code) {
            case nav_ndb: break;
            case nav_vor: break;
            case nav_ils:
                outNavData.ils_list.push_back(NavIls_t());
                if (TextScanner_FormatScan(s, "iddiiifTTTTT",
                                           &rec_code,
                                           &p1x,
                                           &p1y,
                                           &outNavData.ils_list.back().elev,
                                           &outNavData.ils_list.back().freq,
                                           &outNavData.ils_list.back().max_range,
                                           &outNavData.ils_list.back().bearing,
                                           &outNavData.ils_list.back().id,
                                           &outNavData.ils_list.back().apt_icao,
                                           &outNavData.ils_list.back().icao_region,
                                           &outNavData.ils_list.back().rwy,
                                           &outNavData.ils_list.back().name) != 12) {

                    ok = "Illegal ILS line";
                }
                outNavData.ils_list.back().row_code = rec_code;
                outNavData.ils_list.back().location = POINT2(p1x, p1y);
                break;
            case nav_loc: break;
            case nav_gs: 
                outNavData.gs_list.push_back(NavGs_t());
                if (TextScanner_FormatScan(s, "iddiiifTTTTT",
                                           &rec_code,
                                           &p1x,
                                           &p1y,
                                           &outNavData.gs_list.back().elev,
                                           &outNavData.gs_list.back().freq,
                                           &outNavData.gs_list.back().max_range,
                                           &outNavData.gs_list.back().angle_and_bearing,
                                           &outNavData.gs_list.back().id,
                                           &outNavData.gs_list.back().apt_icao,
                                           &outNavData.gs_list.back().icao_region,
                                           &outNavData.gs_list.back().rwy,
                                           &outNavData.gs_list.back().name) != 12) {

                    ok = "Illegal GS line";
                }
                outNavData.gs_list.back().row_code = rec_code;
                outNavData.gs_list.back().location = POINT2(p1x, p1y);
                break;
            case nav_om: break;
            case nav_mm: break;
            case nav_im: break;
            case nav_vor_ils_vortac_dme: break;
            case nav_ndb_alone_dme: break;
            default:;
        }
        TextScanner_Next(s);
        ++ln;
    }

    TextScanner_Close(s);
    if (!ok.empty())
    {
        char buf[50];
        sprintf(buf," (Line %d)",ln);
        ok += buf;
    }

    return ok;
}
