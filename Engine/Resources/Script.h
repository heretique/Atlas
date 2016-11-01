#ifndef SCRIPTRESOURCE_H
#define SCRIPTRESOURCE_H

#include "wrenpp/Wren++.h"

namespace atlas {

class ScriptAsset : public Asset
{
public:
    static Asset *factoryFunc( const std::string &name, int flags ) { return new ScriptAsset( name, flags ); }
    static void releaseFunc(Asset* asset) { delete asset; asset = nullptr; }


    ScriptAsset( const std::string &name, int flags );
    ~ScriptAsset();
    Asset *clone();

    void initDefault();
    void release();
    bool load( const char *data, int size );

private:

};

} // atlas


#endif // SCRIPTRESOURCE_H
