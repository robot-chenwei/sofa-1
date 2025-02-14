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
#define FLEXIBLE_VoronoiShapeFunction_CPP

#include <Flexible/config.h>
#include "../shapeFunction/VoronoiShapeFunction.h"
#include <sofa/core/ObjectFactory.h>

namespace sofa
{
namespace component
{
namespace shapefunction
{

using namespace defaulttype;
using namespace core::behavior;

// Register in the Factory
int VoronoiShapeFunctionClass = core::RegisterObject("Computes natural neighbor shape functions in images")

        .add< VoronoiShapeFunction<ShapeFunction,ImageUC> >(true)
        .add< VoronoiShapeFunction<ShapeFunction,ImageD> >()
        .add< VoronoiShapeFunction<ShapeFunction,ImageB> >()
        .add< VoronoiShapeFunction<ShapeFunction,ImageF> >()
        ;

template class SOFA_Flexible_API VoronoiShapeFunction<ShapeFunction,ImageUC>;
template class SOFA_Flexible_API VoronoiShapeFunction<ShapeFunction,ImageD>;
template class SOFA_Flexible_API VoronoiShapeFunction<ShapeFunction,ImageB>;
template class SOFA_Flexible_API VoronoiShapeFunction<ShapeFunction,ImageF>;
}
}
}
