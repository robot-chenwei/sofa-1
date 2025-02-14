/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#define SOFA_COMPONENT_TOPOLOGY_TRIANGLESETTOPOLOGYALGORITHMS_CPP
#include <SofaBaseTopology/TriangleSetTopologyAlgorithms.inl>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace topology
{
using namespace sofa::defaulttype;

int TriangleSetTopologyAlgorithmsClass = core::RegisterObject("Triangle set topology algorithms")
#ifdef SOFA_FLOAT
        .add< TriangleSetTopologyAlgorithms<Vec3fTypes> >(true) // default template
#else
        .add< TriangleSetTopologyAlgorithms<Vec3dTypes> >(true) // default template
#ifndef SOFA_DOUBLE
        .add< TriangleSetTopologyAlgorithms<Vec3fTypes> >() // default template
#endif
#endif
#ifndef SOFA_FLOAT
        .add< TriangleSetTopologyAlgorithms<Vec2dTypes> >()
        .add< TriangleSetTopologyAlgorithms<Vec1dTypes> >()
#endif
#ifndef SOFA_DOUBLE
        .add< TriangleSetTopologyAlgorithms<Vec2fTypes> >()
        .add< TriangleSetTopologyAlgorithms<Vec1fTypes> >()
#endif
        ;


// methods specilizations declaration
#ifdef SOFA_WITH_DOUBLE
template<> SOFA_BASE_TOPOLOGY_API
int TriangleSetTopologyAlgorithms<defaulttype::Vec2dTypes>::SplitAlongPath(PointID pa, Coord& a, PointID pb, Coord& b,
    sofa::helper::vector< sofa::core::topology::TopologyObjectType>& topoPath_list,
    sofa::helper::vector<ElemID>& indices_list,
    sofa::helper::vector< sofa::defaulttype::Vec<3, double> >& coords_list,
    sofa::helper::vector<EdgeID>& new_edges, double epsilonSnapPath, double epsilonSnapBorder);
template<> SOFA_BASE_TOPOLOGY_API
int TriangleSetTopologyAlgorithms<defaulttype::Vec1dTypes>::SplitAlongPath(PointID pa, Coord& a, PointID pb, Coord& b,
    sofa::helper::vector< sofa::core::topology::TopologyObjectType>& topoPath_list,
    sofa::helper::vector<ElemID>& indices_list,
    sofa::helper::vector< sofa::defaulttype::Vec<3, double> >& coords_list,
    sofa::helper::vector<EdgeID>& new_edges, double epsilonSnapPath, double epsilonSnapBorder);
#endif
#ifdef SOFA_WITH_FLOAT
template<> SOFA_BASE_TOPOLOGY_API
int TriangleSetTopologyAlgorithms<defaulttype::Vec2fTypes>::SplitAlongPath(PointID pa, Coord& a, PointID pb, Coord& b,
    sofa::helper::vector< sofa::core::topology::TopologyObjectType>& topoPath_list,
    sofa::helper::vector<ElemID>& indices_list,
    sofa::helper::vector< sofa::defaulttype::Vec<3, double> >& coords_list,
    sofa::helper::vector<EdgeID>& new_edges, double epsilonSnapPath, double epsilonSnapBorder);
template<> SOFA_BASE_TOPOLOGY_API
int TriangleSetTopologyAlgorithms<defaulttype::Vec1fTypes>::SplitAlongPath(PointID pa, Coord& a, PointID pb, Coord& b,
    sofa::helper::vector< sofa::core::topology::TopologyObjectType>& topoPath_list,
    sofa::helper::vector<ElemID>& indices_list,
    sofa::helper::vector< sofa::defaulttype::Vec<3, double> >& coords_list,
    sofa::helper::vector<EdgeID>& new_edges, double epsilonSnapPath, double epsilonSnapBorder);
#endif



#ifdef SOFA_WITH_DOUBLE
template class SOFA_BASE_TOPOLOGY_API TriangleSetTopologyAlgorithms<Vec3dTypes>;
template class SOFA_BASE_TOPOLOGY_API TriangleSetTopologyAlgorithms<Vec2dTypes>;
template class SOFA_BASE_TOPOLOGY_API TriangleSetTopologyAlgorithms<Vec1dTypes>;
#endif

#ifdef SOFA_WITH_FLOAT
template class SOFA_BASE_TOPOLOGY_API TriangleSetTopologyAlgorithms<Vec3fTypes>;
template class SOFA_BASE_TOPOLOGY_API TriangleSetTopologyAlgorithms<Vec2fTypes>;
template class SOFA_BASE_TOPOLOGY_API TriangleSetTopologyAlgorithms<Vec1fTypes>;
#endif




// methods specilizations definition
#ifdef SOFA_WITH_DOUBLE

template<>
int TriangleSetTopologyAlgorithms<defaulttype::Vec2dTypes>::SplitAlongPath(PointID , Coord& , PointID , Coord& ,
                                                              sofa::helper::vector< sofa::core::topology::TopologyObjectType>& ,
                                                              sofa::helper::vector<ElemID>& ,
                                                              sofa::helper::vector< sofa::defaulttype::Vec<3, double> >& ,
                                                              sofa::helper::vector<EdgeID>& , double  , double )
{
    serr<<"TriangleSetTopologyAlgorithms<defaulttype::Vec2dTypes>::SplitAlongPath not implemented"<<sendl;
    return 0;
}

template<>
int TriangleSetTopologyAlgorithms<defaulttype::Vec1dTypes>::SplitAlongPath(PointID , Coord& , PointID , Coord& ,
                                                              sofa::helper::vector< sofa::core::topology::TopologyObjectType>& ,
                                                              sofa::helper::vector<ElemID>& ,
                                                              sofa::helper::vector< sofa::defaulttype::Vec<3, double> >& ,
                                                              sofa::helper::vector<EdgeID>& , double  , double )
{
    serr<<"TriangleSetTopologyAlgorithms<defaulttype::Vec1dTypes>::SplitAlongPath not implemented"<<sendl;
    return 0;
}

#endif


#ifdef SOFA_WITH_FLOAT

template<>
int TriangleSetTopologyAlgorithms<defaulttype::Vec2fTypes>::SplitAlongPath(PointID , Coord& , PointID , Coord& ,
                                                              sofa::helper::vector< sofa::core::topology::TopologyObjectType>& ,
                                                              sofa::helper::vector<ElemID>& ,
                                                              sofa::helper::vector< sofa::defaulttype::Vec<3, double> >& ,
                                                              sofa::helper::vector<EdgeID>& , double  , double )
{
    sout<<"TriangleSetTopologyAlgorithms<defaulttype::Vec2fTypes>::SplitAlongPath not implemented"<<sendl;
    return 0;
}
template<>
int TriangleSetTopologyAlgorithms<defaulttype::Vec1fTypes>::SplitAlongPath(PointID , Coord& , PointID , Coord& ,
                                                              sofa::helper::vector< sofa::core::topology::TopologyObjectType>& ,
                                                              sofa::helper::vector<ElemID>& ,
                                                              sofa::helper::vector< sofa::defaulttype::Vec<3, double> >& ,
                                                              sofa::helper::vector<EdgeID>& , double  , double )
{
    sout<<"TriangleSetTopologyAlgorithms<defaulttype::Vec1fTypes>::SplitAlongPath not implemented"<<sendl;
    return 0;
}

#endif


} // namespace topology

} // namespace component

} // namespace sofa
