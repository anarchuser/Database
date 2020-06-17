#include "Table.h"

Table::Table (std::vector <Meta> const & meta, std::string const & dbname, std::string const & tablename):
    database {dbname},
    name {tablename}
{
    if (meta.empty()) THROW (std::invalid_argument ("Expected some columns; found none!"));
    for (auto const & column : meta) {
        sv::checkName (column.name);
        this->header.emplace_back (column.name);
        this->meta.push_back (column);
    }
    for (auto const & key : header) {
        table.insert (std::make_pair (key, std::vector<Cell>()));
    }
}

void Table::createRow (std::vector <Cell> const & row) {
    LOG (INFO) << "Creating Row in Table...";

    if (row.size() != columnCount()) {
        THROW (std::range_error ("Invalid amount of columns to insert"));
    }
    matrix.emplace_back();
    for (std::size_t col_index = 0; col_index < row.size(); col_index++) {
        table [header [col_index]].push_back (row [col_index]);
        matrix.back().push_back (& table [header [col_index]].back());
    }

    LOG (INFO) << "Created Row in Table.";
}
void Table::updateRow (std::size_t row_index, std::vector <Cell> const & row) {
    LOG (INFO) << "Updating Row in Table...";

    if (row.size() != columnCount ()) {
        THROW (std::range_error ("Invalid amount of columns to update"));
    }
    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for updateRow out of bounds"));
    }

    for (std::size_t col_index = 0; col_index < row.size(); col_index++) {
        table [header [col_index]] [row_index] = row [col_index];
        matrix [col_index] [row_index] = & table [header [col_index]] [row_index];
    }

    LOG (INFO) << "Updated Row in Table.";
}
std::unordered_map <std::string, Cell> Table::readRow (std::size_t row_index) const {
    LOG (INFO) << "Reading Row from Table...";

    std::vector <Cell> row_list = readRowAsVector (row_index);
    std::unordered_map <std::string, Cell> row_map;
    for (std::size_t idx = 0; idx < header.size(); idx++) {
        row_map.insert ({header [idx], row_list [idx]});
    }

    LOG (INFO) << "Read Row from Table.";

    return row_map;
}

// reads data from table in memory
std::vector <Cell> Table::readRowAsVector (std::size_t row_index) const{
    LOG (INFO) << "Reading Row as Vector from Table...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for readRow out of bounds"));
    }
    std::vector <Cell> row;

   for (auto const & key : header) {
       row.push_back (table.at (key) [row_index]);
   }
    LOG (INFO) << "Read Row as Vector from Table.";
    return row;
}
void Table::deleteRow (std::size_t row_index) {
    LOG (INFO) << "Deleting Row from Table...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for deleteRow out of bounds"));
    }

    matrix.erase (matrix.begin() + row_index);
    for (auto const & key : header) {
        table [key].erase (table [key].begin() + row_index);
    }

    LOG (INFO) << "Deleted Row from Table.";
}

bool Table::isRowEmpty(std::size_t row_index) const {
    LOG (INFO) << "Checking if Row in Table is empty...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for isRowEmpty out of bounds"));
    }

    for (const auto & key : header) {
        if (!isCellEmpty (key, row_index)) return false;
    }

    LOG (INFO) << "Checked Row from Table for emptiness.";

    return true;
}

bool Table::isCellEmpty (std::string const & key, std::size_t row_index) const {
    LOG (INFO) << "Checking if Cell is empty...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for isCellEmpty out of bounds"));
    }
    bool isEmpty = !table.at (key) [row_index];

    LOG (INFO) << "Checked Cell for emptiness.";

    return isEmpty;
}

std::vector <Cell> & Table::operator [] (std::string const & key) {
    return table.at (key);
}
const std::vector <Cell> & Table::operator [] (std::string const & key) const {
    return table.at (key);
}
std::unordered_map <std::string, Cell> Table::operator [] (std::size_t row_index) {
    return readRow (row_index);
}

std::vector <std::string> const & Table::getHeader() const {
    return header;
}
std::vector <Meta> const & Table::getMeta() const {
    return meta;
}
std::vector <std::size_t> Table::getColumnLengths () const {
    std::vector <std::size_t> lengths;
    for (auto const & element : meta) lengths.push_back (element.columnLength);
    return lengths;
}
std::vector <CellType> Table::getDataTypes () const {
    std::vector <CellType> types;
    for (auto const & element : meta) types.push_back (element.dataType);
    return types;
}
std::vector <std::vector <Cell const *>> const & Table::getContent() const {
    return matrix;
}
std::size_t Table::rowCount() const {
    return matrix.size();
}
std::size_t Table::columnCount() const {
    return meta.size();
}

bool Table::operator ! () const {
    return ! rowCount ();
}

std::ostream & operator << (std::ostream & os, Table const & table) {
    if (table.columnCount ()) {
        for (auto const & col : table.getHeader()) os << col << "\t";
        os << std::endl;
        if (table.rowCount ()) {
            os << std::endl;
            for (size_t row = 0; row < table.rowCount (); row++) {
                for (auto const & cell : table.readRowAsVector (row)) os << cell << "\t";
                os << std::endl;
            }
        }
    }
    return os;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */