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
#define SOFA_COMPONENT_MAPPING_BARYCENTRICMAPPINGRIGID_CPP
#include <SofaMiscMapping/BarycentricMappingRigid.inl>
#include <SofaBaseMechanics/BarycentricMappers/TopologyBarycentricMapper.inl>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapperTetrahedronSetTopology.inl>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapperHexahedronSetTopology.inl>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapperTriangleSetTopology.inl>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapperQuadSetTopology.inl>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapperEdgeSetTopology.inl>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapperSparseGridTopology.inl>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapperRegularGridTopology.inl>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapperMeshTopology.inl>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapperTopologyContainer.inl>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapper.inl>

#include <sofa/core/ObjectFactory.h>

#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>


namespace sofa
{

namespace component
{

namespace mapping
{

using namespace sofa::defaulttype;

// Register in the Factory
static int BarycentricMappingRigidClass = core::RegisterObject("")
#ifndef SOFA_FLOAT
        .add< BarycentricMapping< Vec3dTypes, Rigid3dTypes > >()
#endif
#ifndef SOFA_DOUBLE
        .add< BarycentricMapping< Vec3fTypes, Rigid3fTypes > >()
#endif
#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE
        .add< BarycentricMapping< Vec3fTypes, Rigid3dTypes > >()
        .add< BarycentricMapping< Vec3dTypes, Rigid3fTypes > >()
#endif
#endif
        ;


#ifndef SOFA_FLOAT
template <>
void BarycentricMapperHexahedronSetTopology<defaulttype::Vec3dTypes, defaulttype::Rigid3dTypes>::handleTopologyChange(core::topology::Topology* t)
{
    if (t != this->m_fromTopology) return;
    if ( this->m_fromTopology->beginChange() == this->m_fromTopology->endChange() )
        return;

    std::list<const core::topology::TopologyChange *>::const_iterator itBegin = this->m_fromTopology->beginChange();
    std::list<const core::topology::TopologyChange *>::const_iterator itEnd = this->m_fromTopology->endChange();

    for ( std::list<const core::topology::TopologyChange *>::const_iterator changeIt = itBegin;
            changeIt != itEnd; ++changeIt )
    {
        const core::topology::TopologyChangeType changeType = ( *changeIt )->getChangeType();
        switch ( changeType )
        {
            //TODO: implementation of BarycentricMapperHexahedronSetTopology<In,Out>::handleTopologyChange()
        case core::topology::ENDING_EVENT:       ///< To notify the end for the current sequence of topological change events
        {
            if(!m_invalidIndex.empty())
            {
                helper::vector<MappingData>& mapData = *(d_map.beginEdit());

                for ( std::set<int>::const_iterator iter = m_invalidIndex.begin();
                        iter != m_invalidIndex.end(); ++iter )
                {
                    const int j = *iter;
                    if ( mapData[j].in_index == -1 ) // compute new mapping
                    {
                        Vector3 coefs;
                        defaulttype::Vec3dTypes::Coord pos;
                        pos[0] = mapData[j].baryCoords[0];
                        pos[1] = mapData[j].baryCoords[1];
                        pos[2] = mapData[j].baryCoords[2];

                        // find nearest cell and barycentric coords
                        Real distance = 1e10;

                        int index = m_fromGeomAlgo->findNearestElementInRestPos ( pos, coefs, distance );

                        if ( index != -1 )
                        {
                            mapData[j].baryCoords[0] = ( Real ) coefs[0];
                            mapData[j].baryCoords[1] = ( Real ) coefs[1];
                            mapData[j].baryCoords[2] = ( Real ) coefs[2];
                            mapData[j].in_index = index;
                        }
                    }
                }

                d_map.endEdit();
                m_invalidIndex.clear();
            }
        }
        break;
        case core::topology::POINTSINDICESSWAP:  ///< For PointsIndicesSwap.
            break;
        case core::topology::POINTSADDED:        ///< For PointsAdded.
            break;
        case core::topology::POINTSREMOVED:      ///< For PointsRemoved.
            break;
        case core::topology::POINTSRENUMBERING:  ///< For PointsRenumbering.
            break;
        case core::topology::TRIANGLESADDED:  ///< For Triangles Added.
            break;
        case core::topology::TRIANGLESREMOVED:  ///< For Triangles Removed.
            break;
        case core::topology::HEXAHEDRAADDED:     ///< For HexahedraAdded.
        {
        }
        break;
        case core::topology::HEXAHEDRAREMOVED:   ///< For HexahedraRemoved.
        {
            const unsigned int nbHexahedra = this->m_fromTopology->getNbHexahedra();

            const sofa::helper::vector<unsigned int> &hexahedra =
                    ( static_cast< const sofa::core::topology::HexahedraRemoved *> ( *changeIt ) )->getArray();

            for ( unsigned int i=0; i<hexahedra.size(); ++i )
            {
                // remove all references to the removed cubes from the mapping data
                unsigned int cubeId = hexahedra[i];
                for ( unsigned int j=0; j<d_map.getValue().size(); ++j )
                {
                    if ( d_map.getValue()[j].in_index == ( int ) cubeId ) // invalidate mapping
                    {
                        sofa::defaulttype::Vector3 coefs;
                        coefs[0] = d_map.getValue()[j].baryCoords[0];
                        coefs[1] = d_map.getValue()[j].baryCoords[1];
                        coefs[2] = d_map.getValue()[j].baryCoords[2];

                        defaulttype::Vec3dTypes::Coord restPos = m_fromGeomAlgo->getRestPointPositionInHexahedron ( cubeId, coefs );

                        helper::vector<MappingData>& vectorData = *(d_map.beginEdit());
                        vectorData[j].in_index = -1;
                        vectorData[j].baryCoords[0] = restPos[0];
                        vectorData[j].baryCoords[1] = restPos[1];
                        vectorData[j].baryCoords[2] = restPos[2];
                        d_map.endEdit();

                        m_invalidIndex.insert(j);
                    }
                }
            }

            // renumber
            unsigned int lastCubeId = nbHexahedra-1;
            for ( unsigned int i=0; i<hexahedra.size(); ++i, --lastCubeId )
            {
                unsigned int cubeId = hexahedra[i];
                for ( unsigned int j=0; j<d_map.getValue().size(); ++j )
                {
                    if ( d_map.getValue()[j].in_index == ( int ) lastCubeId )
                    {
                        helper::vector<MappingData>& vectorData = *(d_map.beginEdit());
                        vectorData[j].in_index = cubeId;
                        d_map.endEdit();
                    }
                }
            }
        }
        break;
        case core::topology::HEXAHEDRARENUMBERING: ///< For HexahedraRenumbering.
            break;
        default:
            break;
        }
    }
}
#endif

#ifndef SOFA_DOUBLE
template <>
void BarycentricMapperHexahedronSetTopology<defaulttype::Vec3fTypes, defaulttype::Rigid3fTypes>::handleTopologyChange(core::topology::Topology* t)
{
    if (t != this->m_fromTopology) return;

    if ( this->m_fromTopology->beginChange() == this->m_fromTopology->endChange() )
        return;

    std::list<const core::topology::TopologyChange *>::const_iterator itBegin = this->m_fromTopology->beginChange();
    std::list<const core::topology::TopologyChange *>::const_iterator itEnd = this->m_fromTopology->endChange();

    for ( std::list<const core::topology::TopologyChange *>::const_iterator changeIt = itBegin;
            changeIt != itEnd; ++changeIt )
    {
        const core::topology::TopologyChangeType changeType = ( *changeIt )->getChangeType();
        switch ( changeType )
        {
            //TODO: implementation of BarycentricMapperHexahedronSetTopology<In,Out>::handleTopologyChange()
        case core::topology::ENDING_EVENT:       ///< To notify the end for the current sequence of topological change events
        {
            if(!m_invalidIndex.empty())
            {
                helper::vector<MappingData>& mapData = *(d_map.beginEdit());

                for ( std::set<int>::const_iterator iter = m_invalidIndex.begin();
                        iter != m_invalidIndex.end(); ++iter )
                {
                    const int j = *iter;
                    if ( mapData[j].in_index == -1 ) // compute new mapping
                    {
                        Vector3 coefs;
                        defaulttype::Vec3fTypes::Coord pos;
                        pos[0] = mapData[j].baryCoords[0];
                        pos[1] = mapData[j].baryCoords[1];
                        pos[2] = mapData[j].baryCoords[2];

                        // find nearest cell and barycentric coords
                        Real distance = 1e10;

                        int index = m_fromGeomAlgo->findNearestElementInRestPos ( pos, coefs, distance );

                        if ( index != -1 )
                        {
                            mapData[j].baryCoords[0] = ( Real ) coefs[0];
                            mapData[j].baryCoords[1] = ( Real ) coefs[1];
                            mapData[j].baryCoords[2] = ( Real ) coefs[2];
                            mapData[j].in_index = index;
                        }
                    }
                }

                d_map.endEdit();
                m_invalidIndex.clear();
            }
        }
        break;
        case core::topology::POINTSINDICESSWAP:  ///< For PointsIndicesSwap.
            break;
        case core::topology::POINTSADDED:        ///< For PointsAdded.
            break;
        case core::topology::POINTSREMOVED:      ///< For PointsRemoved.
            break;
        case core::topology::POINTSRENUMBERING:  ///< For PointsRenumbering.
            break;
        case core::topology::TRIANGLESADDED:  ///< For Triangles Added.
            break;
        case core::topology::TRIANGLESREMOVED:  ///< For Triangles Removed.
            break;
        case core::topology::HEXAHEDRAADDED:     ///< For HexahedraAdded.
        {
        }
        break;
        case core::topology::HEXAHEDRAREMOVED:   ///< For HexahedraRemoved.
        {
            const unsigned int nbHexahedra = this->m_fromTopology->getNbHexahedra();

            const sofa::helper::vector<unsigned int> &hexahedra =
                    ( static_cast< const sofa::core::topology::HexahedraRemoved *> ( *changeIt ) )->getArray();

            for ( unsigned int i=0; i<hexahedra.size(); ++i )
            {
                // remove all references to the removed cubes from the mapping data
                unsigned int cubeId = hexahedra[i];
                for ( unsigned int j=0; j<d_map.getValue().size(); ++j )
                {
                    if ( d_map.getValue()[j].in_index == ( int ) cubeId ) // invalidate mapping
                    {
                        Vector3 coefs;
                        coefs[0] = d_map.getValue()[j].baryCoords[0];
                        coefs[1] = d_map.getValue()[j].baryCoords[1];
                        coefs[2] = d_map.getValue()[j].baryCoords[2];

                        defaulttype::Vec3fTypes::Coord restPos = m_fromGeomAlgo->getRestPointPositionInHexahedron ( cubeId, coefs );

                        helper::vector<MappingData>& vectorData = *(d_map.beginEdit());
                        vectorData[j].in_index = -1;
                        vectorData[j].baryCoords[0] = restPos[0];
                        vectorData[j].baryCoords[1] = restPos[1];
                        vectorData[j].baryCoords[2] = restPos[2];
                        d_map.endEdit();

                        m_invalidIndex.insert(j);
                    }
                }
            }

            // renumber
            unsigned int lastCubeId = nbHexahedra-1;
            for ( unsigned int i=0; i<hexahedra.size(); ++i, --lastCubeId )
            {
                unsigned int cubeId = hexahedra[i];
                for ( unsigned int j=0; j<d_map.getValue().size(); ++j )
                {
                    if ( d_map.getValue()[j].in_index == ( int ) lastCubeId )
                    {
                        helper::vector<MappingData>& vectorData = *(d_map.beginEdit());
                        vectorData[j].in_index = cubeId;
                        d_map.endEdit();
                    }
                }
            }
        }
        break;
        case core::topology::HEXAHEDRARENUMBERING: ///< For HexahedraRenumbering.
            break;
        default:
            break;
        }
    }
}
#endif

#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE

template <>
void BarycentricMapperHexahedronSetTopology<defaulttype::Vec3dTypes, defaulttype::Rigid3fTypes>::handleTopologyChange(core::topology::Topology* t)
{
    if (t != this->m_fromTopology) return;

    if ( this->m_fromTopology->beginChange() == this->m_fromTopology->endChange() )
        return;

    std::list<const core::topology::TopologyChange *>::const_iterator itBegin = this->m_fromTopology->beginChange();
    std::list<const core::topology::TopologyChange *>::const_iterator itEnd = this->m_fromTopology->endChange();

    for ( std::list<const core::topology::TopologyChange *>::const_iterator changeIt = itBegin;
            changeIt != itEnd; ++changeIt )
    {
        const core::topology::TopologyChangeType changeType = ( *changeIt )->getChangeType();
        switch ( changeType )
        {
        //TODO(dmarchal 2017-05-03) Who will do it and when ? In one year I remove this todo.
        //TODO: implementation of BarycentricMapperHexahedronSetTopology<In,Out>::handleTopologyChange()
        case core::topology::ENDING_EVENT:       ///< To notify the end for the current sequence of topological change events
        {
            if(!m_invalidIndex.empty())
            {
                helper::vector<MappingData>& mapData = *(d_map.beginEdit());

                for ( std::set<int>::const_iterator iter = m_invalidIndex.begin();
                        iter != m_invalidIndex.end(); ++iter )
                {
                    const int j = *iter;
                    if ( mapData[j].in_index == -1 ) // compute new mapping
                    {
                        Vector3 coefs;
                        defaulttype::Vec3dTypes::Coord pos;
                        pos[0] = mapData[j].baryCoords[0];
                        pos[1] = mapData[j].baryCoords[1];
                        pos[2] = mapData[j].baryCoords[2];

                        // find nearest cell and barycentric coords
                        Real distance = 1e10;

                        int index = m_fromGeomAlgo->findNearestElementInRestPos ( pos, coefs, distance );

                        if ( index != -1 )
                        {
                            mapData[j].baryCoords[0] = ( Real ) coefs[0];
                            mapData[j].baryCoords[1] = ( Real ) coefs[1];
                            mapData[j].baryCoords[2] = ( Real ) coefs[2];
                            mapData[j].in_index = index;
                        }
                    }
                }

                d_map.endEdit();
                m_invalidIndex.clear();
            }
        }
        break;
        case core::topology::POINTSINDICESSWAP:  ///< For PointsIndicesSwap.
            break;
        case core::topology::POINTSADDED:        ///< For PointsAdded.
            break;
        case core::topology::POINTSREMOVED:      ///< For PointsRemoved.
            break;
        case core::topology::POINTSRENUMBERING:  ///< For PointsRenumbering.
            break;
        case core::topology::TRIANGLESADDED:  ///< For Triangles Added.
            break;
        case core::topology::TRIANGLESREMOVED:  ///< For Triangles Removed.
            break;
        case core::topology::HEXAHEDRAADDED:     ///< For HexahedraAdded.
        {
        }
        break;
        case core::topology::HEXAHEDRAREMOVED:   ///< For HexahedraRemoved.
        {
            const unsigned int nbHexahedra = this->m_fromTopology->getNbHexahedra();

            const sofa::helper::vector<unsigned int> &hexahedra =
                    ( static_cast< const sofa::core::topology::HexahedraRemoved *> ( *changeIt ) )->getArray();

            for ( unsigned int i=0; i<hexahedra.size(); ++i )
            {
                // remove all references to the removed cubes from the mapping data
                unsigned int cubeId = hexahedra[i];
                for ( unsigned int j=0; j<d_map.getValue().size(); ++j )
                {
                    if ( d_map.getValue()[j].in_index == ( int ) cubeId ) // invalidate mapping
                    {
                        Vector3 coefs;
                        coefs[0] = d_map.getValue()[j].baryCoords[0];
                        coefs[1] = d_map.getValue()[j].baryCoords[1];
                        coefs[2] = d_map.getValue()[j].baryCoords[2];

                        defaulttype::Vec3dTypes::Coord restPos = m_fromGeomAlgo->getRestPointPositionInHexahedron ( cubeId, coefs );

                        helper::vector<MappingData>& vectorData = *(d_map.beginEdit());
                        vectorData[j].in_index = -1;
                        vectorData[j].baryCoords[0] = restPos[0];
                        vectorData[j].baryCoords[1] = restPos[1];
                        vectorData[j].baryCoords[2] = restPos[2];
                        d_map.endEdit();

                        m_invalidIndex.insert(j);
                    }
                }
            }

            // renumber
            unsigned int lastCubeId = nbHexahedra-1;
            for ( unsigned int i=0; i<hexahedra.size(); ++i, --lastCubeId )
            {
                unsigned int cubeId = hexahedra[i];
                for ( unsigned int j=0; j<d_map.getValue().size(); ++j )
                {
                    if ( d_map.getValue()[j].in_index == ( int ) lastCubeId )
                    {
                        helper::vector<MappingData>& vectorData = *(d_map.beginEdit());
                        vectorData[j].in_index = cubeId;
                        d_map.endEdit();
                    }
                }
            }
        }
        break;
        case core::topology::HEXAHEDRARENUMBERING: ///< For HexahedraRenumbering.
            break;
        default:
            break;
        }
    }
}



template <>
void BarycentricMapperHexahedronSetTopology<defaulttype::Vec3fTypes, defaulttype::Rigid3dTypes>::handleTopologyChange(core::topology::Topology* t)
{
    if (t != this->m_fromTopology) return;

    if ( this->m_fromTopology->beginChange() == this->m_fromTopology->endChange() )
        return;

    std::list<const core::topology::TopologyChange *>::const_iterator itBegin = this->m_fromTopology->beginChange();
    std::list<const core::topology::TopologyChange *>::const_iterator itEnd = this->m_fromTopology->endChange();

    for ( std::list<const core::topology::TopologyChange *>::const_iterator changeIt = itBegin;
            changeIt != itEnd; ++changeIt )
    {
        const core::topology::TopologyChangeType changeType = ( *changeIt )->getChangeType();
        switch ( changeType )
        {
        //TODO(dmarchal 2017-05-03) Who will do it and when ? In one year I remove this todo.
        //TODO: implementation of BarycentricMapperHexahedronSetTopology<In,Out>::handleTopologyChange()
        case core::topology::ENDING_EVENT:       ///< To notify the end for the current sequence of topological change events
        {
            if(!m_invalidIndex.empty())
            {
                helper::vector<MappingData>& mapData = *(d_map.beginEdit());

                for ( std::set<int>::const_iterator iter = m_invalidIndex.begin();
                        iter != m_invalidIndex.end(); ++iter )
                {
                    const int j = *iter;
                    if ( mapData[j].in_index == -1 ) // compute new mapping
                    {
                        Vector3 coefs;
                        defaulttype::Vec3fTypes::Coord pos;
                        pos[0] = mapData[j].baryCoords[0];
                        pos[1] = mapData[j].baryCoords[1];
                        pos[2] = mapData[j].baryCoords[2];

                        // find nearest cell and barycentric coords
                        Real distance = 1e10;

                        int index = m_fromGeomAlgo->findNearestElementInRestPos ( pos, coefs, distance );

                        if ( index != -1 )
                        {
                            mapData[j].baryCoords[0] = ( Real ) coefs[0];
                            mapData[j].baryCoords[1] = ( Real ) coefs[1];
                            mapData[j].baryCoords[2] = ( Real ) coefs[2];
                            mapData[j].in_index = index;
                        }
                    }
                }

                d_map.endEdit();
                m_invalidIndex.clear();
            }
        }
        break;
        case core::topology::POINTSINDICESSWAP:  ///< For PointsIndicesSwap.
            break;
        case core::topology::POINTSADDED:        ///< For PointsAdded.
            break;
        case core::topology::POINTSREMOVED:      ///< For PointsRemoved.
            break;
        case core::topology::POINTSRENUMBERING:  ///< For PointsRenumbering.
            break;
        case core::topology::TRIANGLESADDED:  ///< For Triangles Added.
            break;
        case core::topology::TRIANGLESREMOVED:  ///< For Triangles Removed.
            break;
        case core::topology::HEXAHEDRAADDED:     ///< For HexahedraAdded.
        {
        }
        break;
        case core::topology::HEXAHEDRAREMOVED:   ///< For HexahedraRemoved.
        {
            const unsigned int nbHexahedra = this->m_fromTopology->getNbHexahedra();

            const sofa::helper::vector<unsigned int> &hexahedra =
                    ( static_cast< const sofa::core::topology::HexahedraRemoved *> ( *changeIt ) )->getArray();

            for ( unsigned int i=0; i<hexahedra.size(); ++i )
            {
                // remove all references to the removed cubes from the mapping data
                unsigned int cubeId = hexahedra[i];
                for ( unsigned int j=0; j<d_map.getValue().size(); ++j )
                {
                    if ( d_map.getValue()[j].in_index == ( int ) cubeId ) // invalidate mapping
                    {
                        Vector3 coefs;
                        coefs[0] = d_map.getValue()[j].baryCoords[0];
                        coefs[1] = d_map.getValue()[j].baryCoords[1];
                        coefs[2] = d_map.getValue()[j].baryCoords[2];

                        defaulttype::Vec3fTypes::Coord restPos = m_fromGeomAlgo->getRestPointPositionInHexahedron ( cubeId, coefs );

                        helper::vector<MappingData>& vectorData = *(d_map.beginEdit());
                        vectorData[j].in_index = -1;
                        vectorData[j].baryCoords[0] = restPos[0];
                        vectorData[j].baryCoords[1] = restPos[1];
                        vectorData[j].baryCoords[2] = restPos[2];
                        d_map.endEdit();

                        m_invalidIndex.insert(j);
                    }
                }
            }

            // renumber
            unsigned int lastCubeId = nbHexahedra-1;
            for ( unsigned int i=0; i<hexahedra.size(); ++i, --lastCubeId )
            {
                unsigned int cubeId = hexahedra[i];
                for ( unsigned int j=0; j<d_map.getValue().size(); ++j )
                {
                    if ( d_map.getValue()[j].in_index == ( int ) lastCubeId )
                    {
                        helper::vector<MappingData>& vectorData = *(d_map.beginEdit());
                        vectorData[j].in_index = cubeId;
                        d_map.endEdit();
                    }
                }
            }
        }
        break;
        case core::topology::HEXAHEDRARENUMBERING: ///< For HexahedraRenumbering.
            break;
        default:
            break;
        }
    }
}

#endif
#endif

#ifndef SOFA_FLOAT
template class SOFA_MISC_MAPPING_API BarycentricMapping< Vec3dTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperRegularGridTopology< Vec3dTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperSparseGridTopology< Vec3dTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperMeshTopology< Vec3dTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperEdgeSetTopology< Vec3dTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTriangleSetTopology< Vec3dTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperQuadSetTopology< Vec3dTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTetrahedronSetTopologyRigid< Vec3dTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTetrahedronSetTopology< Vec3dTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperHexahedronSetTopology< Vec3dTypes, Rigid3dTypes >;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_MISC_MAPPING_API BarycentricMapping< Vec3fTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperRegularGridTopology< Vec3fTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperSparseGridTopology< Vec3fTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperMeshTopology< Vec3fTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperEdgeSetTopology< Vec3fTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTriangleSetTopology< Vec3fTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperQuadSetTopology< Vec3fTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTetrahedronSetTopologyRigid< Vec3fTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTetrahedronSetTopology< Vec3fTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperHexahedronSetTopology< Vec3fTypes, Rigid3fTypes >;
#endif
#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE
template class SOFA_MISC_MAPPING_API BarycentricMapping< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapping< Vec3fTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperRegularGridTopology< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperRegularGridTopology< Vec3fTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperSparseGridTopology< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperSparseGridTopology< Vec3fTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperMeshTopology< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperMeshTopology< Vec3fTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTriangleSetTopology< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTriangleSetTopology< Vec3fTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperQuadSetTopology< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperQuadSetTopology< Vec3fTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTetrahedronSetTopologyRigid< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTetrahedronSetTopology< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTetrahedronSetTopologyRigid< Vec3fTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperTetrahedronSetTopology< Vec3fTypes, Rigid3dTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperHexahedronSetTopology< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapperHexahedronSetTopology< Vec3fTypes, Rigid3dTypes >;
#endif
#endif


namespace _topologybarycentricmapper_ {
#ifndef SOFA_FLOAT
template class SOFA_MISC_MAPPING_API TopologyBarycentricMapper< Vec3dTypes, Rigid3dTypes >;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_MISC_MAPPING_API TopologyBarycentricMapper< Vec3fTypes, Rigid3fTypes >;
#endif
#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE
template class SOFA_MISC_MAPPING_API TopologyBarycentricMapper< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API TopologyBarycentricMapper< Vec3fTypes, Rigid3dTypes >;
#endif
#endif
}

namespace _barycentricmapper_ {
#ifndef SOFA_FLOAT
template class SOFA_MISC_MAPPING_API BarycentricMapper< Vec3dTypes, Rigid3dTypes >;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_MISC_MAPPING_API BarycentricMapper< Vec3fTypes, Rigid3fTypes >;
#endif
#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE
template class SOFA_MISC_MAPPING_API BarycentricMapper< Vec3dTypes, Rigid3fTypes >;
template class SOFA_MISC_MAPPING_API BarycentricMapper< Vec3fTypes, Rigid3dTypes >;
#endif
#endif





}


} // namespace mapping

} // namespace component

} // namespace sofa

