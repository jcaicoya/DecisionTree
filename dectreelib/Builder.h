#pragma once

#include "Tree.h"
#include "Table.h"
#include <memory>


class Builder
{
public:
    using DecisionTree = Tree<std::string>;

    class Result
    {
    public:
        enum class Status { OK, NO_OK };

        Result(Status status, const DecisionTree &tree)
          : _status(status)
          , _tree(tree)
        {}

        Status getStatus() const { return _status; }
        DecisionTree getTree() { return std::move(_tree); }

    private:
        Status _status;
        DecisionTree _tree;
    };

    static Result Build(Table &&table);
};