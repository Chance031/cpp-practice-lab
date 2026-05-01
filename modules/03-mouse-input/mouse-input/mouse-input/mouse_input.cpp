#include <iostream>
#include <windows.h>
#include <conio.h>

POINT GetMousePosition();
bool IsLeftMousePressed();
bool IsRightMousePressed();
void PrintMouseState(const POINT& position, bool isLeftPressed, bool isRightPressed);

int main()
{
    std::cout << "마우스를 움직이거나 클릭해보십시오.\n";
    std::cout << "q 또는 Q를 누르면 종료합니다.\n\n";

    while (true)
    {
        if (_kbhit())
        {
            int keyCode = _getch();

            if (keyCode == 'q' || keyCode == 'Q')
                break;
        }

        POINT mousePosition = GetMousePosition();
        bool isLeftPressed = IsLeftMousePressed();
        bool isRightPressed = IsRightMousePressed();

        PrintMouseState(mousePosition, isLeftPressed, isRightPressed);
        Sleep(50);
    }

    std::cout << "\n프로그램을 종료합니다.\n";

    return 0;
}

// 현재 마우스 위치를 반환한다.
POINT GetMousePosition()
{
    POINT position = { 0, 0 };
    GetCursorPos(&position);
    return position;
}

// 좌클릭이 눌려 있는지 확인한다.
bool IsLeftMousePressed()
{
    return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}

// 우클릭이 눌려 있는지 확인한다.
bool IsRightMousePressed()
{
    return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
}

// 현재 마우스 상태를 콘솔에 출력한다.
void PrintMouseState(const POINT& position, bool isLeftPressed, bool isRightPressed)
{
    std::cout << "\rMouse Position: (" << position.x << ", " << position.y << ")"
              << " | Left Button: " << (isLeftPressed ? "Pressed" : "Released")
              << " | Right Button: " << (isRightPressed ? "Pressed" : "Released")
              << "    ";
}
