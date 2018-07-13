#ifndef NAVDEFS_H
#define NAVDEFS_H


#include "CompGeomDefs2.h"
#include <vector>

enum eNavRowCode {
    nav_ndb = 2,
    nav_vor = 3,
    nav_ils = 4,
    nav_loc = 5,
    nav_gs = 6,
    nav_om = 7,
    nav_mm = 8,
    nav_im = 9,
    nav_vor_ils_vortac_dme = 12,
    nav_ndb_alone_dme = 13
};

struct NavNdb_t {
    eNavRowCode row_code;
    Point2 location;
    int elev;// MSL
    int freq;// KHz
    int max_range;
    string id;
    string terminal_region;// Airport code for terminal NDBs, ENRT otherwise
    // Must be region code according to ICAO document No. 7910
    // For terminal NDBs, the region code of the airport is used
    string icao_region;
    string name;
};
typedef vector<NavNdb_t> NavNdbVector;

struct NavIls_t {
    eNavRowCode row_code;
    Point2 location;
    int elev;// MSL
    int freq;// KHz
    int max_range;
    float bearing;// Localizer bearing in true degrees
    string id;
    string apt_icao;
    string icao_region;// the region code of the airport is used
    string rwy;
    string name;
};
typedef vector<NavIls_t> NavIlsVector;

struct NavLoc_t {
    eNavRowCode row_code;
    Point2 location;
    int elev;// MSL
    int freq;// KHz
    int max_range;
    float bearing;// Localizer bearing in true degrees
    string id;
    string apt_icao;
    string icao_region;// the region code of the airport is used
    string rwy;
    string name;
};
typedef vector<NavLoc_t> NavLocVector;

struct NavGs_t {
    eNavRowCode row_code;
    Point2 location;
    int elev;// MSL
    int freq;// KHz
    int max_range;
    float angle_and_bearing;// Associated localizer bearing in true degrees prefixed by glideslope angle
    string id;
    string apt_icao;
    string icao_region;// the region code of the airport is used
    string rwy;
    string name;
};
typedef vector<NavGs_t> NavGsVector;

struct NavOm_t {
    eNavRowCode row_code;
    Point2 location;
    int elev;// MSL
    float bearing;// Associated localizer bearing in true degrees (also known as “minor axis”)
    string ils_loc_id;// Associated approach identifier
    string apt_icao;
    string icao_region;// the region code of the airport is used
    string rwy;
    string name;
};
typedef vector<NavOm_t> NavOmVector;

struct NavMm_t {
    eNavRowCode row_code;
    Point2 location;
    int elev;// MSL
    float bearing;// Associated localizer bearing in true degrees (also known as “minor axis”)
    string ils_loc_id;// Associated approach identifier
    string apt_icao;
    string icao_region;// the region code of the airport is used
    string rwy;
    string name;
};
typedef vector<NavMm_t> NavMmVector;

struct NavIm_t {
    eNavRowCode row_code;
    Point2 location;
    int elev;// MSL
    float bearing;// Associated localizer bearing in true degrees (also known as “minor axis”)
    string ils_loc_id;// Associated approach identifier
    string apt_icao;
    string icao_region;// the region code of the airport is used
    string rwy;
    string name;
};
typedef vector<NavIm_t> NavImVector;

struct NavData_t {
    NavIlsVector ils_list;
    NavGsVector gs_list;
};

#endif //NAVDEFS_H