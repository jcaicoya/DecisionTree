#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include <stack>


template<typename T>
class Node
{
public:
    Node(const T &value)
      : _value(value)
      , _children()
    {}

    const T &getValue() const { return _value; }
    size_t getNumberOfChildren() const { return _children.size(); }
    std::shared_ptr<Node<T>> getNthChild(size_t n) const { return _children[n]; }
    void addChild(const std::shared_ptr<Node<T>> &child) { _children.push_back(child); }
    void addChild(const T &value) { _children.push_back(std::make_shared<Node<T>>(value)); }
    bool isLeaf() const { return _children.empty(); }

private:
    const T _value;
    std::vector<std::shared_ptr<Node<T>>> _children;
};


template<typename T>
class Tree
{
public:
    Tree(std::shared_ptr<Node<T>> root)
      : _root(root)
    {}

    Tree(const T &value)
      : _root(std::make_shared<Node<T>>(value))
    {}

    std::shared_ptr<Node<T>> getRoot() const { return _root; }

    void print(std::ostream &out = std::cout) const { return Print(_root, out); }
    std::string toString() const { return ToString(_root); }
    inline size_t deep() const { return Deep(_root); }

private:
    std::shared_ptr<Node<T>> _root;

    inline static void Print(std::shared_ptr<Node<T>> currentNode, std::ostream &out = std::cout);
    inline static std::string ToString(std::shared_ptr<Node<T>> currentNode);
    inline static size_t Deep(std::shared_ptr<Node<T>> currentNode);
};


template<typename T>
void Tree<T>::Print(std::shared_ptr<Node<T>> currentNode, std::ostream &out)
{
    if (currentNode->isLeaf())
    {
        out << currentNode->getValue() << '\n';
        return;
    }

    for (size_t ii = 0; ii < currentNode->getNumberOfChildren(); ii++)
    {
        auto childNode = currentNode->getNthChild(ii);
        if (!childNode)
        {
            out << ii << "-child of " << currentNode->getValue() << " is null!\n";
        }
        else
        {
            Print(currentNode->getNthChild(ii), out);
        }
    }

    /*
        std::stack<std::shared_ptr<Node<std::string>>> nodeStack;
        nodeStack.push(currentNode);
        while (!nodeStack.empty())
        {
            std::shared_ptr<Node<std::string>> currentNode = nodeStack.top();
            if (!currentNode)
            {
                continue;
            }

            nodeStack.pop();
            if (currentNode->isLeaf())
            {
                out << currentNode->getValue() << '\n';
            }
            else
            {
                for (size_t ii = 0; ii < currentNode->getNumberOfChildren(); ii++)
                {
                    auto childNode = currentNode->getNthChild(ii);
                    if (!childNode)
                    {
                        continue;
                    }
                    nodeStack.push(currentNode->getNthChild(ii));
                }
            }
        }
        */
}


template<typename T>
std::string Tree<T>::ToString(std::shared_ptr<Node<T>> currentNode)
{
    if (currentNode->isLeaf())
    {
        return currentNode->getValue().toString();
    }

    std::string result;

    for (size_t ii = 0; ii < currentNode->getNumberOfChildren(); ii++)
    {
        result += ToString(currentNode->getNthChild(ii));
        result += '\n';
    }

    return result;
}


template<>
inline std::string Tree<std::string>::ToString(std::shared_ptr<Node<std::string>> currentNode)
{
    if (currentNode->isLeaf())
    {
        return currentNode->getValue() + '\n';
    }

    std::string result;

    for (size_t ii = 0; ii < currentNode->getNumberOfChildren(); ii++)
    {
        result += ToString(currentNode->getNthChild(ii));
    }

    return result;
}


template<typename T>
size_t Tree<T>::Deep(std::shared_ptr<Node<T>> currentNode)
{
    if (currentNode->isLeaf())
    {
        return 0;
    }

    size_t maxDeep = 0;
    for (size_t ii = 0; ii < currentNode->getNumberOfChildren(); ii++)
    {
        size_t childDeep = Deep(currentNode->getNthChild(ii));
        if (childDeep > maxDeep)
        {
            maxDeep = childDeep;
        }
    }

    return maxDeep + 1;
}