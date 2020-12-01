#ifndef SAILINSANE_SINGLETONS_H
#define SAILINSANE_SINGLETONS_H

#include <gf/Singleton.h>
#include <gf/ResourceManager.h>
#include <gf/Random.h>

namespace sail
{

    extern gf::Singleton<gf::ResourceManager> gResourceManager;

}

#endif //SAILINSANE_SINGLETONS_H
