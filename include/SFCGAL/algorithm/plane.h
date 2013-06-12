/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _SFCGAL_ALGORITHM_PLANE_H_
#define _SFCGAL_ALGORITHM_PLANE_H_

#include <boost/format.hpp>

//#include <SFCGAL/detail/ublas.h>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/algorithm/normal.h>

namespace SFCGAL {
namespace algorithm {


    /**
     * Test if a set of points lie in the same plane
     */
    inline
    bool isPlane3D( const LineString & l, const double & toleranceAbs )
    {
        if (l.isEmpty()) return true;

        // the present approach is to find a good plane by:
        // - computing the centroid C of the point set
        // - finding the farest point F from C
        // - finding the farest point G from (CF)
        // - we define the unit normal N to the plane from CFxCG
        // - we check that points Xi are in the plane CXi.N < tolerance
        //
        // note that we could compute the covarence matrix of the points and use SVD
        // but we would need a lib for that, and it may be overkill

		typedef CGAL::Point_3< Kernel > Point_3 ;
		typedef CGAL::Vector_3< Kernel > Vector_3 ;

        const LineString::const_iterator end = l.end();

        // centroid
        Vector_3 c(0,0,0);
        int numPoint = 0;
        for ( LineString::const_iterator x = l.begin(); x != end; ++x ) {
            c = c + x->toVector_3() ;
            ++numPoint;
        }
        BOOST_ASSERT( numPoint );
        c = c / numPoint;

        // farest point from centroid
        Vector_3 f = c ;
        Kernel::FT maxDistanceSq = 0;
        for ( LineString::const_iterator x = l.begin(); x != end; ++x ) {
            const Vector_3 cx = x->toVector_3() - c ;
            const Kernel::FT dSq = cx * cx ;
            if ( dSq > maxDistanceSq ) {
                f = x->toVector_3() ;
                maxDistanceSq = dSq ;
            }
        }

        if ( std::sqrt( CGAL::to_double( maxDistanceSq ) ) < toleranceAbs ) {
            // std::cout << "all points in the same location\n";
            return true;
        }

        // farest point from line
        Vector_3 g=c;
        const Vector_3 cf = f - c ; // direction of (CF)
        maxDistanceSq = 0; // watch out, we reuse the variable
        for ( LineString::const_iterator x = l.begin(); x != end; ++x ) {
            const Vector_3 cx = x->toVector_3() - c ;
            const Vector_3 cp = ( cx * cf ) * cf / cf.squared_length() ; // projection of x on line
            const Kernel::FT dSq = (cx - cp ).squared_length() ;
            if ( dSq > maxDistanceSq ) {
                g = x->toVector_3() ;
                maxDistanceSq = dSq ;
            }
        }

        if ( std::sqrt( CGAL::to_double( maxDistanceSq ) ) < toleranceAbs ) {
            // std::cout << "all points aligned\n";
            return true;
        }

        const Vector_3 n = CGAL::cross_product( cf, g - c );
        const Vector_3 nNormed = n / std::sqrt( CGAL::to_double( n.squared_length() ) );
        for ( LineString::const_iterator x = l.begin(); x != end; ++x ) {
            const Vector_3 cx = x->toVector_3() - c ;
            if ( std::abs( CGAL::to_double( cx * n ) ) > toleranceAbs ) {
                // std::cout << "point out of plane\n";
                return false; 
            }
        }

        // std::cout << "plane general case\n";
        return true;
    }

	/**
	 * Test if a 3D plane can be extracted from a Polygon
	 */
	template < typename Kernel >
	bool hasPlane3D( const Polygon& polygon,
			 CGAL::Point_3< Kernel > & a,
			 CGAL::Point_3< Kernel > & b,
			 CGAL::Point_3< Kernel > & c )
	{
		typedef CGAL::Point_3< Kernel > Point_3 ;

		const LineString & exteriorRing = polygon.exteriorRing() ;

		/*
		 * look for 3 non collinear points
		 */
		size_t  n = 0 ;
		for ( size_t i = 0; i < exteriorRing.numPoints(); i++ ){
			Point_3 p = exteriorRing.pointN(i).toPoint_3() ;
			if ( n == 0 ){
				a = p ;
				n++ ;
			}else if ( n == 1 && a != p ){
				b = p ;
				n++ ;
			}else if ( n == 2 && ! CGAL::collinear( a, b, p ) ) {
				c = p ;
				n++ ;
				return true ;
			}
		}
		BOOST_ASSERT( n < 3 );
		return false;
	}

	/**
	 * Test if a 3D plane can be extracted from a Polygon
	 */
	template < typename Kernel >
	bool hasPlane3D( const Polygon& polygon )
	{
		// temporary arguments
		CGAL::Point_3< Kernel > a, b, c;
		return hasPlane3D( polygon, a, b, c );
	}

	/**
	 * Get 3 non collinear points from a Polygon
	 */
	template < typename Kernel >
	void plane3D(
		const Polygon & polygon,
		CGAL::Point_3< Kernel > & a,
		CGAL::Point_3< Kernel > & b,
		CGAL::Point_3< Kernel > & c
	)
	{
		if ( ! hasPlane3D( polygon, a, b, c) ){
			BOOST_THROW_EXCEPTION(
				Exception(
					( boost::format("can't find plane for Polygon '%1%'") % polygon.asText(3) ).str()
				)
			);
		}
	}

	/**
	 * Returns the oriented 3D plane of a polygon (supposed to be planar).
	 * @warning result is rounded to double if exact is false (avoid huge expression tree)
	 */
	template < typename Kernel >
	CGAL::Plane_3< Kernel > plane3D( const Polygon & polygon, bool exact = true )
	{
		CGAL::Vector_3< Kernel > normal = normal3D< Kernel >( polygon, exact );
		return CGAL::Plane_3< Kernel >( polygon.exteriorRing().pointN(0).toPoint_3(), normal );
	}



}//algorithm
}//SFCGAL


#endif
