#include "Builder.h"
#include "BuilderImpl.h"


Builder::Result Builder::Build(Table &&table)
{
    auto node = BuilderImpl::BuildNode("", table);
    if (!node)
    {
        return Result(Result::Status::NO_OK, DecisionTree(node));
    }

    return Result(Result::Status::OK, DecisionTree(node));
}