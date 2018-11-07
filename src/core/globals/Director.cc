#include "Director.h"

namespace wkt
{

Director& Director::getInstance()
{
    static Director director;
    return director;
}

}