#ifndef WED_NAVAID_H
#define WED_NAVAID_H

#include "WED_GISPoint_Heading.h"
#include "NavIO.h"

struct	AptGate_t;

class	WED_NavAid : public WED_GISPoint_Heading {

    DECLARE_PERSISTENT(WED_NavAid)

public:

    void	SetType(int		navaid_type);
    int		GetType() const;

    void	Import(const NavIls_t& x, void (* print_func)(void *, const char *, ...), void * ref);
    void	Export(		 NavIls_t& x) const;

    virtual const char *	HumanReadableType(void) const {
        return "Navigation Aid";
    }

private:

    WED_PropIntEnum			navaid_type;

};

#endif /* WED_NAVAID_H */