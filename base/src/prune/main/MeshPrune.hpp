/*
 * MeshPrune.hpp
 *
 *  Created on: Jul 28, 2017
 *      Author: tzirkle
 */

#ifndef ISO_PRUNE_MAIN_MESHPRUNE_HPP_
#define ISO_PRUNE_MAIN_MESHPRUNE_HPP_

#include "MeshManager.hpp"
#ifdef BUILD_IN_SIERRA
#include <percept/PerceptMesh.hpp>
#else
#include <PerceptMesh.hpp>
#endif
#include "PerceptPrune.hpp"

class MeshPrune
{
public:
    MeshPrune();

    void prune_mesh(MeshManager &tMeshManager,int argc, char *argv[],int tLastPrune);

};



#endif /* ISO_PRUNE_MAIN_MESHPRUNE_HPP_ */
