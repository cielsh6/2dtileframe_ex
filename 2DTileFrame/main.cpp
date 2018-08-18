#include <Windows.h>
#include <d3d9.h>
#include "GameTimer.h"


// ������ ���ν��� �Լ� : ������� ���� ���� �̺�Ʈ�� ó���ϴ� �Լ� (���� ó���Ѵ�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, "Hello World", "Hello", MB_OK);
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
	wc.lpszClassName = "2DTileFrameWnd";			// �� ������ ��Ÿ���� �̸�

	// ���� ������ ��Ÿ�� ���
	if (FALSE == RegisterClass(&wc))
	{
		return 0;
	}

	// â �ڵ�(���̵�)�� ���� �߱��� ����
	HWND hWnd = CreateWindow(
		"2DTileFrameWnd",	// ����� ������ ��Ÿ�� �̸�. OS�� ��� �Ǿ�����.
		"2D Tile Frmae",
		WS_OVERLAPPEDWINDOW,	// ������ ��Ÿ��
		CW_USEDEFAULT, CW_USEDEFAULT,	// ������ġ :  x, y
		1024, 768,		// �ػ�. �ʺ�/����
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

	//directX
	//������(directX)���� �ϵ��� ���� ������ �� �ִ� ����(dxDevice)�� ������ �޶�� ��û
	LPDIRECT3D9 direct3d;	//�׷����� ����ϴ� directX
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	//�����찡 ���ָ� ������
	if (NULL == direct3d)
	{
		return 0;
	}

	//device�� �����ϱ� ����, device�� ���ؼ� ȭ�鿡 ��� ���������� ����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = 12380;
	d3dpp.BackBufferHeight = 768;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;		//���� ���۸� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = true;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


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

			// deltaTime : ���� ������ ������Ʈ���� ���ݱ��� �帥 �ð�
			frameTime += deltaTime;
			if (frameInterval <= frameTime)
			{
				frameTime = 0.0f;	//�ٽ� 0���� �����
				
				//directX�� ������� �� ���� �⺻���� ���
				dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 128, 0), 0, 0.0f, 0);	//ȭ���� ���� �� �����Ӹ��� ä���
				dxDevice->Present(NULL, NULL, NULL, NULL);	//ä�� ���� ����͸� ���� �����ش�

			}
		}
	}

	return 0;
}