#ifndef EVAL_MONTE_H_
#define EVAL_MONTE_H_

#include "../game/card.h"

#include <cstdint>
#include <utility>

namespace eval {

class Simulator {
public:
    using Results = std::pair<std::size_t, std::size_t>;

    Results operator()(const std::pair<game::Card, game::Card>& hand, std::size_t tableSize, std::size_t runs) const;
};

}

#endif