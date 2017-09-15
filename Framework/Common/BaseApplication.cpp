#include "BaseApplication.hpp"

int Onion::BaseApplication::Initialize()
{
    mQuit = false;
    return 0;
}

void Onion::BaseApplication::Finalize()
{

}

void Onion::BaseApplication::Tick()
{

}

bool Onion::BaseApplication::IsQuit()
{
    return mQuit;
}