#include <iostream>

void PrintPlayerPosition(int x, int y);
void MovePlayer(char input, int& x, int& y);

int main()
{
    int playerX = 0;
    int playerY = 0;
    char input = '\0';

    std::cout << "=== Keyboard Input Practice ===\n";
    std::cout << "Use w, a, s, d to move.\n";
    std::cout << "Press q to quit.\n\n";

    while (true)
    {
        PrintPlayerPosition(playerX, playerY);

        std::cout << "Input: ";
        std::cin >> input;

        if (input == 'q')
            break;

        MovePlayer(input, playerX, playerY);
    }

    std::cout << "\nProgram Ended.\n";

    return 0;
}

void PrintPlayerPosition(int x, int y)
{
    std::cout << "Player Position: (" << x << ", " << y << ")\n";
}

void MovePlayer(char input, int& x, int& y)
{
    if (input == 'w')
        y += 1;
    else if (input == 's')
        y -= 1;
    else if (input == 'a')
        x -= 1;
    else if (input == 'd')
        x += 1;
    else
        std::cout << "Invalid Input.\n";

    std::cout << '\n';
}
