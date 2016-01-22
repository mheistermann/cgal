// Copyright (c) 2007-2016  INRIA (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Laurent Saboret, Nader Salman, Gael Guennebaud

#ifndef CGAL_POINT_SET_3_H
#define CGAL_POINT_SET_3_H

#include <CGAL/property_map.h>
#include <CGAL/Min_sphere_of_spheres_d.h>
#include <CGAL/Min_sphere_of_points_d_traits_3.h>
#include <CGAL/Min_sphere_of_spheres_d_traits_3.h>
#include <CGAL/Surface_mesh.h>

#include <algorithm>
#include <vector>
# include <CGAL/gl.h>

/// The Point_set_3 class is array of points + normals of type
/// Point_with_normal_3<Gt> (in fact
/// UI_point_3 to support a selection flag and an optional radius).
/// It provides:
/// - accessors: points and normals iterators, property maps
/// - OpenGL rendering
/// - bounding box
///
/// CAUTION:
/// - User is responsible to call invalidate_bounds() after adding, moving or removing points.
/// - Selecting points changes the order of the points in the
///   container. If selection is *not* empty, it becomes invalid after
///   adding, moving or removing points, user is reponsible to call
///   unselect_all() in those cases.
///
/// @heading Parameters:
/// @param Gt       Geometric traits class.

namespace CGAL {

template <class Gt>
class Point_set_3
{
public:

  typedef Point_set_3<Gt> Point_set;
  typedef typename Gt::FT FT;
  typedef typename Gt::Point_3 Point;
  typedef typename Gt::Vector_3 Vector;
  typedef CGAL::cpp11::array<FT, 3> Color;
  typedef typename Gt::Iso_cuboid_3 Iso_cuboid;
  typedef typename Gt::Sphere_3 Sphere;
  
  typedef CGAL::Surface_mesh<Point> Base;
  typedef typename Base::Vertex_index Item;

  typedef typename Base::template Property_map<Item, std::size_t> Index_pmap;
  typedef typename Base::template Property_map<Item, Point> Point_pmap;
  typedef typename Base::template Property_map<Item, Vector> Vector_pmap;
  typedef typename Base::template Property_map<Item, Color> Color_pmap;

  typedef typename Base::Vertex_range::iterator iterator;
  typedef typename Base::Vertex_range::const_iterator const_iterator;
  
private:

  struct Index_back_inserter {

    typedef std::output_iterator_tag iterator_category;
    typedef std::size_t              value_type;
    typedef std::ptrdiff_t           difference_type;
    typedef void                     pointer;
    typedef void                     reference;

  private:

    Point_set& ps;
    std::size_t ind;
  
  public:
  
    Index_back_inserter(Point_set& ps, std::size_t ind=0)
      : ps(ps), ind(ind)
    {}

    Index_back_inserter& operator++()
    {
      return *this;
    }
    Index_back_inserter& operator++(int)
    {
      return *this;
    }
  
    Index_back_inserter& operator= (std::size_t& ind)
    {
      if(! ps.surface_mesh().has_valid_index(ind)){
        ps.surface_mesh().add_vertex();
      }
      put(ps.indices(), Point_set::Item(ind),ind);
      ++ ind;
      return *this;
    }
                                  
    Index_back_inserter& operator*()
    {
      return *this;
    }

  };

  struct  Point_push_pmap {
    Point_set& ps;
    std::size_t ind;

    Point_push_pmap(Point_set& ps, std::size_t ind=0)
      : ps(ps), ind(ind)
    {}

    inline friend void put(Point_push_pmap& pm, std::size_t& i, Point& p)
    {
      if(! pm.ps.surface_mesh().has_valid_index(pm.ind)){
        pm.ps.surface_mesh().add_vertex();
      }
      put(pm.ps.points(), Point_set::Item(pm.ind),p);
      i = pm.ind;
      ++pm.ind;
    }
  };

  struct  Normal_push_pmap {
    Point_set& ps;
    std::size_t ind;

    Normal_push_pmap(Point_set& ps, std::size_t ind=0)
      : ps(ps), ind(ind)
    {}

    inline friend void put(Normal_push_pmap& pm, std::size_t& i  , Vector& v)
    {
      if(! pm.ps.surface_mesh().has_valid_index(pm.ind)){
        pm.ps.surface_mesh().add_vertex();
      }
      put(pm.ps.normals(), Point_set::Item(pm.ind),v);
      i = pm.ind;
      ++pm.ind;
    }
  };



  Base m_base;
  Index_pmap m_indices;
  Vector_pmap m_normals;
  Color_pmap m_colors;

  std::size_t m_nb_selected; // handle selection

  // Assignment operator not implemented and declared private to make
  // sure nobody uses the default one without knowing it
  Point_set_3& operator= (const Point_set_3&)
  {
    return *this;
  }

  
public:

  Point_set_3 () : m_base()
  {
    m_nb_selected = 0;
    assert (add_index_property());
  }

