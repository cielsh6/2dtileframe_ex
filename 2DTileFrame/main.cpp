#include <Windows.h>
#include <d3dx9.h>
#include "GameTimer.h"
#include "Sprite.h"


// ������ ���ν��� �Լ� : ������� ���� ���� �̺�Ʈ�� ó���ϴ� �Լ� (���� ó���Ѵ�)
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
			DestroyWindow(hWnd);	//������ �ı���û (�����û)
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);		//�ı� �ϰڴٴ� ���(�޽���)�� ���� �ı���
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(
	HINSTANCE hInstance,	// ���� ���� ���α׷��� ���� �ڵ�
	HINSTANCE hPrevInstance,	// �Ⱦ���
	PSTR pCmdLine,	// �����ϴµ� ���̴� ����� �Ǵ� �μ�
	int nCmdShow	// �������α׷� ǥ�� ���. �ּ�ȭ/�ִ�ȭ �ؼ� �����ٰų� �܎�
)
{
	int width = 1024;
	int height = 768;
	bool isWindow = true;
	
	// ������ ��Ÿ���� �����
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;	// ������ ���ν��� �Լ� ���
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"2DTileFrameWnd";			// �� ������ ��Ÿ���� �̸�

	// ���� ������ ��Ÿ�� ���
	if (FALSE == RegisterClass(&wc))
	{
		return 0;
	}

	//fullscreen ����� ����, �����쿡 �ΰ����� ��Ҹ� �����Ѵ�
	DWORD style;
	if (isWindow)
	{
		style = WS_OVERLAPPEDWINDOW;
	}
	else
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}


	// â �ڵ�(=���̵�)�� ���� �߱��� ����
	HWND hWnd = CreateWindow(
		L"2DTileFrameWnd",	// ����� ������ ��Ÿ�� �̸�. OS�� ��� �Ǿ�����.
		L"2D Tile Frmae",
		style,	//������ ��Ÿ��. ĸ���� ���� ������ �޴��� ���� ������ ���
		CW_USEDEFAULT, CW_USEDEFAULT,	// ������ġ :  x, y
		width, height,		// �ػ�. �ʺ�/����
		0,		// �θ� â�� �ڵ�. ��� ����
		0,		// �޴� �ڵ�. ��� ����
		hInstance,	// OS�� ������ ����. OS���� �����츦 ������ �� �ִ�.
		0
	);
	if (NULL == hWnd)
	{
		return 0;
	}

	// hWnd : �ϳ��� �����츦 ����Ű�� �ڵ�(���̵�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//������ ����϶� ���� ���� ���� Ȯ��
	if (true == isWindow)
	{
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		MoveWindow(hWnd, 0, 0, width + (width - clientRect.right), height + (height - clientRect.bottom), TRUE);
	}

	//directX
	//������(directX)���� �ϵ��� ���� ������ �� �ִ� ����(dxDevice)�� ������ �޶�� ��û
	LPDIRECT3D9 direct3d;	//�׷����� ����ϴ� directX
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	//�����찡 ���ָ� ������
	if (NULL == direct3d)
	{
		return 0;
	}


	D3DFORMAT format;
	if (isWindow)
	{
		format = D3DFMT_UNKNOWN;	//������ ������ �ñ�
	}
	else
	{
		format = D3DFMT_X8R8G8B8;	// Ǯ��ũ���̸� ���� ���� ������ ����ϰڴ�
	}

	//device�� �����ϱ� ����, device�� ���ؼ� ȭ�鿡 ��� ���������� ����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = format;
	d3dpp.BackBufferCount = 1;		//���� ���۸� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//��ü��ų�� ����� ����Ʈ
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = isWindow;		//������ â��� ��� ����
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		//��ü�Ҷ� ��� �������

	//device ���� ��û
	LPDIRECT3DDEVICE9 dxDevice;
	HRESULT hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dxDevice);
	//������ �����ϸ� ���α׷� ����
	if (FAILED(hr))
	{
		return 0;
	}

	//sprite COM �������̽� ����
	ID3DXSprite* spriteDX;
	hr = D3DXCreateSprite(dxDevice, &spriteDX);
	if (FAILED(hr))
	{
		return 0;
	}

	//�̹��� �ε�
	IDirect3DTexture9* textureDX;
	RECT textureRect;
	D3DCOLOR textureColor;

	//�ε� �� ���� ��
	LPCWSTR fileName = L"../Resources/Image/character_2.png";

	// ��������Ʈ�� ����
	Sprite* testSprite = new Sprite();
	testSprite->Init(fileName, dxDevice, spriteDX);

	
	float fps = 60.0f;
	float frameInterval = 1.0f / fps;		// "f" means = �Ǽ�.
	float frameTime = 0.0f;

	GameTimer gameTimer;
	gameTimer.Init();

	// �� �����찡 ������ �ʰ� �����ǵ��� ó��
	/*
	MSG msg;
	while (-1 != GetMessage(&msg, 0, 0, 0))	// �߻��� �޽����� ������
	{
		TranslateMessage(&msg);		// Ű���� ��ȯ ����
		DispatchMessage(&msg);	// �޽��� â�� ����� ��û�Ѵ�.
	}
	*/

	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (TRUE == PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);		// Ű���� ��ȯ ����
			DispatchMessage(&msg);	// �޽��� â�� ����� ��û�Ѵ�.
		}
		else
		{
			gameTimer.Update();
			float deltaTime = gameTimer.GetDeltaTime();

			testSprite->Update(deltaTime);
			
			// deltaTime : ���� ������ ������Ʈ���� ���ݱ��� �帥 �ð�
			frameTime += deltaTime;
			if (frameInterval <= frameTime)
			{
				frameTime = 0.0f;	//�ٽ� 0���� �����
				
				//directX�� ������� �� ���� �⺻���� ���
				dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(180, 255, 255), 0.0f, 0);	//ȭ���� ���� �� �����Ӹ��� ä���
				{
					dxDevice->BeginScene();
					{
						

						//scene �۾� : ���� ȭ��� ���õ� ��� �۾� ����
						spriteDX->Begin(D3DXSPRITE_ALPHABLEND);
						{
							//��������Ʈ ����
							testSprite->Render();

						}
						spriteDX->End();
					}

					// DeviceLost ó��
					// Device ���� Ȯ�� (�� ������Ʈ ����)
					hr = dxDevice->TestCooperativeLevel();
					// Device ���¿� ������ ������ ������ ��� ���ҽ� ���� (=ó������ �ε�)
					if (FAILED(hr))
					{
						// �� ������ ������ ���� �ٸ��� ó��
						if (D3DERR_DEVICELOST == hr)
						{
							//���� �ƹ��͵� �� �� ���� ����
							Sleep(100);		//����
						}
						else if(D3DERR_DEVICENOTRESET == hr)
						{
							//������ ������ ����������, ������ ������ ������ ����
							//���ݺ��� ������ �����Ͻÿ�
							//������ ������� �͵��� ��� ���� -> ���� ����
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
								//����̽� ����
								HRESULT hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dxDevice);
								
								if (SUCCEEDED(hr))
								{
									//��������Ʈ �������̽� ����
									hr = D3DXCreateSprite(dxDevice, &spriteDX);
								}
								
								if (SUCCEEDED(hr))
								{
									testSprite->Reset();

									/*
									//�ؽ��� ����
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
				dxDevice->Present(NULL, NULL, NULL, NULL);	//ä�� ���� ����͸� ���� �����ش�

			}
		}
	}

	//COM�������̽��� ����� ���� �Ϲ����� ���	
	//�ؽ��� ������ ��������Ʈ�� �ı��� ��
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