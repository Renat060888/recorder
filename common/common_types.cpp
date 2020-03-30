
#include "common_types.h"

namespace common_types{

void SListenedTrajectory::accept( IListenedObjectVisitor * _visitor ) const {
    _visitor->visit( this );
}


}
