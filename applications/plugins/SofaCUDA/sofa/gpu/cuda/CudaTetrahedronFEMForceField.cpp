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
#include "CudaTetrahedronFEMForceField.inl"
#include <sofa/core/behavior/ForceField.inl>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace forcefield
{
using namespace sofa::gpu::cuda;

template class TetrahedronFEMForceField<CudaVec3fTypes>;
template class TetrahedronFEMForceField<CudaVec3f1Types>;
#ifdef SOFA_GPU_CUDA_DOUBLE
template class TetrahedronFEMForceField<CudaVec3dTypes>;
template class TetrahedronFEMForceField<CudaVec3d1Types>;
#endif // SOFA_GPU_CUDA_DOUBLE

}

}

namespace gpu
{

namespace cuda
{

int TetrahedronFEMForceFieldCudaClass = core::RegisterObject("Supports GPU-side computations using CUDA")
        .add< component::forcefield::TetrahedronFEMForceField<CudaVec3fTypes> >()
        .add< component::forcefield::TetrahedronFEMForceField<CudaVec3f1Types> >()
#ifdef SOFA_GPU_CUDA_DOUBLE
        .add< component::forcefield::TetrahedronFEMForceField<CudaVec3dTypes> >()
        .add< component::forcefield::TetrahedronFEMForceField<CudaVec3d1Types> >()
#endif // SOFA_GPU_CUDA_DOUBLE
        ;

} // namespace cuda

} // namespace gpu

} // namespace sofa
