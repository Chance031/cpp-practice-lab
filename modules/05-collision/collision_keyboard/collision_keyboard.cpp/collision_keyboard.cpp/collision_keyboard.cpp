#include <iostream>
#include <cstdlib>  // system()을 사용하기 위한 헤더
#include <conio.h>  // _getch()를 사용하기 위한 헤더
#include <windows.h> // Sleep()를 사용하기 위한 헤더

struct Rect
{
    int x;
    int y;
    int width;
    int height;
};

const int kMapWidth = 20;
const int kMapHeight = 12;

bool IsColliding(const Rect& a, const Rect& b);
bool IsPointInsideRect(int x, int y, const Rect& rect);
void PrintCollisionMap(const Rect& a, const Rect& b);
void MoveRect(Rect& rect, int keyCode);
void KeepRectInsideMap(Rect& rect);

int main()
{
    Rect playerRect = { 1, 1, 4, 3 };
    Rect wallRect = { 10, 4, 5, 4 };

    while (true)
    {
        system("cls");

        std::cout << "=== Collision Keyboard Example ===\n";
        std::cout << "Move Rect A with arrow keys.\n";
        std::cout << "Press q or Q to quit.\n\n";

        PrintCollisionMap(playerRect, wallRect);
        std::cout << '\n';

        if (IsColliding(playerRect, wallRect))
            std::cout << "Collision Detected.\n";
        else
            std::cout << "No Collision.\n";

        int keyCode = _getch();

        if (keyCode == 'q' || keyCode == 'Q')
            break;

        if (keyCode == 0 || keyCode == 224)
        {
            keyCode = _getch();
            MoveRect(playerRect, keyCode);
            KeepRectInsideMap(playerRect);
        }

        Sleep(30);
    }

    return 0;
}

// 두 사각형이 서로 겹치는지 확인한다.
bool IsColliding(const Rect& a, const Rect& b)
{
    return a.x < b.x + b.width
        && a.x + a.width > b.x
        && a.y < b.y + b.height
        && a.y + a.height > b.y;
}

// 한 점이 사각형 안에 포함되는지 확인한다.
bool IsPointInsideRect(int x, int y, const Rect& rect)
{
    return x >= rect.x
        && x < rect.x + rect.width
        && y >= rect.y
        && y < rect.y + rect.height;
}

// 콘솔 격자에 두 사각형의 위치를 시각화한다.
void PrintCollisionMap(const Rect& a, const Rect& b)
{
    for (int y = 0; y < kMapHeight; ++y)
    {
        for (int x = 0; x < kMapWidth; ++x)
        {
            bool isInsideA = IsPointInsideRect(x, y, a);
            bool isInsideB = IsPointInsideRect(x, y, b);

            if (isInsideA && isInsideB)
                std::cout << 'X';
            else if (isInsideA)
                std::cout << 'A';
            else if (isInsideB)
                std::cout << 'B';
            else
                std::cout << '.';
        }

        std::cout << '\n';
    }
}

// 방향키 입력에 따라 사각형을 이동시킨다.
void MoveRect(Rect& rect, int keyCode)
{
    switch (keyCode)
    {
    case 72:
        rect.y -= 1;
        break;
    case 80:
        rect.y += 1;
        break;
    case 75:
        rect.x -= 1;
        break;
    case 77:
        rect.x += 1;
        break;
    default:
        break;
    }
}

// 사각형이 맵 바깥으로 나가지 않도록 보정한다.
void KeepRectInsideMap(Rect& rect)
{
    if (rect.x < 0)
        rect.x = 0;
    if (rect.y < 0)
        rect.y = 0;
    if (rect.x + rect.width > kMapWidth)
        rect.x = kMapWidth - rect.width;
    if (rect.y + rect.height > kMapHeight)
        rect.y = kMapHeight - rect.height;
}