  // copy constructor 
  Point_set_3 (const Point_set_3& p) : m_base(p.m_base)
  {
    m_nb_selected = p.nb_selected_points ();
    assert (add_index_property());
  }

  void push_back (const Point& p)
  {
    m_base.add_vertex (p);
  }

  Base& surface_mesh()
  {
    return m_base;
  }

  Index_pmap& indices()
  {
    return m_indices;
  }

  Point_pmap points()
  {
    return m_base.points();
  }


  Vector_pmap& normals()
  {
    return m_normals;
  }


  iterator begin() { return m_base.vertices().begin(); }
  iterator end() { return m_base.vertices().end(); }
  const_iterator begin() const { return m_base.vertices().begin(); }
  const_iterator end() const { return m_base.vertices().end(); }
  bool empty() const { return m_base.is_empty(); }
  std::size_t size () const { return m_base.number_of_vertices(); }
  void clear() { m_base.clear(); }
  Point& operator[] (std::size_t index) { return m_base.point (Item (m_indices[Item(index)])); }
  const Point& operator[] (std::size_t index) const { return (*this)[index]; }
  std::size_t index (std::size_t i) { return m_indices[Item(i)]; }

  Index_back_inserter index_back_inserter ()
  {
    return Index_back_inserter (*this);
  }
  Point_push_pmap point_push_pmap ()
  {
    return Point_push_pmap (*this);
  }
  Normal_push_pmap normal_push_pmap ()
  {
    return Normal_push_pmap (*this);
  }
    
  iterator first_selected() { return end() - m_nb_selected; }
  const_iterator first_selected() const { return end() - m_nb_selected; }
  void set_first_selected(iterator it)
  {
    m_nb_selected = static_cast<std::size_t>(std::distance (it, end()));
  }
  
  // Test if point is selected
  bool is_selected(const_iterator it) const
  {
    return static_cast<std::size_t>(std::distance (it, end())) <= m_nb_selected;
  }

  /// Gets the number of selected points.
  std::size_t nb_selected_points() const
  {
    return m_nb_selected;
  }

  /// Mark a point as selected/not selected.
  void select(iterator it, bool selected = true)
  {
    bool currently = is_selected (it);
    iterator first = first_selected();
    if (currently && !selected)
      {
        std::swap (*it, *first);
        -- m_nb_selected;
      }
    else if (!currently && selected)
      {
        std::swap (*it, *first);
        ++ m_nb_selected;
      }
  }

  void select_all()
  {
    m_nb_selected = size ();
  }
  void unselect_all()
  {
    m_nb_selected = 0;
  }

  // Invert selection
  void invert_selection()
  {
    iterator sel = end() - 1;
    iterator unsel = begin();

    iterator first = first_selected();

    while (sel != first - 1 && unsel != first)
      std::swap (*(sel --), *(unsel ++));
    
    m_nb_selected = size() - m_nb_selected;
  }

  /// Deletes selected points.
  void delete_selection()
  {
    // Deletes selected points using erase-remove idiom
    for (std::size_t i = size() - 1; i != 0; -- i)
      m_base.remove_vertex (Item (i));

    m_nb_selected = 0;
  }

  bool add_index_property()
  {
    bool out = false;
    boost::tie (m_indices, out) = m_base.template add_property_map<Item, std::size_t> ("index");
    return out;
  }

  bool has_normals() const
  {
    std::pair<Vector_pmap, bool> pm = m_base.template property_map<Item, Vector> ("normal");
    return pm.second;
  }
  bool add_normal_property()
  {
    bool out = false;
    boost::tie (m_normals, out) = m_base.template add_property_map<Item, Vector> ("normal");
    return out;
  }
  void remove_normal_property()
  {
    m_base.remove_property_map (m_normals);
  }
  Vector& normal (std::size_t index) { return m_normals[Item (m_indices[Item(index)])]; }
  const Vector& normal (std::size_t index) const { return this->normal(index); }

  bool has_colors() const
  {
    std::pair<Color_pmap, bool> pm = m_base.template property_map<Item, Color> ("color");
    return pm.second;
  }
  bool add_color_property()
  {
    bool out = false;
    boost::tie (m_colors, out) = m_base.template add_property_map<Item, Color> ("color");
    return out;
  }
  void remove_color_property()
  {
    m_base.remove_property_map (m_colors);
  }
  Color& color (std::size_t index) { return m_colors[Item (index)]; }
  const Color& color (std::size_t index) const { return this->color(index); }

  static CGAL::Second_of_pair_property_map<std::pair<Point, Vector> >
  point_property_map ()
  {
    return CGAL::make_second_of_pair_property_map (std::pair<Point, Vector>());
  }
  static CGAL::First_of_pair_property_map<std::pair<Point, Vector> >
  normal_property_map ()
  {
    return CGAL::make_first_of_pair_property_map (std::pair<Point, Vector>());
  }

private:


  
  
  
}; // end of class Point_set_3

} // namespace CGAL

#endif // CGAL_POINT_SET_3_H
