// Copyright (c) 2017  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Mael Rouxel-Labbé

#ifndef CGAL_INTERNAL_STATIC_FILTERS_PERIODIC_3_POWER_TEST_3_H
#define CGAL_INTERNAL_STATIC_FILTERS_PERIODIC_3_POWER_TEST_3_H

#include <CGAL/license/Periodic_3_triangulation_3.h>

#include <CGAL/Profile_counter.h>
#include <CGAL/internal/Static_filters/Static_filter_error.h>
#include <CGAL/internal/Static_filters/tools.h>

#include <CGAL/Periodic_3_offset_3.h>

#include <cmath>

namespace CGAL {

namespace internal {

namespace Static_filters_predicates {

template <typename K_base>
class Periodic_3_power_side_of_oriented_power_sphere_3:
    public K_base::Power_side_of_oriented_power_sphere_3
{
  // @todo
};

} // namespace Static_filters_predicates

} // namespace internal

} //namespace CGAL

#endif // CGAL_INTERNAL_STATIC_FILTERS_PERIODIC_3_POWER_TEST_3_H
