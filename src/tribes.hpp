#pragma once

#include "ids.hpp"

enum class tribe { none, mech, beast, demon, murloc, dragon, all };

struct minion;

auto get_tribe(id const& id) -> tribe;

auto get_tribe(minion const& m) -> tribe;
