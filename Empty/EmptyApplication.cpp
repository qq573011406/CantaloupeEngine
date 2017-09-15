#include "BaseApplication.hpp"
namespace Onion
{
    BaseApplication gBaseApp;
    IApplication* gApp = &gBaseApp;
}