#ifndef DATABASE_CONFIG_H
#define DATABASE_CONFIG_H

/// Minimum LOG level. 0 = Everything, 1 = Ignore LOG (INFO), ...
#define GOOGLE_STRIP_LOG 0

#define THROW(error)                \
    LOG (ERROR) << error.what();    \
    throw error

#include <string>
#include <cstdlib>
#include <variant>
#include <glog/logging.h>
#include <netinet/in.h>

#include "DBMS/Cell/Cell.h"

#define IP_ADDRESS_FAMILY AF_INET
#define ADDRESS "localhost"

static const std::string PROJECT_ROOT (std::getenv("SCRATCHQL_ROOT"));

enum KeyTypes {
    NORMAL,
    PRIMARY,
    UNIQUE,
    FOREIGN,
    INDEXED,
};

static const std::string validQueryChars (" <>!=+-*/{}[]().,;@#");

auto const STR = std::string();


#endif //DATABASE_CONFIG_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */