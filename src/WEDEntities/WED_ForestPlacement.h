/*
 * Copyright (c) 2008, Laminar Research.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef WED_ForestPlacement_H
#define WED_ForestPlacement_H

#include "IHasResource.h"
#include "WED_GISPolygon.h"

enum {
	dsf_fill_area = 0,
	dsf_fill_line = 1,
	dsf_fill_points = 2
};

class	WED_ForestPlacement : public WED_GISPolygon, public IHasResource {

DECLARE_PERSISTENT(WED_ForestPlacement)

public:

			double		GetDensity(void) const;
			void		SetDensity(double h);

	virtual void		GetResource(	  string& r) const;
	virtual void		SetResource(const string& r);

			int			GetFillMode(void) const;
#if AIRPORT_ROUTING			
			void		SetFillMode(int mode);
#endif			

	virtual const char *	HumanReadableType(void) const { return "Forest"; }

protected:

	virtual	bool		IsInteriorFilled(void) const { return GetFillMode() == dsf_fill_area; }

private:

	WED_PropDoubleText		density;
	WED_PropStringText		resource;
#if AIRPORT_ROUTING	
	WED_PropIntEnum			fill_mode;
#endif	

};


#endif /* WED_ForestPlacement_H */
