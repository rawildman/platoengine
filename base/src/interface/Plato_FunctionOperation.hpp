#ifndef SRC_FUNCTION_OPERATION_HPP_
#define SRC_FUNCTION_OPERATION_HPP_

#include <map>
#include <string>
#include <vector>
#include <functional>

#include "Plato_Operation.hpp"

namespace Plato
{

class Performer;
class SharedData;
class OperationInputDataMng;

/// 
class FunctionOperation final : public Operation
{
public:
    FunctionOperation(const Plato::OperationInputDataMng & aOperationDataMng,
                    const std::shared_ptr<Plato::Performer> aPerformer,
                    const std::vector<Plato::SharedData*>& aSharedData);

    void update(const ::Plato::OperationInputDataMng & aOperationDataMng,
                        const std::shared_ptr<::Plato::Performer> aPerformer,
                        const std::vector<::Plato::SharedData*>& aSharedData) override;

    void initialize(const ::Plato::OperationInputDataMng & aOperationDataMng,
                    const std::shared_ptr<::Plato::Performer> aPerformer,
                    const std::vector<::Plato::SharedData*>& aSharedData);

protected:
    void computeImpl() override;

private:
    std::function<void(Performer&)> mComputeFunction;
};

} // End namespace Plato

#endif
