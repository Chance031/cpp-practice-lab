#include <iostream>

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
void PrintRect(const char* name, const Rect& rect);
bool IsPointInsideRect(int x, int y, const Rect& rect);
void PrintCollisionMap(const Rect& a, const Rect& b);

int main()
{
    // 충돌 여부를 확인할 두 사각형을 만든다.
    Rect rectA = { 2, 2, 6, 4 };
    Rect rectB = { 5, 3, 7, 5 };

    // 각 사각형의 위치와 크기를 먼저 출력한다.
    PrintRect("Rect A", rectA);
    PrintRect("Rect B", rectB);
    std::cout << '\n';

    // 콘솔 격자에 두 사각형을 시각적으로 출력한다.
    PrintCollisionMap(rectA, rectB);
    std::cout << '\n';

    // 충돌 판정 결과를 출력한다.
    if (IsColliding(rectA, rectB))
        std::cout << "Collision Detected.\n";
    else
        std::cout << "No Collision.\n";

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

// 사각형의 위치와 크기를 출력한다.
void PrintRect(const char* name, const Rect& rect)
{
    std::cout << name
              << " : x = " << rect.x
              << ", y = " << rect.y
              << ", width = " << rect.width
              << ", height = " << rect.height << '\n';
}

// 한 점이 사각형 안에 포함되는지 확인한다.
bool IsPointInsideRect(int x, int y, const Rect& rect)
{
    return x >= rect.x
        && x < rect.x + rect.width
        && y >= rect.y
        && y < rect.y + rect.height;
}

// 두 사각형의 위치를 콘솔 격자로 시각화한다.
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
