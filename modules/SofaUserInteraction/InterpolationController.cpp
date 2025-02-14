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
#define SOFA_COMPONENT_FORCEFIELD_INTERPOLATIONCONTROLLER_CPP

#include <SofaUserInteraction/InterpolationController.inl>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace controller
{

using namespace sofa::defaulttype;

int InterpolationControllerClass = core::RegisterObject("Interpolates nodes between two meshes")
#ifndef SOFA_FLOAT
.add< InterpolationController<Vec3dTypes> > ()
.add< InterpolationController<Rigid3dTypes> >()
#endif
#ifndef SOFA_DOUBLE
.add< InterpolationController<Vec3fTypes> > ()
.add< InterpolationController<Rigid3fTypes> >()
#endif
;

#ifndef SOFA_FLOAT
template class SOFA_USER_INTERACTION_API InterpolationController<Vec3dTypes>;
template class SOFA_USER_INTERACTION_API InterpolationController<Rigid3dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_USER_INTERACTION_API InterpolationController<Vec3fTypes>;
template class SOFA_USER_INTERACTION_API InterpolationController<Rigid3fTypes>;
#endif

} // namespace forcefield

} // namespace component

} // namespace sofa
