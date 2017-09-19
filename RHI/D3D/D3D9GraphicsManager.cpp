#include "D3D9GraphicsManager.hpp"
#include "WindowsApplication.hpp"
#include <mmsystem.h>
using namespace Onion;
namespace Onion
{
    extern IApplication* g_pApp;
    
    template<class T>
    inline void SafeRelease(T **ppInterfaceToRelease)
    {
        if (*ppInterfaceToRelease != nullptr)
        {
            (*ppInterfaceToRelease)->Release();

            (*ppInterfaceToRelease) = nullptr;
        }
    }


	struct CUSTOMVERTEX
	{
		FLOAT x, y, z;
		DWORD color;
	};
	// Our custom FVF, which describes our custom vertex structure
	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
}

Onion::D3D9GraphicsManager::D3D9GraphicsManager():
    m_pD3D(nullptr),
    m_pD3DDevice(nullptr)
{

}

int Onion::D3D9GraphicsManager::Initialize()
{
    int result = 0;
    result = GraphicsManager::Initialize();
    if(result!=0)return result;

    result = static_cast<int>(InitD3D());
    return result;
}

void Onion::D3D9GraphicsManager::Tick()
{
    Render();
}

void Onion::D3D9GraphicsManager::Finalize()
{
    ClearupD3D();
}


HRESULT Onion::D3D9GraphicsManager::InitD3D()
{


    HWND hWnd = reinterpret_cast<WindowsApplication*>(g_pApp)->GetMainWindow();


    // Create the D3D object, which is needed to create the D3DDevice.
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(m_pD3D == nullptr) return E_FAIL;
    
    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp,sizeof(d3dpp));

    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &m_pD3DDevice ) ) )
        {
        return E_FAIL;
    }

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    return InitVB();
}

HRESULT Onion::D3D9GraphicsManager::InitVB()
{
	// Initialize three vertices for rendering a triangle
	CUSTOMVERTEX vertices[] =
	{
		{ -1.0f,-1.0f, 0.0f, 0xffff0000, },
		{ 1.0f,-1.0f, 0.0f, 0xff0000ff, },
		{ 0.0f, 1.0f, 0.0f, 0xffffffff, },
	};


	if (FAILED(m_pD3DDevice->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX),
		0,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT,
		&m_pVB,NULL)))
	{
		return E_FAIL;
	}

	VOID* pVertices = nullptr;
	if (FAILED(m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();
	return S_OK;
}

void Onion::D3D9GraphicsManager::SetupMatrices()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	//D3DXMATRIXA16 matWorld;
}



void Onion::D3D9GraphicsManager::ClearupD3D()
{
	SafeRelease(&m_pVB);
    SafeRelease(&m_pD3DDevice);
    SafeRelease(&m_pD3D);
}

void Onion::D3D9GraphicsManager::Render()
{
    if(m_pD3DDevice == nullptr) return;
    m_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB( 0, 0, 255 ),1.0f,0);
    // Begin the scene
    if( SUCCEEDED( m_pD3DDevice->BeginScene() ) )
    {
        // Rendering of scene objects can happen here
		m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
		m_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
        // End the scene
        m_pD3DDevice->EndScene();
    }

     // Present the backbuffer contents to the display
    m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}