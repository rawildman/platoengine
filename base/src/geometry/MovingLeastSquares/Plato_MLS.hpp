/*
//@HEADER
// *************************************************************************
//   Plato Engine v.1.0: Copyright 2018, National Technology & Engineering
//                    Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Sandia Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact the Plato team (plato3D-help@sandia.gov)
//
// *************************************************************************
//@HEADER
*/

/*!
 * Plato_MLS.hpp
 *
 * Created on: Feb 27, 2018
 *
 */

#pragma once

#include <Kokkos_Core.hpp>
#include <array>
#include <map>
#include <string>
#include "Plato_Parser.hpp"
#include "Plato_Exceptions.hpp"
#include "Plato_KokkosTypes.hpp"

#include <Kokkos_Macros.hpp>

using MemSpace = Kokkos::DefaultExecutionSpace::memory_space;

namespace Plato {
namespace Geometry {

template<typename ScalarType=double>
using ScalarVector = typename Kokkos::View<ScalarType*, MemSpace>;

template<typename ScalarType=double>
using ScalarArray  = typename Kokkos::View<ScalarType**, Plato::Layout, MemSpace>;

using NeighborMap = typename Kokkos::View<int**, Plato::Layout, Kokkos::DefaultExecutionSpace::memory_space>;
using NeighborMapHost = typename NeighborMap::HostMirror;

template<int SpaceDim, typename ScalarType>
class NeighborMapper
{
   // for now, this is done on the host
   typename ScalarArray<ScalarType>::HostMirror m_points;

   struct Neighbor{ 
     Neighbor() : index(0), distance(0.0) {}
     int        index;
     ScalarType distance;
     friend bool operator< (const Neighbor & a, const Neighbor & b)
     {
          return a.distance < b.distance;
     }
   };
  
   public:
       NeighborMapper(const typename ScalarArray<ScalarType>::HostMirror & a_points) : m_points( a_points ) {}
       NeighborMap map( int a_numNeighbors );
};

template<int SpaceDim=3, typename ScalarType=double>
class PointGrid
{

  ScalarVector<int> m_n;
  ScalarVector<int> m_stride;

  ScalarVector<ScalarType> m_d;
  ScalarVector<ScalarType> m_o;
  int m_numPoints;
  
  public:
    PointGrid(std::array<int,SpaceDim> n, 
              std::array<ScalarType,SpaceDim> d,
              std::array<ScalarType,SpaceDim> o);
    
    ScalarArray<ScalarType>
    operator()();
};

template<int SpaceDim=3, typename ScalarType=double>
class SphereArray {

    ScalarType m_radius, m_insideValue, m_outsideValue, m_spacing;
    ScalarArray<ScalarType> m_sphereCoords;

    public:
    /***************************************************************************/
    /*!
     *  /brief Constructor with parsing.
     *  Creates a SphereArray instance from the provided xml input.
     */
    /***************************************************************************/
    SphereArray(const Plato::InputData& a_node, const std::array<ScalarType,SpaceDim>& a_size, const std::array<ScalarType,SpaceDim>& a_offset);

     void
     getValues(ScalarArray<ScalarType> a_coords, ScalarVector<ScalarType> a_field);
};


template<int SpaceDim=3, typename ScalarType=double>
class MovingLeastSquares {

    ScalarArray<ScalarType> m_coords;
    NeighborMap m_neighborMap;
    std::map<std::string, ScalarVector<ScalarType>> m_fields;

    ScalarType m_radius;

    std::string strint(std::string base, int index)
    {
      std::stringstream out;
      out << base << index;
      return out.str();
    }

  public:

    typename decltype(m_coords)::HostMirror getPointCoords()
    {
        typename decltype(m_coords)::HostMirror m_coords_mirror = Kokkos::create_mirror_view(m_coords);
        Kokkos::deep_copy(m_coords_mirror, m_coords);
        return m_coords_mirror;
    }


    decltype(m_fields) getPointFields(){return m_fields;}

    int getNumPoints(){return m_coords.extent(0); }

    /***************************************************************************/
    /*!
     *  /brief Constructor with parsing.
     *  Creates a MovingLeastSquares instance from the provided xml input.
     */
    /***************************************************************************/
    MovingLeastSquares(const Plato::InputData& a_node);

    /******************************************************************************/
    /*! 
     *  /brief Compute the MLS function values.
     *  /input a_pointValues Values at the MLS points
     *  /input a_nodeCoords Coordinates (nodeIndex, dimIndex) of nodes for which MLS 
     *         function values will be computed
     *  /output a_nodeValues MLS function values (nodeIndex)
     */
    /******************************************************************************/
    void
    f(ScalarVector<ScalarType> a_pointValues, ScalarArray<ScalarType> a_nodeCoords, ScalarVector<ScalarType> a_nodeValues);

    /******************************************************************************/
    /*! 
     *  /brief Map a derivative with respect to MLS values at nodes to a derivative
               with respect to MLS point values.
     *  /input a_nodeCoords Coordinates (nodeIndex, dimIndex) of nodes.
     *  /input a_nodeValues Values (nodeIndex) of a gradient with respect to nodal MLS values
     *  /output a_mappedValues Values (pointIndex) of the gradient with respect to the MLS point values.
     */
    /******************************************************************************/
    void
    mapToPoints( 
      ScalarArray<ScalarType> a_nodeCoords, 
      ScalarVector<ScalarType> a_nodeValues, 
      ScalarVector<ScalarType> a_mappedValues);
};

} // end namespace Geometry
} // end namespace Plato
