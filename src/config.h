#ifndef DATABASE_CONFIG_H
#define DATABASE_CONFIG_H

#include <cstdlib>

static const std::string PROJECT_ROOT (std::getenv("SCRATCHQL_ROOT"));
static const std::string DATABASE_DIR = "/var/db/scratchql/";

#endif //DATABASE_CONFIG_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */