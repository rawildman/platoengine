/*
//@HEADER
// *************************************************************************
//   Plato Engine v.1.0: Copyright 2018, National Technology & Engineering
//                    Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Sandia Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact the Plato team (plato3D-help@sandia.gov)
//
// *************************************************************************
//@HEADER
*/

/*
 * Plato_DecomposeMesh.cpp
 *
 */

#ifdef STK_ENABLED
#include "Plato_DecomposeMesh.hpp"
#include <stk_io/StkMeshIoBroker.hpp>
#endif

#include <string>

namespace Plato
{

void decomposeMesh(MPI_Comm aComm, const std::string& aMeshFile)
{
#ifdef STK_ENABLED
    int tMyRank;
    int tSize;
    MPI_Comm_rank(aComm, &tMyRank);
    MPI_Comm_size(aComm, &tSize);

    // Read the serial mesh
    stk::io::StkMeshIoBroker ioBroker(aComm);
    ioBroker.property_add(Ioss::Property("DECOMPOSITION_METHOD", "RCB"));
    ioBroker.add_mesh_database(aMeshFile, stk::io::READ_MESH);
    ioBroker.create_input_mesh();
    ioBroker.populate_bulk_data();

    // Write the spread file.
    size_t index = ioBroker.create_output_mesh(aMeshFile, stk::io::WRITE_RESULTS);
    ioBroker.set_active_mesh(index);
    ioBroker.write_output_mesh(index);

    MPI_Barrier(aComm);
#endif
}


} // end namespace Plato
