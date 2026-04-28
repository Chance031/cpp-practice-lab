#include <iostream>
#include <string>

enum class PlayerState
{
    Idle,
    Burn
};

std::string GetStateName(PlayerState state);
void PrintPlayerStatus(int hp, int maxHp, int mp, int maxMp, int level, int experience, int maxExperience, PlayerState state);
int ApplyDamage(int currentHp, int amount);
int ApplyStateDamage(int currentHp, PlayerState state);
int HealHp(int currentHp, int maxHp, int amount);
int UseMp(int currentMp, int amount);
int RecoverMp(int currentMp, int maxMp, int amount);
int LevelUp(int currentLevel);
int GainExperience(int currentExperience, int maxExperience, int gainedExperience, int& level);

int main()
{
    int maxHp = 100;                        // 최대 체력
    int hp = maxHp;                         // 현재 체력
    int maxMp = 50;                         // 최대 마나
    int mp = maxMp;                         // 현재 마나
    int level = 1;                          // 현재 레벨
    int experience = 0;                     // 현재 경험치
    int maxExperience = 100;                // 레벨업에 필요한 경험치
    PlayerState state = PlayerState::Idle;  // 현재 상태이상

    PrintPlayerStatus(hp, maxHp, mp, maxMp, level, experience, maxExperience, state);

    std::cout << "\n몬스터에게 공격을 당했다.\n데미지를 10 입었다.\n화상 상태이상에 걸렸다.\n\n";

    hp = ApplyDamage(hp, 10);
    state = PlayerState::Burn;

    std::cout << "화상 때문에 추가 데미지를 입었다.\n\n";
    hp = ApplyStateDamage(hp, state);

    PrintPlayerStatus(hp, maxHp, mp, maxMp, level, experience, maxExperience, state);

    std::cout << "\n포션을 사용해 체력을 15 회복했다.\n상태를 회복했다.\n\n";

    hp = HealHp(hp, maxHp, 15);
    state = PlayerState::Idle;

    PrintPlayerStatus(hp, maxHp, mp, maxMp, level, experience, maxExperience, state);

    std::cout << "\n스킬을 사용해 MP를 10 소모했다.\n\n";

    mp = UseMp(mp, 10);

    PrintPlayerStatus(hp, maxHp, mp, maxMp, level, experience, maxExperience, state);

    std::cout << "\n몬스터를 처치해 경험치 120을 획득했다.\n\n";

    experience = GainExperience(experience, maxExperience, 120, level);

    PrintPlayerStatus(hp, maxHp, mp, maxMp, level, experience, maxExperience, state);

    return 0;
}

std::string GetStateName(PlayerState state)
{
    switch (state)
    {
    case PlayerState::Burn:
        return "Burn";
    case PlayerState::Idle:
    default:
        return "Idle";
    }
}

// 플레이어의 현재 수치를 화면에 출력한다.
void PrintPlayerStatus(int hp, int maxHp, int mp, int maxMp, int level, int experience, int maxExperience, PlayerState state)
{
    std::cout << "=== Player Status ===\n";
    std::cout << "HP: " << hp << '/' << maxHp << '\n';
    std::cout << "MP: " << mp << '/' << maxMp << '\n';
    std::cout << "Level: " << level << '\n';
    std::cout << "EXP: " << experience << '/' << maxExperience << '\n';
    std::cout << "State: " << GetStateName(state) << '\n';
}

// 받은 데미지만큼 체력을 감소시킨다.
int ApplyDamage(int currentHp, int amount)
{
    currentHp -= amount;

    if (currentHp <= 0)
        currentHp = 0;

    return currentHp;
}

// 현재 상태이상에 따라 추가 데미지를 적용한다.
int ApplyStateDamage(int currentHp, PlayerState state)
{
    if (state == PlayerState::Burn)
        currentHp = ApplyDamage(currentHp, 5);

    return currentHp;
}

// 지정한 수치만큼 체력을 회복한다.
int HealHp(int currentHp, int maxHp, int amount)
{
    currentHp += amount;

    if (currentHp > maxHp)
        currentHp = maxHp;

    return currentHp;
}

// 지정한 수치만큼 마나를 소모한다.
int UseMp(int currentMp, int amount)
{
    currentMp -= amount;

    if (currentMp <= 0)
        currentMp = 0;

    return currentMp;
}

// 지정한 수치만큼 마나를 회복한다.
int RecoverMp(int currentMp, int maxMp, int amount)
{
    currentMp += amount;

    if (currentMp > maxMp)
        currentMp = maxMp;

    return currentMp;
}

// 레벨을 1 증가시킨다.
int LevelUp(int currentLevel)
{
    return currentLevel + 1;
}

// 경험치를 더하고, 최대 경험치를 넘기면 레벨업을 처리한다.
int GainExperience(int currentExperience, int maxExperience, int gainedExperience, int& level)
{
    currentExperience += gainedExperience;

    if (currentExperience >= maxExperience)
    {
        int remainingExperience = currentExperience - maxExperience;
        level = LevelUp(level);
        currentExperience = remainingExperience;
    }

    return currentExperience;
}
