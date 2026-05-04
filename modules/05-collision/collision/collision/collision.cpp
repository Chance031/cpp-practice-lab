#include <iostream>

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
void PrintRect(const char* name, const Rect& rect);
bool IsPointInsideRect(int x, int y, const Rect& rect);
void PrintCollisionMap(const Rect& a, const Rect& b);

int main()
{
    Rect rectA = { 2, 2, 6, 4 };
    Rect rectB = { 5, 3, 7, 5 };

    PrintRect("Rect A", rectA);
    PrintRect("Rect B", rectB);
    std::cout << '\n';

    PrintCollisionMap(rectA, rectB);
    std::cout << '\n';

    if (IsColliding(rectA, rectB))
        std::cout << "Collision Detected.\n";
    else
        std::cout << "No Collision.\n";

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
