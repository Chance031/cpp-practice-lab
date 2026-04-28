#include <iostream>
#include <string>

void PrintPlayerStatus(int hp, int maxHp, int mp, int maxMp, int level, const std::string& state);

int main()
{
    int maxHp = 100;            // 캐릭터의 최대 HP를 나타낸다.
    int hp = maxHp;             // 캐릭터의 현재 HP를 나타낸다.
    int maxMp = 50;             // 캐릭터의 최대 MP를 나타낸다.
    int mp = maxMp;             // 캐릭터의 현재 MP를 나타낸다.
    int level = 1;              // 캐릭터의 레벨을 나타낸다.
    std::string state = "Idle"; // 캐릭터의 현재 상태를 나타낸다.

    PrintPlayerStatus(hp, maxHp, mp, maxMp, level, state);

    std::cout << "\n몬스터에게 공격을 당했다.\n데미지를 10 입었다.\n\n";

    hp -= 10;

    PrintPlayerStatus(hp, maxHp, mp, maxMp, level, state);

    return 0;
}

void PrintPlayerStatus(int hp, int maxHp, int mp, int maxMp, int level, const std::string& state)
{
    std::cout << "HP: " << hp << '/' << maxHp << '\n';
    std::cout << "MP: " << mp << '/' << maxMp << '\n';
    std::cout << "Level: " << level << '\n';
    std::cout << "State: " << state << '\n';
}