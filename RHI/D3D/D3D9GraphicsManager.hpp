#pragma once
#include <stdint.h>
#include <d3d9.h>
#include <strsafe.h>
#include "GraphicsManager.hpp"

namespace Onion
{
    class D3D9GraphicsManager : public GraphicsManager
    {
    public:
        D3D9GraphicsManager();
        virtual int Initialize();
	    virtual void Finalize();
        virtual void Tick();
    
    private:
        HRESULT InitD3D();
		HRESULT InitVB();
        void ClearupD3D();
        void Render();
    private:
        LPDIRECT3D9 m_pD3D;
        LPDIRECT3DDEVICE9 m_pD3DDevice;
		LPDIRECT3DVERTEXBUFFER9 m_pVB; // Buffer to hold vertices
    };
}