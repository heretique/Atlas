#ifndef SCRIPTRESOURCE_H
#define SCRIPTRESOURCE_H

#include "wrenpp/Wren++.h"

namespace atlas {

class ScriptResource : public Resource
{
public:
    static void initializationFunc();
    static void releaseFunc();
    static Resource *factoryFunc( const std::string &name, int flags )
        { return new ScriptResource( name, flags ); }

    ScriptResource( const std::string &name, int flags );
    ~ScriptResource();
    Resource *clone();

    void initDefault();
    void release();
    bool load( const char *data, int size );

private:

};

} // atlas


#endif // SCRIPTRESOURCE_H
