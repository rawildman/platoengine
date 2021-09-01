/*
 * MeshTransfer.hpp
 *
 *  Created on: Jul 28, 2017
 *      Author: tzirkle
 */

#ifndef ISO_PRUNE_MAIN_MESHTRANSFER_HPP_
#define ISO_PRUNE_MAIN_MESHTRANSFER_HPP_

#include "MeshManager.hpp"
#ifdef BUILD_IN_SIERRA
#include <percept/xfer/STKMeshTransferSetup.hpp>
#include <percept/PerceptMesh.hpp>
#else
#include <STKMeshTransferSetup.hpp>
#include <PerceptMesh.hpp>
#endif

class MeshTransfer
{
public:
    MeshTransfer();

    void transfer_mesh(MeshManager& aMeshManager);
};



#endif /* ISO_PRUNE_MAIN_MESHTRANSFER_HPP_ */
