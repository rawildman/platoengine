#include <iostream>
#include <algorithm>
#include <sstream>

#include "Plato_Exceptions.hpp"
#include "Plato_FunctionOperation.hpp"
#include "Plato_Performer.hpp"
#include "Plato_SharedData.hpp"
#include "Plato_Utils.hpp"
#include "Plato_OperationInputDataMng.hpp"
#include "Plato_EnumTable.hpp"

namespace Plato {

namespace {
std::string unrecognizedOperationErrorMessage(
    const std::string& aErroneousOperation, 
    const Performer& aPerformer)
{
    std::string errorMessage = R"(Unknown operation ")" + aErroneousOperation + R"(". Performer supports: )";
    for(const OperationType operation : aPerformer.supportedOperationTypes())
    {
        errorMessage += "\n  " + operationTypeName(operation);
    }
    return errorMessage;
}
}

/******************************************************************************/
FunctionOperation::
FunctionOperation(const Plato::OperationInputDataMng & aOperationDataMng,
               const std::shared_ptr<Plato::Performer> aPerformer,
               const std::vector<Plato::SharedData*>& aSharedData) :
  Operation(aOperationDataMng, aPerformer, aSharedData)
/******************************************************************************/
{
    initialize(aOperationDataMng, aPerformer, aSharedData);
}

/******************************************************************************/
void
FunctionOperation::
initialize(const Plato::OperationInputDataMng & aOperationDataMng,
           const std::shared_ptr<Plato::Performer> aPerformer,
           const std::vector<Plato::SharedData*>& aSharedData)
/******************************************************************************/
{
    initializeBaseSingle(aOperationDataMng, aPerformer, aSharedData);
    const boost::optional<OperationType> tOperation = 
        aOperationDataMng.getOperationType(aPerformer->myName());
    if(!tOperation)
    {
        // todo: Is ParsingException the correct exception type?
        throw ParsingException(unrecognizedOperationErrorMessage(
            aOperationDataMng.getOperationName(aPerformer->myName()),
            *aPerformer
        ));
    }
    mComputeFunction = Performer::computeFunction(*tOperation);
}

void FunctionOperation::computeImpl() 
{
    mComputeFunction(*m_performer);
}

/******************************************************************************/
void FunctionOperation::
update(const Plato::OperationInputDataMng & aOperationDataMng,
       const std::shared_ptr<Plato::Performer> aPerformer,
       const std::vector<Plato::SharedData*>& aSharedData)
/******************************************************************************/
{
    // If the shared data is recreated then the operation must be
    // updated so to have the new links to the shared data.
    initialize(aOperationDataMng, aPerformer, aSharedData);
}

} // End namespace Plato
