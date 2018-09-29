#include <windows.h>
#include <iostream>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

ostream& operator<<(ostream& os, FXMVECTOR v)
{
  XMFLOAT3 dest;

  XMStoreFloat3(&dest, v);

  os << "(" << dest.x << "," << dest.y << "," << dest.z << ")";
  return os;
}


int main()
{
  int a = -1;
  unsigned int b = a >> 1;
  std::cout << b << std::endl;
  std::cout.setf(std::ios_base::boolalpha);

  std::cout<< "XMVerifyCPUSuppot : " << XMVerifyCPUSupport() <<endl;
  if (!XMVerifyCPUSupport())
  {
    return 0;
  }

  {
    XMVECTOR p = XMVectorZero();
    XMVECTOR q = XMVectorSplatOne();
    XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 4.0f);
    XMVECTOR v = XMVectorReplicate(3.0f);
    XMVECTOR w = XMVectorSplatW(u);

    std::cout << " p = " << p << std::endl;
    std::cout << " q = " << q << std::endl;
    std::cout << " u = " << u << std::endl;
    std::cout << " v = " << v << std::endl;
    std::cout << " w = " << w << std::endl;
  }

  {
    XMVECTOR n = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 1.0f);
    XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 0.0f);
    XMVECTOR w = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);


    XMVECTOR a = u + v;
    XMVECTOR b = u - v;
    XMVECTOR c = 10.0f*u;
    XMVECTOR len2 = XMVector2Length(u);
    XMVECTOR len3 = XMVector3Length(u);
    XMVECTOR len4 = XMVector4Length(u);

    XMVECTOR nor2 = XMVector2Normalize(u);
    XMVECTOR nor3 = XMVector3Normalize(u);
    XMVECTOR nor4 = XMVector4Normalize(u);

    XMVECTOR dot = XMVector2Dot(u, v);

    std::cout << " n = " << n << std::endl;
    std::cout << " u = " << u << std::endl;
    std::cout << " v = " << v << std::endl;
    std::cout << " w = " << w << std::endl;
    std::cout << " a = " << a << std::endl;
    std::cout << " b = " << b << std::endl;
    std::cout << " c = " << c << std::endl;
    std::cout << " len2 = " << len2 << std::endl;
    std::cout << " len3 = " << len3 << std::endl;
    std::cout << " len4 = " << len4 << std::endl;
    std::cout << " nor2 = " << nor2 << std::endl;
    std::cout << " nor3 = " << nor3 << std::endl;
    std::cout << " nor4 = " << nor4 << std::endl;


  }



  getchar();
}


