#include <iostream>
#include <vector>
#include <string>
#include <windows.h> // POINT, GetCursorPos(), GetAsyncKeyState(), GetConsoleWindow()를 사용하기 위한 헤더

// 사각형의 위치와 크기를 저장하는 구조체이다.
struct Rect
{
    int x;
    int y;
    int width;
    int height;
};

// 콘솔에 출력할 격자의 크기이다.
const int kMapWidth = 40;
const int kMapHeight = 20;
const int kScreenWidth = 80;
const int kScreenHeight = 32;
const int kMapOffsetX = 2;
const int kMapOffsetY = 5;

POINT GetMousePosition();
bool IsLeftMousePressed();
bool GetMouseCellPosition(POINT& screenCellPosition, POINT& mapCellPosition);
bool IsPointInsideRect(const POINT& point, const Rect& rect);
void DrawText(std::vector<CHAR_INFO>& buffer, int x, int y, const std::string& text);
void DrawCollisionMap(std::vector<CHAR_INFO>& buffer, const Rect& rect, const POINT& mapCellPosition, bool isInsideMapArea);
void DrawMouseState(std::vector<CHAR_INFO>& buffer, const POINT& mousePosition, const POINT& screenCellPosition, const POINT& mapCellPosition, bool isLeftPressed, bool isInsideMapArea, bool isInsideRect);
void RenderFrame(HANDLE bufferHandle, const Rect& rect, const POINT& mousePosition, const POINT& screenCellPosition, const POINT& mapCellPosition, bool isLeftPressed, bool isInsideMapArea, bool isInsideRect);
void ConfigureBuffer(HANDLE bufferHandle);

int main()
{
    // 마우스 충돌을 확인할 사각형을 콘솔 맵 기준으로 만든다.
    Rect rect = { 12, 5, 12, 6 };

    HANDLE originalBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE buffers[2] =
    {
        CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr),
        CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr)
    };

    if (buffers[0] == INVALID_HANDLE_VALUE || buffers[1] == INVALID_HANDLE_VALUE)
        return 1;

    ConfigureBuffer(buffers[0]);
    ConfigureBuffer(buffers[1]);

    int activeIndex = 0;

    while (true)
    {
        POINT mousePosition = GetMousePosition();
        POINT screenCellPosition = { -1, -1 };
        POINT mapCellPosition = { -1, -1 };
        bool isInsideMapArea = GetMouseCellPosition(screenCellPosition, mapCellPosition);
        bool isLeftPressed = IsLeftMousePressed();
        bool isInsideRect = isInsideMapArea && IsPointInsideRect(mapCellPosition, rect);

        int backIndex = 1 - activeIndex;
        RenderFrame(buffers[backIndex], rect, mousePosition, screenCellPosition, mapCellPosition, isLeftPressed, isInsideMapArea, isInsideRect);
        SetConsoleActiveScreenBuffer(buffers[backIndex]);
        activeIndex = backIndex;

        if (isInsideRect && isLeftPressed)
            break;

        // 반복문이 너무 빠르게 도는 것을 막기 위해 잠시 대기한다.
        // 숫자가 커질수록 화면 상태가 갱신되는 속도는 느려진다.
        Sleep(50);
    }

    SetConsoleActiveScreenBuffer(originalBuffer);
    CloseHandle(buffers[0]);
    CloseHandle(buffers[1]);

    std::cout << "프로그램을 종료합니다.\n";

    return 0;
}

// 현재 마우스 위치를 화면 픽셀 좌표로 반환한다.
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

// 마우스의 화면 좌표를 콘솔 셀 좌표와 맵 내부 좌표로 변환한다.
bool GetMouseCellPosition(POINT& screenCellPosition, POINT& mapCellPosition)
{
    HWND consoleWindow = GetConsoleWindow();

    if (consoleWindow == nullptr)
        return false;

    RECT clientRect = { 0, 0, 0, 0 };
    if (!GetClientRect(consoleWindow, &clientRect))
        return false;

    POINT clientOrigin = { clientRect.left, clientRect.top };
    ClientToScreen(consoleWindow, &clientOrigin);

    POINT mousePosition = GetMousePosition();
    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;

    if (mousePosition.x < clientOrigin.x || mousePosition.x >= clientOrigin.x + clientWidth
        || mousePosition.y < clientOrigin.y || mousePosition.y >= clientOrigin.y + clientHeight)
    {
        return false;
    }

    int relativeX = mousePosition.x - clientOrigin.x;
    int relativeY = mousePosition.y - clientOrigin.y;

    screenCellPosition.x = relativeX * kScreenWidth / clientWidth;
    screenCellPosition.y = relativeY * kScreenHeight / clientHeight;

    mapCellPosition.x = screenCellPosition.x - kMapOffsetX;
    mapCellPosition.y = screenCellPosition.y - kMapOffsetY;

    return mapCellPosition.x >= 0 && mapCellPosition.x < kMapWidth
        && mapCellPosition.y >= 0 && mapCellPosition.y < kMapHeight;
}

// 한 점이 사각형 안에 포함되는지 확인한다.
bool IsPointInsideRect(const POINT& point, const Rect& rect)
{
    return point.x >= rect.x
        && point.x < rect.x + rect.width
        && point.y >= rect.y
        && point.y < rect.y + rect.height;
}

// 버퍼의 지정한 위치에 문자열을 그린다.
void DrawText(std::vector<CHAR_INFO>& buffer, int x, int y, const std::string& text)
{
    if (y < 0 || y >= kScreenHeight)
        return;

    for (size_t i = 0; i < text.size(); ++i)
    {
        int drawX = x + static_cast<int>(i);

        if (drawX < 0 || drawX >= kScreenWidth)
            continue;

        int index = y * kScreenWidth + drawX;
        buffer[index].Char.AsciiChar = text[i];
        buffer[index].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
}

// 사각형과 마우스 위치를 콘솔 격자로 시각화한다.
void DrawCollisionMap(std::vector<CHAR_INFO>& buffer, const Rect& rect, const POINT& mapCellPosition, bool isInsideMapArea)
{
    for (int y = 0; y < kMapHeight; ++y)
    {
        for (int x = 0; x < kMapWidth; ++x)
        {
            bool isInsideRect = x >= rect.x
                && x < rect.x + rect.width
                && y >= rect.y
                && y < rect.y + rect.height;

            bool isMouseHere = isInsideMapArea && mapCellPosition.x == x && mapCellPosition.y == y;
            char drawChar = '.';

            if (isInsideRect && isMouseHere)
                drawChar = 'X';
            else if (isMouseHere)
                drawChar = 'M';
            else if (isInsideRect)
                drawChar = '#';

            int index = (y + kMapOffsetY) * kScreenWidth + (x + kMapOffsetX);
            buffer[index].Char.AsciiChar = drawChar;
            buffer[index].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        }
    }
}

// 현재 마우스 상태를 버퍼에 출력한다.
void DrawMouseState(std::vector<CHAR_INFO>& buffer, const POINT& mousePosition, const POINT& screenCellPosition, const POINT& mapCellPosition, bool isLeftPressed, bool isInsideMapArea, bool isInsideRect)
{
    DrawText(buffer, 2, 0, "Mouse Collision Example");
    DrawText(buffer, 2, 1, "Move the mouse into the box and left click to exit.");
    DrawText(buffer, 2, 2, "#: Rect Area, M: Mouse, X: Collision Point");
    DrawText(buffer, 2, 27, "Screen Mouse Position: (" + std::to_string(mousePosition.x) + ", " + std::to_string(mousePosition.y) + ")");

    if (screenCellPosition.x >= 0 && screenCellPosition.y >= 0)
        DrawText(buffer, 2, 28, "Console Cell Position: (" + std::to_string(screenCellPosition.x) + ", " + std::to_string(screenCellPosition.y) + ")");
    else
        DrawText(buffer, 2, 28, "Console Cell Position: Outside Console Window");

    if (isInsideMapArea)
        DrawText(buffer, 2, 29, "Map Cell Position: (" + std::to_string(mapCellPosition.x) + ", " + std::to_string(mapCellPosition.y) + ")");
    else
        DrawText(buffer, 2, 29, "Map Cell Position: Outside Collision Map");

    DrawText(buffer, 2, 30, std::string("Left Button: ") + (isLeftPressed ? "Pressed" : "Released"));
    DrawText(buffer, 2, 31, std::string("Inside Rect: ") + (isInsideRect ? "Yes" : "No"));
}

// 한 프레임 전체를 버퍼에 그린다.
void RenderFrame(HANDLE bufferHandle, const Rect& rect, const POINT& mousePosition, const POINT& screenCellPosition, const POINT& mapCellPosition, bool isLeftPressed, bool isInsideMapArea, bool isInsideRect)
{
    std::vector<CHAR_INFO> buffer(kScreenWidth * kScreenHeight);

    for (CHAR_INFO& cell : buffer)
    {
        cell.Char.AsciiChar = ' ';
        cell.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }

    DrawText(buffer, 2, 3, "Rect : x = " + std::to_string(rect.x)
        + ", y = " + std::to_string(rect.y)
        + ", width = " + std::to_string(rect.width)
        + ", height = " + std::to_string(rect.height));

    DrawCollisionMap(buffer, rect, mapCellPosition, isInsideMapArea);
    DrawMouseState(buffer, mousePosition, screenCellPosition, mapCellPosition, isLeftPressed, isInsideMapArea, isInsideRect);
    DrawText(buffer, 2, 25, isInsideRect ? "Collision Detected." : "No Collision.");

    SMALL_RECT writeRegion = { 0, 0, static_cast<SHORT>(kScreenWidth - 1), static_cast<SHORT>(kScreenHeight - 1) };
    COORD bufferSize = { static_cast<SHORT>(kScreenWidth), static_cast<SHORT>(kScreenHeight) };
    COORD bufferOrigin = { 0, 0 };

    WriteConsoleOutputA(bufferHandle, buffer.data(), bufferSize, bufferOrigin, &writeRegion);
}

// 더블 버퍼로 사용할 콘솔 버퍼를 설정한다.
void ConfigureBuffer(HANDLE bufferHandle)
{
    COORD bufferSize = { static_cast<SHORT>(kScreenWidth), static_cast<SHORT>(kScreenHeight) };
    SetConsoleScreenBufferSize(bufferHandle, bufferSize);

    SMALL_RECT windowRect = { 0, 0, static_cast<SHORT>(kScreenWidth - 1), static_cast<SHORT>(kScreenHeight - 1) };
    SetConsoleWindowInfo(bufferHandle, TRUE, &windowRect);

    CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
    SetConsoleCursorInfo(bufferHandle, &cursorInfo);
}
