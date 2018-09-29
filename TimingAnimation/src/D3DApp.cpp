#include <windows.h>
#include "D3DApp.h"
#include "GameTimer.h"

namespace
{
  D3DApp* gd3dApp = nullptr;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  return gd3dApp->MsgProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance)
  :mhAppInst(),
  mMainWndCaption(L"D3D11 Application"),
  mClientWidth(1280),
  mClientHeight(720),
  mEnable4xMsaa(false),
  mhMainWnd(nullptr),
  mAppPaused(false),
  mMinimized(false),
  mMaximized(false),
  mResizing(false),
  m4xMsaaQuality(0),
  md3dDevice(nullptr),
  md3dImmediateContext(nullptr),
  mSwapChain(nullptr),
  mDepthStencilBuffer(nullptr),
  mRenderTargetView(nullptr),
  mDepthStencilView(nullptr)
{
  ZeroMemory(&mScreenViewport, 0, sizeof(D3D11_VIEWPORT));

  gd3dApp = this;
}

D3DApp::~D3DApp()
{
  ReleaseCOM(mRenderTargetView);
  ReleaseCOM(mDepthStencilView);
  ReleaseCOM(mDepthStencilBuffer);
  ReleaseCOM(mSwapChain);
  if (md3dImmediateContext)
  {
    md3dImmediateContext->ClearState();
  }
  ReleaseCOM(md3dImmediateContext);
  ReleaseCOM(md3dDevice);
}

HINSTANCE D3DApp::AppInst() const
{
  return mhAppInst;
}

HWND D3DApp::MainWnd() const
{
  return mhMainWnd;
}

float D3DApp::AspectRatio() const
{
  return static_cast<float>(mClientWidth) / mClientHeight;
}

int D3DApp::Run()
{
  MSG msg = {0};

  mTimer.Reset();

  while (msg.message != WM_QUIT)
  {
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      mTimer.Tick();

      if (!mAppPaused)
      {
        CalculateFrameStats();
        UpdateScene(mTimer.DeltaTime());
        DrawScene();
      }
      else
      {
        Sleep(100);
      }
    }
  }
}

bool D3DApp::Init()
{
  if (!InitMainWindow())
  {
    return false;
  }

  if (!InitDirect3D())
  {
    return false;
  }

  return true;
} 

void D3DApp::OnReSize()
{
  assert(md3dImmediateContext);
  assert(mSwapChain);
  assert(md3dDevice);

  ReleaseCOM(mRenderTargetView);
  ReleaseCOM(mDepthStencilBuffer);
  ReleaseCOM(mDepthStencilView);

  HRESULT hr;
  hr = mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
  if (FAILED(hr))
  {
    return;
  }

  ID3D11Texture2D* backBuffer;
  hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
  if (FAILED(hr))
  {
    return;
  }

  hr = md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
  if (FAILED(hr))
  {
    return;
  }

  D3D11_TEXTURE2D_DESC depthStencilDesc;

  depthStencilDesc.Width = mClientWidth;
  depthStencilDesc.Height = mClientHeight;
  depthStencilDesc.MipLevels = 1;
  depthStencilDesc.ArraySize = 1;
  depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

  if (mEnable4xMsaa)
  {
    depthStencilDesc.SampleDesc.Count = 4;
    depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
  }
  else
  {
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
  }

  depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
  depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depthStencilDesc.CPUAccessFlags = 0;
  depthStencilDesc.MiscFlags = 0;

  md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
  md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

  //bind render target view and depth/stencil view to the pipeline
  md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

  mScreenViewport.TopLeftX = 0;
  mScreenViewport.TopLeftY = 0;
  mScreenViewport.Width = static_cast<float>(mClientWidth);
  mScreenViewport.Height = static_cast<float>(mClientHeight);
  mScreenViewport.MinDepth = 0;
  mScreenViewport.MaxDepth = 1;

  md3dImmediateContext->RSSetViewports(1, &mScreenViewport);
}

LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_ACTIVATE:
    if (LOWORD(wParam) == WA_INACTIVE)
    {
      mAppPaused = true;
      mTimer.Stop();
    }
    else
    {
      mAppPaused = false;
      mTimer.Start();
    }
    return 0;
  case WM_SIZE:
    mClientHeight = LOWORD(lParam);
    mClientWidth = HIWORD(lParam);
    if (md3dDevice)
    {
      if (wParam == SIZE_MINIMIZED)
      {
        mAppPaused = true;
        mMinimized = true;
        mMaximized = false;
      }
      else if (wParam == SIZE_MAXIMIZED)
      {
        mAppPaused = false;
        mMinimized = false;
        mMaximized = true;
      }
      else if (wParam == SIZE_RESTORED)
      {
        if (mMinimized)
        {
          mAppPaused = false;
          mMinimized = false;
          OnReSize();
        }
        else if (mMaximized)
        {
          mAppPaused = false;
          mMaximized = false;
          OnReSize();
        }
        else if (mResizing)
        {
          
        }
        else
        {
          OnReSize();
        }
      }
    }
    return 0;

  case WM_ENTERSIZEMOVE:
    mAppPaused = true;
    mResizing = true;
    return 0;

  case WM_EXITSIZEMOVE:
    mAppPaused = false;
    mResizing = false;
    mTimer.Start();
    OnReSize();
    return 0;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_MENUCHAR:
    return MAKELRESULT(0, WM_CLOSE);

  case WM_GETMINMAXINFO:
    ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
    ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
    return 0;

  case WM_LBUTTONDOWN:
  case WM_MBUTTONDOWN:
  case WM_RBUTTONDOWN:
    OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    return 0;
  default:
    break;
  }
}

bool D3DApp::InitMainWindow()
{

}

bool D3DApp::InitDirect3D()
{

}

void D3DApp::CalculateFrameStats()
{

}
