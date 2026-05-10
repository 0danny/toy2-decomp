#pragma once

#include <cstdint>
#include <iterator>

static const char* CreatureIdNames[] =
{
    "CREATURE_BUZZ",
    "CREATURE_WOODY",
    "CREATURE_BUZZINT",
    "CREATURE_ZURG1",
    "CREATURE_ZURG3",
    "CREATURE_TINMAN",
    "CREATURE_SHEEP",
    "CREATURE_BOPEEP",
    "CREATURE_RC",
    "CREATURE_MRPOT",
    "CREATURE_HAMM",
    "CREATURE_COTBIT",
    "CREATURE_2_LAWN",
    "CREATURE_ARMY",
    "CREATURE_2_ZGCAR",
    "CREATURE_2_ZKITE",
    "CREATURE_2_SLIME",
    "CREATURE_2_PAINT",
    "CREATURE_2_FTYKE",
    "CREATURE_2_LTYKE",
    "CREATURE_2_ZPOD",
    "CREATURE_2_SHINY",
    "CREATURE_2_DRILL",
    "CREATURE_2_MOUSE",
    "CREATURE_3_BPLANE",
    "CREATURE_3_BOX",
    "CREATURE_3_DINO",
    "CREATURE_2_ZBOAT",
    "CREATURE_3_CHICK",
    "CREATURE_3_HOOLA",
    "CREATURE_3_LOCK",
    "CREATURE_3_GUNSP",
    "CREATURE_3_CLOWN",
    "CREATURE_3_RATTLE",
    "CREATURE_3_MUM",
    "CREATURE_3_ROOST",
    "CREATURE_4_MOTHER",
    "CREATURE_4_FSAUCE",
    "CREATURE_4_MARTIAN",
    "CREATURE_4_JESSIE",
    "CREATURE_6_RABID",
    "CREATURE_3_BUZZARD",
    "CREATURE_4_BULLSEYE",
    "CREATURE_4_DUCKS",
    "CREATURE_4_SLINKY",
    "CREATURE_4_GUNSL",
    "CREATURE_4_FATBLOKE",
    "CREATURE_5_BBUGGY",
    "CREATURE_5_BI",
    "CREATURE_4_ROCKY",
    "CREATURE_4_GEORGE",
    "CREATURE_4_MILDRED",
    "CREATURE_4_KAREN",
    "CREATURE_4_PILOT",
    "CREATURE_5_ZPOD",
    "CREATURE_5_BUB",
    "CREATURE_5_BUZL",
    "CREATURE_5_ZEMP",
    "CREATURE_5_SMITH",
    "CREATURE_5_LUGGAGE",
    "CREATURE_6_LUGMAN",
    "CREATURE_6_PROSP",
    "CREATURE_6_REX",
};

constexpr int32_t kCreatureInvalid = 0xFF;
constexpr const char* kUnknownCreature = "CREATURE_UNKNOWN";

inline const char* GetCreatureName(int32_t id)
{
    if (id == kCreatureInvalid)
        return "CREATURE_INVALID";

    if (id < std::size(CreatureIdNames))
        return CreatureIdNames[id];

    return "CREATURE_UNKNOWN";
}