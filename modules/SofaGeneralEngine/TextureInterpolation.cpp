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
#define SOFA_COMPONENT_ENGINE_TEXTUREINTERPOLATION_CPP
#include <SofaGeneralEngine/TextureInterpolation.inl>
#include <sofa/core/ObjectFactory.h>
#include <sofa/defaulttype/Vec3Types.h>

namespace sofa
{

namespace component
{

namespace engine
{

using namespace sofa::defaulttype;

int TextureInterpolationClass = core::RegisterObject("Create texture coordinate for a given field")
#ifndef SOFA_FLOAT
        .add< TextureInterpolation <Vec1dTypes> >()
        .add< TextureInterpolation <Vec2dTypes> >()
        .add< TextureInterpolation <Vec3dTypes> >()
#endif //SOFA_FLOAT
#ifndef SOFA_DOUBLE
        .add< TextureInterpolation <Vec1fTypes> >()
        .add< TextureInterpolation <Vec2fTypes> >()
        .add< TextureInterpolation <Vec3fTypes> >()
#endif //SOFA_DOUBLE
        ;

#ifndef SOFA_FLOAT
template class SOFA_GENERAL_ENGINE_API TextureInterpolation <Vec1dTypes>;
template class SOFA_GENERAL_ENGINE_API TextureInterpolation <Vec2dTypes>;
template class SOFA_GENERAL_ENGINE_API TextureInterpolation <Vec3dTypes>;
#endif //SOFA_FLOAT
#ifndef SOFA_DOUBLE
template class SOFA_GENERAL_ENGINE_API TextureInterpolation <Vec1fTypes>;
template class SOFA_GENERAL_ENGINE_API TextureInterpolation <Vec2fTypes>;
template class SOFA_GENERAL_ENGINE_API TextureInterpolation <Vec3fTypes>;
#endif //SOFA_DOUBLE


} // namespace constraint

} // namespace component

} // namespace sofa

