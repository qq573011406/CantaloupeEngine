#include "BaseApplication.hpp"

using namespace Onion;

bool Onion::BaseApplication::m_bQuit = false;
Onion::BaseApplication::BaseApplication(GfxConfiguration& cfg)
    :m_Config(cfg)
{

}
int Onion::BaseApplication::Initialize()
{
    m_bQuit = false;
    std::cout<<m_Config;
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
    return m_bQuit;
}

GfxConfiguration Onion::BaseApplication::GetConfiguration()
{
    return m_Config;
}