#include <iostream>
#include <conio.h>  // _getch()를 사용하기 위한 라이브러리

void PrintInputKey(int keyCode);
void PrintSpecialKey(int keyCode);

int main()
{
    int keyCode = 0;

    std::cout << "키보드를 입력하십시오.\n";
    std::cout << "일반 키와 방향키를 확인할 수 있습니다.\n";
    std::cout << "q 또는 Q를 누르면 종료합니다.\n\n";

    while (true)
    {
        // Enter 없이 입력된 키를 바로 읽어온다.
        keyCode = _getch();

        // q 또는 Q를 누르면 반복을 종료한다.
        if (keyCode == 'q' || keyCode == 'Q')
            break;

        // 특수 키는 0 또는 224가 먼저 들어오므로, 한 번 더 읽어서 실제 키 값을 확인한다.
        if (keyCode == 0 || keyCode == 224)
        {
            keyCode = _getch();
            PrintSpecialKey(keyCode);
        }
        else
        {
            PrintInputKey(keyCode);
        }
    }

    return 0;
}

// 일반 키의 문자와 코드 값을 출력한다.
void PrintInputKey(int keyCode)
{
    std::cout << "일반 키 입력: " << char(keyCode)
              << ", 코드 값: " << keyCode << std::endl;
}

// 특수 키의 이름과 코드 값을 출력한다.
void PrintSpecialKey(int keyCode)
{
    std::cout << "특수 키 입력: ";

    switch (keyCode)
    {
    case 72:
        std::cout << "Up Arrow";
        break;
    case 75:
        std::cout << "Left Arrow";
        break;
    case 77:
        std::cout << "Right Arrow";
        break;
    case 80:
        std::cout << "Down Arrow";
        break;
    default:
        std::cout << "Unknown Special Key";
        break;
    }

    std::cout << ", 코드 값: " << keyCode << std::endl;
}
