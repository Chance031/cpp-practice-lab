#include <iostream>
#include <windows.h> // POINT, GetCursorPos(), GetAsyncKeyState(), Sleep()를 사용하기 위한 헤더
#include <conio.h>   // _kbhit(), _getch()를 사용하기 위한 헤더

// POINT는 x, y 좌표를 저장하는 Windows 구조체이다.
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
        // 키보드 입력이 있는지 먼저 확인한다.
        if (_kbhit())
        {
            int keyCode = _getch();

            // q 또는 Q를 누르면 프로그램을 종료한다.
            if (keyCode == 'q' || keyCode == 'Q')
                break;
        }

        POINT mousePosition = GetMousePosition();
        bool isLeftPressed = IsLeftMousePressed();
        bool isRightPressed = IsRightMousePressed();

        PrintMouseState(mousePosition, isLeftPressed, isRightPressed);

        // Sleep()은 반복문이 너무 빠르게 도는 것을 막기 위해 잠시 대기한다.
        // 숫자가 커질수록 마우스 상태가 갱신되는 속도는 느려진다.
        Sleep(50);
    }

    std::cout << "\n프로그램을 종료합니다.\n";

    return 0;
}

// 현재 마우스 위치를 POINT 구조체로 반환한다.
POINT GetMousePosition()
{
    POINT position = { 0, 0 };

    // GetCursorPos()는 현재 마우스 좌표를 읽어서 position에 직접 저장한다.
    // &position은 position 변수의 주소를 넘겨준다는 뜻이다.
    GetCursorPos(&position);

    return position;
}

// 좌클릭이 눌려 있는지 확인한다.
bool IsLeftMousePressed()
{
    // VK_LBUTTON은 왼쪽 마우스 버튼을 뜻한다.
    // GetAsyncKeyState()는 지정한 키나 버튼의 현재 상태 값을 반환한다.
    // 0x8000은 반환값에서 "현재 눌려 있음" 상태를 확인하기 위한 비트 마스크이다.
    return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}

// 우클릭이 눌려 있는지 확인한다.
bool IsRightMousePressed()
{
    // VK_RBUTTON은 오른쪽 마우스 버튼을 뜻한다.
    // GetAsyncKeyState()는 지정한 키나 버튼의 현재 상태 값을 반환한다.
    // 0x8000은 반환값에서 "현재 눌려 있음" 상태를 확인하기 위한 비트 마스크이다.
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
