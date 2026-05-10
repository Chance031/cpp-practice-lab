#include <iostream>
#include <cstdlib>   // system()을 사용하기 위한 헤더
#include <conio.h>   // _getch()를 사용하기 위한 헤더
#include <windows.h> // Sleep()를 사용하기 위한 헤더

// 사각형의 위치와 크기를 저장하는 구조체이다.
struct Rect
{
    int x;
    int y;
    int width;
    int height;
};

// 콘솔에 출력할 격자의 크기이다.
const int kMapWidth = 20;
const int kMapHeight = 12;

bool IsColliding(const Rect& a, const Rect& b);
bool IsPointInsideRect(int x, int y, const Rect& rect);
void PrintCollisionMap(const Rect& a, const Rect& b);
void MoveRect(Rect& rect, int keyCode);
void KeepRectInsideMap(Rect& rect);

int main()
{
    // 플레이어 역할을 하는 사각형과 벽 역할을 하는 사각형을 만든다.
    Rect playerRect = { 1, 1, 4, 3 };
    Rect wallRect = { 10, 4, 5, 4 };

    while (true)
    {
        // 이전 화면을 지우고 현재 상태를 다시 출력한다.
        system("cls");

        std::cout << "=== Collision Keyboard Example ===\n";
        std::cout << "Move Rect A with arrow keys.\n";
        std::cout << "Press q or Q to quit.\n\n";

        // 콘솔 격자에 두 사각형을 시각적으로 출력한다.
        PrintCollisionMap(playerRect, wallRect);
        std::cout << '\n';

        // 충돌 판정 결과를 출력한다.
        if (IsColliding(playerRect, wallRect))
            std::cout << "Collision Detected.\n";
        else
            std::cout << "No Collision.\n";

        // 방향키 또는 종료 키를 입력받는다.
        int keyCode = _getch();

        // q 또는 Q를 누르면 반복문을 종료한다.
        if (keyCode == 'q' || keyCode == 'Q')
            break;

        // 특수 키는 0 또는 224가 먼저 들어오므로, 한 번 더 읽어서 실제 키 값을 확인한다.
        if (keyCode == 0 || keyCode == 224)
        {
            keyCode = _getch();
            MoveRect(playerRect, keyCode);
            KeepRectInsideMap(playerRect);
        }

        // 반복문이 너무 빠르게 실행되지 않도록 잠시 대기한다.
        Sleep(30);
    }

    return 0;
}

// 두 사각형이 서로 겹치는지 확인한다.
bool IsColliding(const Rect& a, const Rect& b)
{
    // AABB 충돌 판정:
    // 한 사각형이 다른 사각형의 왼쪽, 오른쪽, 위, 아래로 완전히 벗어나지 않았으면 충돌로 본다.
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

            // X는 겹친 영역, A는 Rect A만 있는 영역,
            // B는 Rect B만 있는 영역, .은 빈 공간을 뜻한다.
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
