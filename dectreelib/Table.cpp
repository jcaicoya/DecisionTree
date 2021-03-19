#include "Table.h"
#include "Configuration.h"


void Table::setHeaders(Headers &&headers)
{
    _inputHeaders = headers;
    _inputColumns.resize(_inputHeaders.size());
}


bool Table::addRow(Row &&row)
{
    if (_inputHeaders.size() != row.size())
    {
        return false;
    }

    auto columnIterator = _inputColumns.begin();

    for (const auto &element : row)
    {
        columnIterator->push_back(element);
        ++columnIterator;
    }

    return true;
}


bool Table::setOutputAttributeIndex(size_t outputAttributeIndex)
{
    if (outputAttributeIndex >= _inputHeaders.size())
    {
        return false;
    }

    _outputHeader = _inputHeaders[outputAttributeIndex];
    _inputHeaders.erase(_inputHeaders.begin() + outputAttributeIndex);
    _outputColumn = _inputColumns[outputAttributeIndex];
    _inputColumns.erase(_inputColumns.begin() + outputAttributeIndex);
    return true;
}


size_t Table::getNumberOfRows() const
{
    if (_inputColumns.empty())
    {
        return 0;
    }

    return _inputColumns.begin()->size();
}


void Table::reserveInputHeaderAndColumnSize(std::size_t size)
{
    _inputHeaders.reserve(size);
    _inputColumns.reserve(size);
}


void Table::setOutputHeaderAndColumn(const Table::Header &header, Table::Column &&column)
{
    _outputHeader = header;
    _outputColumn = column;
}


void Table::addInputHeaderAndColumn(const Table::Header &header, Table::Column &&column)
{
    _inputHeaders.emplace_back(header);
    _inputColumns.emplace_back(column);
}