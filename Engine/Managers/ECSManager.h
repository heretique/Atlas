#ifndef ECSMANAGER_H
#define ECSMANAGER_H

#include <entityx/entityx.h>

namespace atlas {

class ECSManager
{
public:
    ECSManager() :  _entities(_events), _systems(_entities, _events) {}

    void installSystems();
    void update(entityx::TimeDelta dt);

private:
    entityx::EventManager _events;
    entityx::EntityManager _entities;
    entityx::SystemManager _systems;
};

} // atlas



#endif // ECSMANAGER_H
