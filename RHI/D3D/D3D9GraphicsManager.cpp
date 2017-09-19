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
		FLOAT x, y, z, rhw;
		DWORD color;
	};
	// Our custom FVF, which describes our custom vertex structure
	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

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
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;


    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &m_pD3DDevice ) ) )
        {
        return E_FAIL;
    }

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    return InitGeometry();
}

HRESULT Onion::D3D9GraphicsManager::InitVB()
{
	// Initialize three vertices for rendering a triangle
	CUSTOMVERTEX vertices[] =
	{
		{ 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
		{ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
		{ 50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
	};

	if (FAILED(m_pD3DDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT,
		&m_pVB, NULL)))
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

HRESULT Onion::D3D9GraphicsManager::InitGeometry()
{
	HRESULT hr = S_OK;

	m_pEffect = nullptr;
	m_pCode = nullptr;
	hr = D3DXCreateEffectFromFile(m_pD3DDevice, "Assets\\Shader\\diffuse.fx", NULL, NULL, 0, NULL, &m_pEffect, &m_pCode);
	if (FAILED(hr)) {
		LPVOID pBufErr = m_pCode->GetBufferPointer();
		char* ErrInfo = (char*)pBufErr;
		m_pCode->Release();
		return hr;
	}
	hr = m_pEffect->SetTechnique("Default");
	if (FAILED(hr)) return hr;
	return InitVB();
}

void Onion::D3D9GraphicsManager::SetupMatrices()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixRotationY(&matWorld, fAngle);
	//m_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	HRESULT hr = m_pEffect->SetMatrix("g_mWorld", &matWorld);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	// ¹Û²ì¾ØÕó  
	D3DXVECTOR3 vEyePt(15.0f, 3.0f, 0.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 3.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	//m_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	hr = m_pEffect->SetMatrix("g_mView", &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 64.0 / 41.0f, 1.0, 100.0f);
	//m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	hr = m_pEffect->SetMatrix("g_mView", &matProj);

}



void Onion::D3D9GraphicsManager::ClearupD3D()
{
	SafeRelease(&m_pCode);
	SafeRelease(&m_pEffect);
	SafeRelease(&m_pVB);
    SafeRelease(&m_pD3DDevice);
    SafeRelease(&m_pD3D);
}

void Onion::D3D9GraphicsManager::Render()
{

	if (m_pD3DDevice == nullptr) return;
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	HRESULT hr = S_OK;
    if( SUCCEEDED( m_pD3DDevice->BeginScene() ) )
    {
		hr = m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
		hr = m_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		SetupMatrices();
		UINT iPass=0, passCount=0;
		HRESULT hr = m_pEffect->Begin(&passCount, 0);

		for (iPass = 0;iPass<passCount;iPass++)
		{
			hr = m_pEffect->BeginPass(iPass);
			hr = m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
			hr = m_pEffect->EndPass();
		}

        m_pD3DDevice->EndScene();
    }

     // Present the backbuffer contents to the display
    m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}