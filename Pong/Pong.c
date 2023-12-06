#include <windows.h>
#include "Restart.h"
#include "Menu.h"
#include "Pong.h"


// Définition des variables globales
int paddleY = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
int ballX = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
int ballY = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
int ballSpeedX = BALL_SPEED;
int ballSpeedY = BALL_SPEED;

// Fonction de fenêtre
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        SetTimer(hWnd, 1, 16, NULL);  // Démarre une minuterie pour mettre à jour le jeu
        break;

    case WM_TIMER:
        // Mise à jour du jeu
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        // Rebondissement sur les bords verticaux
        if (ballY <= 0 || ballY + BALL_SIZE >= WINDOW_HEIGHT - 40) {
            ballSpeedY = -ballSpeedY;
        }

        // Rebondissement sur les bords horizontaux
        if (ballX + BALL_SIZE >= WINDOW_WIDTH - 10) {
			ballSpeedX = -ballSpeedX;
		}

        // Rebondissement sur la raquette
        if (ballX <= PADDLE_WIDTH + 20 && ballY + BALL_SIZE >= paddleY && ballY <= paddleY + PADDLE_HEIGHT) {
            ballSpeedX = -ballSpeedX;
        }

        // Gestion de la raquette
        if (GetAsyncKeyState(VK_UP) & 0x8000 && paddleY > 0) {
            paddleY -= PADDLE_SPEED;
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000 && paddleY + PADDLE_HEIGHT < WINDOW_HEIGHT) {
            paddleY += PADDLE_SPEED;
        }

        //Gestion de la defaite
        if (ballX <= 0) {
            RestartGame();
        }

        // Redessiner la fenêtre
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Dessiner la raquette
        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); // Couleur noire (0, 0, 0) en format RGB
        RECT rect = { PADDLE_WIDTH, paddleY, PADDLE_WIDTH + PADDLE_WIDTH, paddleY + PADDLE_HEIGHT };
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);

        // Dessiner la balle
        Ellipse(hdc, ballX, ballY, ballX + BALL_SIZE, ballY + BALL_SIZE);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        KillTimer(hWnd, 1);  // Arrête la minuterie avant de quitter
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Créer et afficher la fenêtre principale
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance,
                        LoadIcon(hInstance, IDI_APPLICATION),
                        LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1),
                        NULL, L"PongClass", LoadIcon(hInstance, IDI_APPLICATION) };

    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(L"PongClass", L"Pong", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, 0,
        WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Boucle de messages principale
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
