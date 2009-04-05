/*
 * Copyright (c) 2004, Laminar Research.
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
#ifndef MESHDEFS_H
#define MESHDEFS_H

#define CGAL_NO_PRECONDITIONS
#ifndef NDEBUG
#define NDEBUG
#endif

#include "ParamDefs.h"
#include "DEMDefs.h"

//class GISFace;

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Filtered_kernel.h>

// make sure our kernel types match

#include "MapDefsCGAL.h"

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/assertions.h>
#include <CGAL/intersections.h>


//typedef	CGAL::Simple_cartesian<double>	                BaseKernel;

//typedef CGAL::Filtered_kernel<BaseKernel> 					FastKernel;

typedef multimap<float, void *, greater<float> >			FaceQueue;	// YUCK - hard cast to avoid snarky problems with forward decls

struct	MeshVertexInfo {
	MeshVertexInfo() : height(0.0), wave_height(1.0) { }
	MeshVertexInfo(const MeshVertexInfo& rhs) :
								height(rhs.height),
								border_blend(rhs.border_blend) {
								normal[0] = rhs.normal[0];
								normal[1] = rhs.normal[1];
								normal[2] = rhs.normal[2]; }
	MeshVertexInfo& operator=(const MeshVertexInfo& rhs) {
								height = rhs.height;
								normal[0] = rhs.normal[0];
								normal[1] = rhs.normal[1];
								normal[2] = rhs.normal[2];
								border_blend = rhs.border_blend; return *this; }

	double					height;					// Height of mesh at this vertex.
	double					wave_height;			// ratio of vegetation to terrain at this vertex.
	float					normal[3];				// Normal - X,Y,Z in OGL coords(!)
	hash_map<int, float>	border_blend;			// blend level for a border of this layer at this triangle!

};

struct	MeshFaceInfo {
	MeshFaceInfo() : terrain(DEM_NO_DATA),feature(NO_VALUE),flag(0), orig_face(NULL) { }
	MeshFaceInfo(const MeshFaceInfo& rhs) :
								terrain(rhs.terrain),
								feature(rhs.feature),
								flag(rhs.flag),
								terrain_border(rhs.terrain_border) {
								normal[0] = rhs.normal[0];
								normal[1] = rhs.normal[1];
								normal[2] = rhs.normal[2];
								orig_face = rhs.orig_face; }


	MeshFaceInfo& operator=(const MeshFaceInfo& rhs) {
								terrain = rhs.terrain;
								feature = rhs.feature;
								flag = rhs.flag;
								terrain_border = rhs.terrain_border;
								normal[0] = rhs.normal[0];
								normal[1] = rhs.normal[1];
								normal[2] = rhs.normal[2];
								orig_face = rhs.orig_face;
								return *this; }

	int				insert_x;
	int				insert_y;
	float			insert_err;
	double			plane_a;
	double			plane_b;
	double			plane_c;

	int				terrain;				// Specific terrain type, e.g. natural converted to a real land use. (This is a .ter enum, NOT a table index btw)
	int				feature;				// General terrain type for this triangle, e.g. terrain_Natural, terrain_Water
	int				flag;					// General purpose, useful for..um...algorithms.
	set<int>		terrain_border;			// All terrains on top of us!
	float			normal[3];				// Tri flat normal - not in final DSF but handy for other sh-t.

	Face_handle		orig_face;				// If a face caused us to get the terrain we did, this is who!

	FaceQueue::iterator	self;					// Queue ref to self!

// BENTODO - clean this up
	float			debug_slope_dem;
	float			debug_slope_tri;
	float			debug_temp;
	float			debug_temp_range;
	float			debug_rain;
	float			debug_heading;
};

typedef	CGAL::Triangulation_vertex_base_with_info_2<MeshVertexInfo, Traits_2>		Vb;
typedef CGAL::Triangulation_face_base_with_info_2<MeshFaceInfo, Traits_2>			Fbi;


typedef	CGAL::Constrained_triangulation_face_base_2<Traits_2, Fbi>				Fb;
typedef	CGAL::Triangulation_data_structure_2<Vb, Fb>								TDS;

//typedef	CGAL::Constrained_Delaunay_triangulation_2<FastKernel, TDS, CGAL::No_intersection_tag>	CDTBase;
typedef	CGAL::Constrained_Delaunay_triangulation_2<FastKernel, TDS, CGAL::Exact_predicates_tag>	CDTBase;

class CDT : public CDTBase {
public:

	typedef hash_map<int, Face_handle>	HintMap;

	static	int			gen_cache_key(void);
			Face_handle locate_cache(const Point& p, Locate_type& lt, int& li, int cache_key) const;
			void		cache_reset(void);
			void		clear(void);


	// SAFE insert...basically when using a fast cartesian kernel, the
	// face locate may return 'face' when it means 'edge' because the
	// set of tests for the march-locate aren't quite specific enough to resolve
	// rounding errors.  (At least that's what I _think_ it is.)  So...
	// We do the locate, check for this case, and try to manually find an edge
	// we're on.  If we do, we fix up the locate info and procede safely.  If not
	// we assert - we would have done that anyway.
#if 1
	Vertex_handle	safe_insert(const Point& p, Face_handle hint);
#else
	Vertex_handle	safe_insert(const Point& p, Face_handle hint)
	{
		int			li;
		Locate_type	lt;
		Face_handle	who = locate(p, lt, li, hint);
		if (lt == FACE && oriented_side(who, p) != CGAL::ON_POSITIVE_SIDE)
		{
			if(lt == FACE && oriented_side(who, p) != CGAL::ON_POSITIVE_SIDE)
			{

				Point	p0(who->vertex(0)->point());
				Point	p1(who->vertex(1)->point());
				Point	p2(who->vertex(2)->point());

				CGAL_triangulation_precondition( orientation(p0, p1, p2) != CGAL::COLLINEAR);

				CGAL::Orientation 	o2 = orientation(p0, p1, p),
									o0 = orientation(p1, p2, p),
									o1 = orientation(p2, p0, p),
									o2b= orientation(p1, p0, p),
									o0b= orientation(p2, p1, p),
									o1b= orientation(p0, p2, p);

				// Collinear witih TWO sides?  Hrmm...should be a vertex.
				if (o1 == CGAL::COLLINEAR && o2 == CGAL::COLLINEAR) { li = 0; lt = VERTEX; }
				if (o2 == CGAL::COLLINEAR && o0 == CGAL::COLLINEAR) { li = 1; lt = VERTEX; }
				if (o0 == CGAL::COLLINEAR && o1 == CGAL::COLLINEAR) { li = 2; lt = VERTEX; }

				// Colinear with a side and positive on the other two - should be on that edge.
				if (o1 == CGAL::COLLINEAR && o2 == CGAL::POSITIVE && o0 == CGAL::POSITIVE) 				{ li = 1; lt = EDGE; }
				if (o2 == CGAL::COLLINEAR && o0 == CGAL::POSITIVE && o1 == CGAL::POSITIVE) 				{ li = 2; lt = EDGE; }
				if (o0 == CGAL::COLLINEAR && o1 == CGAL::POSITIVE && o2 == CGAL::POSITIVE) 				{ li = 0; lt = EDGE; }

				// On negative of a side AND its opposite?  We've got a rounding error.  Call it the edge and go home.
				if (o0 == CGAL::NEGATIVE && o0b == CGAL::NEGATIVE) { li = 0; lt = EDGE; }
				if (o1 == CGAL::NEGATIVE && o1b == CGAL::NEGATIVE) { li = 1; lt = EDGE; }
				if (o2 == CGAL::NEGATIVE && o2b == CGAL::NEGATIVE) { li = 2; lt = EDGE; }

				if (lt == FACE) AssertPrintf("Unable to resolve bad locate.");
			}
		}
		return CDTBase::insert(p, lt, who, li);
	}
#endif
private:

	static	int		sKeyGen;
	mutable	HintMap	mHintMap;

};

#define CONVERT_POINT(__X)	(CDT::Point((__X).x(),(__X).y()))

#endif
