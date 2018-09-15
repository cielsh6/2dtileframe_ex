#include <Windows.h>
#include <d3dx9.h>
#include "GameTimer.h"
#include "Sprite.h"


// 윈도우 프로시저 함수 : 윈도우로 부터 받은 이벤트를 처리하는 함수 (내가 처리한다)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World", L"Hello", MB_OK);
		return 0;
	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam)
		{
			DestroyWindow(hWnd);	//윈도우 파괴요청 (종료요청)
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);		//파괴 하겠다는 허락(메시지)이 오면 파괴함
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(
	HINSTANCE hInstance,	// 현재 응용 프로그램의 메인 핸들
	HINSTANCE hPrevInstance,	// 안쓰임
	PSTR pCmdLine,	// 실행하는데 쓰이는 명령줄 또는 인수
	int nCmdShow	// 응용프로그램 표시 방식. 최소화/최대화 해서 보여줄거냐 겨렂
)
{
	int width = 1024;
	int height = 768;
	bool isWindow = true;
	
	// 윈도우 스타일을 만들고
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;	// 윈도우 프로시저 함수 등록
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"2DTileFrameWnd";			// 이 윈도우 스타일의 이름

	// 만든 윈도우 스타일 등록
	if (FALSE == RegisterClass(&wc))
	{
		return 0;
	}

	//fullscreen 모드일 때는, 윈도우에 부가적인 요소를 제거한다
	DWORD style;
	if (isWindow)
	{
		style = WS_OVERLAPPEDWINDOW;
	}
	else
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}


	// 창 핸들(=아이디)를 먼저 발급을 받자
	HWND hWnd = CreateWindow(
		L"2DTileFrameWnd",	// 사용할 윈도우 스타일 이름. OS에 등록 되어있음.
		L"2D Tile Frmae",
		style,	//윈도우 스타일. 캡션이 있을 것인지 메뉴가 있을 것인지 등등
		CW_USEDEFAULT, CW_USEDEFAULT,	// 시작위치 :  x, y
		width, height,		// 해상도. 너비/높이
		0,		// 부모 창의 핸들. 사용 안함
		0,		// 메뉴 핸들. 사용 안함
		hInstance,	// OS와 윈도우 연결. OS에서 윈도우를 관리할 수 있다.
		0
	);
	if (NULL == hWnd)
	{
		return 0;
	}

	// hWnd : 하나의 윈도우를 가리키는 핸들(아이디)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//윈도우 모드일때 실제 게임 영역 확보
	if (true == isWindow)
	{
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		MoveWindow(hWnd, 0, 0, width + (width - clientRect.right), height + (height - clientRect.bottom), TRUE);
	}

	//directX
	//누군가(directX)한테 하드웨어에 집적 접근할 수 있는 무언가(dxDevice)를 생성해 달라고 요청
	LPDIRECT3D9 direct3d;	//그래픽을 담당하는 directX
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	//윈도우가 안주면 끝낸다
	if (NULL == direct3d)
	{
		return 0;
	}


	D3DFORMAT format;
	if (isWindow)
	{
		format = D3DFMT_UNKNOWN;	//윈도우 설정에 맡김
	}
	else
	{
		format = D3DFMT_X8R8G8B8;	// 풀스크린이면 전용 색상 포맷을 사용하겠다
	}

	//device를 생성하기 전에, device를 통해서 화면에 어떻게 보여질지를 결정
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = format;
	d3dpp.BackBufferCount = 1;		//더블 버퍼링 갯수
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//교체시킬때 사용할 이펙트
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = isWindow;		//윈도우 창모드 사용 여부
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		//교체할때 즉시 보여줘라

	//device 생성 요청
	LPDIRECT3DDEVICE9 dxDevice;
	HRESULT hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dxDevice);
	//생성에 실패하면 프로그램 종료
	if (FAILED(hr))
	{
		return 0;
	}

	//sprite COM 인터페이스 생성
	ID3DXSprite* spriteDX;
	hr = D3DXCreateSprite(dxDevice, &spriteDX);
	if (FAILED(hr))
	{
		return 0;
	}

	//이미지 로드
	IDirect3DTexture9* textureDX;
	RECT textureRect;
	D3DCOLOR textureColor;

	//로드 할 파일 명
	LPCWSTR fileName = L"../Resources/Image/character_2.png";

	// 스프라이트를 생성
	Sprite* testSprite = new Sprite();
	testSprite->Init(fileName, dxDevice, spriteDX);

	
	float fps = 60.0f;
	float frameInterval = 1.0f / fps;		// "f" means = 실수.
	float frameTime = 0.0f;

	GameTimer gameTimer;
	gameTimer.Init();

	// 이 윈도우가 꺼지지 않고 유지되도록 처리
	/*
	MSG msg;
	while (-1 != GetMessage(&msg, 0, 0, 0))	// 발생된 메시지가 있으면
	{
		TranslateMessage(&msg);		// 키보드 변환 수행
		DispatchMessage(&msg);	// 메시지 창에 배분을 요청한다.
	}
	*/

	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (TRUE == PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);		// 키보드 변환 수행
			DispatchMessage(&msg);	// 메시지 창에 배분을 요청한다.
		}
		else
		{
			gameTimer.Update();
			float deltaTime = gameTimer.GetDeltaTime();

			testSprite->Update(deltaTime);
			
			// deltaTime : 이전 윈도우 업데이트부터 지금까지 흐른 시간
			frameTime += deltaTime;
			if (frameInterval <= frameTime)
			{
				frameTime = 0.0f;	//다시 0으로 만든다
				
				//directX를 사용했을 때 가장 기본적인 모습
				dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(180, 255, 255), 0.0f, 0);	//화면의 색을 매 프레임마다 채운다
				{
					dxDevice->BeginScene();
					{
						

						//scene 작업 : 게임 화면과 관련된 모든 작업 공간
						spriteDX->Begin(D3DXSPRITE_ALPHABLEND);
						{
							//스프라이트 랜더
							testSprite->Render();

						}
						spriteDX->End();
					}

					// DeviceLost 처리
					// Device 상태 확인 (매 업데이트 마다)
					hr = dxDevice->TestCooperativeLevel();
					// Device 상태에 문제가 있으면 생성된 모든 리소스 복구 (=처음부터 로드)
					if (FAILED(hr))
					{
						// 세 종류의 문제에 따라 다르게 처리
						if (D3DERR_DEVICELOST == hr)
						{
							//현재 아무것도 할 수 없는 상태
							Sleep(100);		//쉰다
						}
						else if(D3DERR_DEVICENOTRESET == hr)
						{
							//여전히 망가진 상태이지만, 이제는 복구가 가능한 상태
							//지금부터 복구를 진행하시오
							//기존에 만들어진 것들을 모두 리셋 -> 새로 생성
							testSprite->Release();
							/*
							if (textureDX)
							{
								textureDX->Release();
								textureDX = NULL;
							}
							*/

							direct3d = Direct3DCreate9(D3D_SDK_VERSION);
							if (NULL != direct3d)
							{
								//디바이스 생성
								HRESULT hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dxDevice);
								
								if (SUCCEEDED(hr))
								{
									//스프라이트 인터페이스 생성
									hr = D3DXCreateSprite(dxDevice, &spriteDX);
								}
								
								if (SUCCEEDED(hr))
								{
									testSprite->Reset();

									/*
									//텍스쳐 생성
									hr = D3DXCreateTextureFromFileEx(dxDevice,
										fileName,
										texInfo.Width, texInfo.Height,
										1,
										0,
										D3DFMT_UNKNOWN,
										D3DPOOL_DEFAULT,
										D3DX_DEFAULT,
										D3DX_DEFAULT,
										D3DCOLOR_ARGB(255, 255, 255, 255),
										&texInfo,
										NULL,
										&textureDX);
									*/
								}
							}
						}

					}

					dxDevice->EndScene();
				}
				dxDevice->Present(NULL, NULL, NULL, NULL);	//채운 색을 모니터를 통해 보여준다

			}
		}
	}

	//COM인터페이스를 지우는 가장 일반적인 방법	
	//텍스쳐 해제는 스프라이트가 파괴될 때
	if (NULL != testSprite)
	{
		delete testSprite;
	}
	/*
	if (textureDX)
	{
		textureDX->Release();
		textureDX = NULL;
	}
	*/
		
	if (dxDevice)
	{
		dxDevice->Release();
		dxDevice = NULL;
	}

	if (direct3d)
	{
		direct3d->Release();
		direct3d = NULL;
	}

	return 0;
}