#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "components/Looking.h"
#include "components/Positioned.h"
#include "world/model/Entity.h"

class MainPlayer :
    public Entity,
    public Looking,
    public Positioned {
};

#endif //MAIN_PLAYER_H
