/*
 * MeshRefine.hpp
 *
 *  Created on: Jul 28, 2017
 *      Author: tzirkle
 */

#ifndef ISO_PRUNE_MAIN_MESHREFINE_HPP_
#define ISO_PRUNE_MAIN_MESHREFINE_HPP_

#include <adapt/UniformRefinerPattern.hpp>
#include <adapt/UniformRefiner.hpp>
#include <adapt/RefinerUtil.hpp>
#include <adapt/UniformRefinerPattern_def.hpp>

#include "MeshManager.hpp"
#ifdef BUILD_IN_SIERRA
#include <percept/PerceptMesh.hpp>
#else
#include <PerceptMesh.hpp>
#endif

class MeshRefine
{
public:
    MeshRefine();

    void refine_mesh(MeshManager &tMeshManager);
};



#endif /* ISO_PRUNE_MAIN_MESHREFINE_HPP_ */
