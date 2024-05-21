/*
 * MeshPrune.cpp
 *
 *  Created on: Jul 28, 2017
 *      Author: tzirkle
 */
#include "MeshPrune.hpp"

#include "PruneMeshAPISTK.hpp"

void MeshPrune::prune_mesh(MeshManager &tMeshManager,int argc, char *argv[])
{
    tMeshManager.define_iso_fields();
    prune::PerceptPrune pruner;
    const int tNumberOfBufferLayers = tMeshManager.get_buffer_layers();
    const bool tAllowNonmanifoldConnections = tMeshManager.allow_nonmanifold_connections();
    if(pruner.import(argc,argv,"LSD","",1e-5,0.0,0,1,0,
                     tAllowNonmanifoldConnections,tNumberOfBufferLayers))
    {
        pruner.run_percept_mesh_stand_alone(tMeshManager);
    }
}


