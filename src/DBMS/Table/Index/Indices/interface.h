#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H

#include "../../../../config.h"

#include <variant>

namespace idx {
using Rows = std::variant <std::monostate, std::size_t, std::vector <std::size_t>>;
}

struct IndexImpl {
    virtual ~IndexImpl() = default;

    virtual bool insert (Cell cell, std::size_t row) = 0;
    virtual bool remove (Cell cell, std::size_t row) = 0;
    virtual idx::Rows select (Cell const & cell) const = 0;

    virtual std::ostream & operator << (std::ostream & os) const { return os << str(); }
    virtual std::string str() const = 0;
};

#endif //DATABASE_INTERFACE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */