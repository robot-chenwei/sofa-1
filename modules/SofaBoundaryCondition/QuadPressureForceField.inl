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
#ifndef SOFA_COMPONENT_FORCEFIELD_QUADPRESSUREFORCEFIELD_INL
#define SOFA_COMPONENT_FORCEFIELD_QUADPRESSUREFORCEFIELD_INL

#include <SofaBoundaryCondition/QuadPressureForceField.h>
#include <SofaBaseTopology/TopologySparseData.inl>
#include <sofa/core/visual/VisualParams.h>
#include <SofaBaseTopology/QuadSetGeometryAlgorithms.h>
#include <sofa/defaulttype/RGBAColor.h>
#include <vector>
#include <set>


namespace sofa
{

namespace component
{

namespace forcefield
{


template <class DataTypes> QuadPressureForceField<DataTypes>::~QuadPressureForceField()
{
}

template <class DataTypes>
QuadPressureForceField<DataTypes>::QuadPressureForceField()
    : pressure(initData(&pressure, "pressure", "Pressure force per unit area"))
    , quadList(initData(&quadList,"quadList", "Indices of quads separated with commas where a pressure is applied"))
    , normal(initData(&normal,"normal", "Normal direction for the plane selection of quads"))
    , dmin(initData(&dmin,(Real)0.0, "dmin", "Minimum distance from the origin along the normal direction"))
    , dmax(initData(&dmax,(Real)0.0, "dmax", "Maximum distance from the origin along the normal direction"))
    , p_showForces(initData(&p_showForces, (bool)false, "showForces", "draw quads which have a given pressure"))
    , quadPressureMap(initData(&quadPressureMap, "quadPressureMap", "map between edge indices and their pressure"))
{
}

template <class DataTypes>
void QuadPressureForceField<DataTypes>::init()
{
    this->core::behavior::ForceField<DataTypes>::init();

    _topology = this->getContext()->getMeshTopology();
    if(!_topology)
        serr << "Missing component: Unable to get MeshTopology from the current context. " << sendl;

    if(!_topology)
        return;

    if (dmin.getValue()!=dmax.getValue())
    {
        selectQuadsAlongPlane();
    }
    if (quadList.getValue().size()>0)
    {
        selectQuadsFromString();
    }

    quadPressureMap.createTopologicalEngine(_topology);
    quadPressureMap.registerTopologicalData();

    initQuadInformation();
}

template <class DataTypes>
void QuadPressureForceField<DataTypes>::addForce(const core::MechanicalParams* /* mparams */, DataVecDeriv& d_f, const DataVecCoord& /* d_x */, const DataVecDeriv& /* d_v */)
{
    VecDeriv& f = *d_f.beginEdit();
    Deriv force;

    const sofa::helper::vector <unsigned int>& my_map = quadPressureMap.getMap2Elements();
    const sofa::helper::vector<QuadPressureInformation>& my_subset = quadPressureMap.getValue();

    for (unsigned int i=0; i<my_map.size(); ++i)
    {
        force=my_subset[i].force/4;
        f[_topology->getQuad(my_map[i])[0]]+=force;
        f[_topology->getQuad(my_map[i])[1]]+=force;
        f[_topology->getQuad(my_map[i])[2]]+=force;
        f[_topology->getQuad(my_map[i])[3]]+=force;

    }
    d_f.endEdit();
    updateQuadInformation();
}


template<class DataTypes>
void QuadPressureForceField<DataTypes>::addDForce(const core::MechanicalParams* mparams, DataVecDeriv& /* d_df */, const DataVecDeriv& /* d_dx */)
{
    //Todo

    //Remove warning
    Real kFactor = (Real)mparams->kFactorIncludingRayleighDamping(this->rayleighStiffness.getValue());
    (void)kFactor;

    return;
}


template<class DataTypes>
void QuadPressureForceField<DataTypes>::initQuadInformation()
{
    sofa::component::topology::QuadSetGeometryAlgorithms<DataTypes>* quadGeo;
    this->getContext()->get(quadGeo);

    if(!quadGeo)
        serr << "Missing component: Unable to get QuadSetGeometryAlgorithms from the current context." << sendl;

    // FIXME: a dirty way to avoid a crash
    if(!quadGeo)
        return;

    const sofa::helper::vector <unsigned int>& my_map = quadPressureMap.getMap2Elements();
    sofa::helper::vector<QuadPressureInformation>& my_subset = *(quadPressureMap).beginEdit();

    for (unsigned int i=0; i<my_map.size(); ++i)
    {
        my_subset[i].area=quadGeo->computeRestQuadArea(my_map[i]);
        my_subset[i].force=pressure.getValue()*my_subset[i].area;
    }

    quadPressureMap.endEdit();
}


template<class DataTypes>
void QuadPressureForceField<DataTypes>::updateQuadInformation()
{
    sofa::helper::vector<QuadPressureInformation>& my_subset = *(quadPressureMap).beginEdit();

    for (unsigned int i=0; i<my_subset.size(); ++i)
        my_subset[i].force=(pressure.getValue()*my_subset[i].area);

    quadPressureMap.endEdit();
}


template <class DataTypes>
void QuadPressureForceField<DataTypes>::selectQuadsAlongPlane()
{
    const VecCoord& x = this->mstate->read(core::ConstVecCoordId::restPosition())->getValue();
    std::vector<bool> vArray;
    unsigned int i;

    vArray.resize(x.size());

    for( i=0; i<x.size(); ++i)
    {
        vArray[i]=isPointInPlane(x[i]);
    }

    sofa::helper::vector<QuadPressureInformation>& my_subset = *(quadPressureMap).beginEdit();
    helper::vector<unsigned int> inputQuads;

    for (int n=0; n<_topology->getNbQuads(); ++n)
    {
        if ((vArray[_topology->getQuad(n)[0]]) && (vArray[_topology->getQuad(n)[1]])&& (vArray[_topology->getQuad(n)[2]])&& (vArray[_topology->getQuad(n)[3]]) )
        {
            // insert a dummy element : computation of pressure done later
            QuadPressureInformation q;
            q.area = 0;
            my_subset.push_back(q);
            inputQuads.push_back(n);
        }
    }
    quadPressureMap.endEdit();
    quadPressureMap.setMap2Elements(inputQuads);

    return;
}


template <class DataTypes>
void QuadPressureForceField<DataTypes>::selectQuadsFromString()
{
    sofa::helper::vector<QuadPressureInformation>& my_subset = *(quadPressureMap).beginEdit();
    helper::vector<unsigned int> _quadList = quadList.getValue();

    quadPressureMap.setMap2Elements(_quadList);

    for (unsigned int i = 0; i < _quadList.size(); ++i)
    {
        QuadPressureInformation q;
        q.area = 0;
        my_subset.push_back(q);
    }

    quadPressureMap.endEdit();

    return;
}

template <class DataTypes>
bool QuadPressureForceField<DataTypes>::isPointInPlane(Coord p)
{
    Real d=dot(p,normal.getValue());
    if ((d>dmin.getValue())&& (d<dmax.getValue()))
        return true;
    else
        return false;
}

template<class DataTypes>
void QuadPressureForceField<DataTypes>::draw(const core::visual::VisualParams* vparams)
{
    vparams->drawTool()->saveLastState();

    if (!p_showForces.getValue())
        return;

    if (vparams->displayFlags().getShowWireFrame())
        vparams->drawTool()->setPolygonMode(0, true);

    const VecCoord& x = this->mstate->read(core::ConstVecCoordId::position())->getValue();

    vparams->drawTool()->disableLighting();
    std::vector<sofa::defaulttype::Vector3> vertices;
    sofa::defaulttype::RGBAColor color = sofa::defaulttype::RGBAColor::green();

    const sofa::helper::vector <unsigned int>& my_map = quadPressureMap.getMap2Elements();

    for (unsigned int i=0; i<my_map.size(); ++i)
    {
        for(unsigned int j=0 ; j<4 ; j++)
            vertices.push_back(x[_topology->getQuad(my_map[i])[j]]);
    }
    vparams->drawTool()->drawQuads(vertices, color);


    if (vparams->displayFlags().getShowWireFrame())
        vparams->drawTool()->setPolygonMode(0, false);

    vparams->drawTool()->saveLastState();
}

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_FORCEFIELD_QUADPRESSUREFORCEFIELD_INL
