#pragma once

#include <bitset>

using std::bitset;

const unsigned int FIVI_ECS_MaxComponents = 32;

/**
 * @brief Signature
 * @details Bitset of 1's and 0's. Identifier for what components are owned by an entity and managed by a specific system.
 */
typedef bitset<FIVI_ECS_MaxComponents> Signature;