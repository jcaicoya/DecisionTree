#pragma once

#include <string>
#include <vector>
#include <mutex>


class Table
{
public:
    using Header = std::string;
    using Headers = std::vector<Header>;
    using Element = std::string;
    using Row = std::vector<Element>;
    using Column = std::vector<Element>;
    using Columns = std::vector<Column>;

    Table() = default;

    void setHeaders(Headers &&headers);
    bool addRow(Row &&row);
    bool setOutputAttributeIndex(size_t outputAttributeIndex);
    size_t getNumberOfRows() const;

    void reserveInputHeaderAndColumnSize(std::size_t size);
    void setOutputHeaderAndColumn(const Header &header, Column &&column);
    void addInputHeaderAndColumn(const Header &header, Column &&column);

    const Header &getOutputHeader() const { return _outputHeader; }
    const Column &getOutputColumn() const { return _outputColumn; }
    size_t getNumberOfInputHeaders() const { return _inputHeaders.size(); }
    const Header &getInputHeader(std::size_t pos) const { return _inputHeaders[pos]; }
    const Headers &getInputHeaders() const { return _inputHeaders; }
    const Column &getInputColumn(std::size_t pos) const { return _inputColumns[pos]; }
    const Columns &getInputColumns() const { return _inputColumns; }


private:
    Header _outputHeader;
    Column _outputColumn;
    Headers _inputHeaders;
    Columns _inputColumns;
};