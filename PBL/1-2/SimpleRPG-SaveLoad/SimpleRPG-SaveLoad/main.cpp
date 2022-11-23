#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <format>
#include <cassert>
#include <Windows.h>
#include "InputMenu.h"
#include "GameData.h"
#include "Serializer.h"

static int Rand(GameData& data)
{
    int ret = rand();
    data.Seed *= static_cast<uint32_t>(GetTickCount64());
    data.Seed *= 112345;
    data.Seed += 214325;
    data.Seed *= 11;
    srand(data.Seed);
    return ret;
}

/// <summary>
/// 새로운 게임을 시작합니다.
/// </summary>
static void NewGame();

/// <summary>
/// save.dat 에서 데이터를 읽어 게임을 로드합니다.
/// </summary>
static void Load();

/// <summary>
/// 메인 게임 루프로 진입합니다.
/// </summary>
/// <param name="data">게임 데이터</param>
static void Play(GameData& data);

/// <summary>
/// 사용자에게 이름을 입력받습니다.
/// </summary>
/// <param name="data">게임 데이터</param>
static void PickName(GameData& data);

/// <summary>
/// 스킬을 사용하여 적을 공격합니다.
/// </summary>
/// <param name="data">게임 데이터</param>
static void UseSkill(GameData& data);

/// <summary>
/// 일반 공격으로 적을 공격합니다.
/// </summary>
/// <param name="data">게임 데이터</param>
static void Attack(GameData& data);

/// <summary>
/// 경험치를 획득합니다.
/// </summary>
/// <param name="data">게임 데이터</param>
/// <param name="exp">획득할 경험치량</param>
static void GainExp(GameData& data, int32_t exp);

/// <summary>
/// 적에게 랜덤으로 데미지를 입힙니다. 최소 데미지 및 최대 데미지는 0보다 커야하며, 최소 데미지보다 최대 데미지가 커야합니다.
/// </summary>
/// <param name="data">게임 데이터</param>
/// <param name="minDamage">최소 데미지</param>
/// <param name="maxDamage">최대 데미지</param>
static void DealDamage(GameData& data, int32_t minDamage, int32_t maxDamage);

/// <summary>
/// 적으로부터 5 ~ 9 사이의 랜덤 피격 데미지를 입습니다.
/// </summary>
/// <param name="data">게임 데이터</param>
static void Hit(GameData& data);

/// <summary>
/// HP를 1 ~ 20 사이 만큼 랜덤으로 회복합니다.
/// </summary>
/// <param name="data">게임 데이터</param>
static void RecoverHP(GameData& data);

/// <summary>
/// MP를 2 ~ 11 사이 만큼 랜덤으로 회복합니다.
/// </summary>
/// <param name="data">게임 데이터</param>
static void RecoverMP(GameData& data);

/// <summary>
/// save.dat에 현재 게임 데이터를 저장합니다.
/// </summary>
/// <param name="data">게임 데이터</param>
static void Save(GameData& data);

int main()
{
    std::cout << "[Cool Fantasy] 에 오신 것을 환영합니다." << std::endl;
    
    InputMenu menu({ "새로운 게임", "불러오기" });

    switch (menu.Ask())
    {
    case 1:
        NewGame();
        break;
    case 2:
        Load();
        break;
    }

    return 0;
}

void NewGame()
{
    GameData data;
    data.Seed = static_cast<uint32_t>(time(NULL) * GetTickCount64());
    srand(data.Seed);

    PickName(data);
    std::cout << std::format("당신의 이름은 [{}] 입니다.", data.CharacterData.Name) << std::endl;

    Play(data);
}

void Load()
{
    std::ifstream ifs("save.dat", std::ios::binary);
    if (!ifs)
    {
        std::cout << "저장 파일을 읽어올 수 없습니다.";
        return;
    }
    ifs.exceptions(ifs.exceptions() | std::ios::failbit);

    GameData data;
    Serializer<GameData> s(ifs);
    try
    {
        s.Load(data);
    }
    catch (const std::ios::failure& fail)
    {
        std::cout << "저장 파일을 읽어내지 못했습니다." << std::endl;
        std::cout << fail.what() << std::endl;
        return;
    }

    Play(data);
}

void Play(GameData& data)
{
    while (true)
    {
        std::cout << std::endl;
        std::cout << std::format("[{}] Lv. {} HP[{}]  MP [{}]  EXP [{}/{}]",
            data.CharacterData.Name,
            data.CharacterData.Level,
            data.CharacterData.Health,
            data.CharacterData.Mana,
            data.CharacterData.Exp,
            data.CharacterData.Level * 50) << std::endl;
        std::cout << std::format("적 체력: [{}]", data.EnemyHealth) << std::endl;
        std::cout << std::endl;

        InputMenu action({ "스킬사용", "공격", "HP회복", "MP회복", "저장" });
        switch (action.Ask())
        {
        case 1:
            UseSkill(data);
            Hit(data);
            break;
        case 2:
            Attack(data);
            Hit(data);
            break;
        case 3:
            RecoverHP(data);
            Hit(data);
            break;
        case 4:
            RecoverMP(data);
            Hit(data);
            break;
        case 5:
            Save(data);
            break;
        }
    }
}

void PickName(GameData& data)
{
    std::cout << "이름을 선택하십시오." << std::endl;
    InputMenu name({ "로아", "타티아나", "제임스", "이안" });
    switch (name.Ask())
    {
    case 1:
        strcpy_s(data.CharacterData.Name, "로아");
        break;
    case 2:
        strcpy_s(data.CharacterData.Name, "타티아나");
        break;
    case 3:
        strcpy_s(data.CharacterData.Name, "제임스");
        break;
    case 4:
        strcpy_s(data.CharacterData.Name, "이안");
        break;
    }
}

void UseSkill(GameData& data)
{
    constexpr int32_t mana = 13;

    if (data.CharacterData.Mana < mana)
    {
        std::cout << std::format("스킬을 사용하려면 최소한 MP {} 만큼 필요합니다.", mana) << std::endl;
        return;
    }

    data.CharacterData.Mana -= mana;
    std::cout << std::format("MP를 [{}] 만큼을 소모했습니다.", mana) << std::endl;
    DealDamage(data, 20, 30);
}

void Attack(GameData& data)
{
    DealDamage(data, 8, 13);
}

void GainExp(GameData& data, int32_t exp)
{
    data.CharacterData.Exp += exp;
    std::cout << std::format("경험치 [{}] 만큼 획득했습니다.", exp) << std::endl;
    if (data.CharacterData.Exp >= data.CharacterData.Level * 50)
    {
        data.CharacterData.Level++;
        data.CharacterData.Exp = 0;
        std::cout << "레벨이 올랐습니다!" << std::endl;
    }
}

void DealDamage(GameData& data, int32_t minDamage, int32_t maxDamage)
{
    assert(minDamage > 0);
    assert(maxDamage > 0);
    assert(maxDamage > minDamage);

    const int32_t damage = Rand(data) % (maxDamage - minDamage) + minDamage;
    data.EnemyHealth -= damage;
    if (data.EnemyHealth < 0)
        data.EnemyHealth = 0;
    std::cout << std::format("적에게 데미지 [{}] 만큼을 입혔습니다.", damage) << std::endl;

    if (data.EnemyHealth <= 0)
    {
        std::cout << "적을 쓰러뜨렸습니다." << std::endl;
        GainExp(data, Rand(data) % 5 + 30);
        data.EnemyHealth = (Rand(data) % 5 + 3) * 10;
    }
}

void Hit(GameData& data)
{
    const int32_t hit = Rand(data) % 5 + 5;
    data.CharacterData.Health -= hit;
    if (data.CharacterData.Health < 0)
        data.CharacterData.Health = 0;
    std::cout << std::format("적의 공격으로 HP를 [{}] 만큼 잃었습니다.", hit) << std::endl;
    if (data.CharacterData.Health <= 0)
    {
        std::cout << "게임 오버! 당신은 사망하였습니다." << std::endl;
        exit(0);
    }
}

void RecoverHP(GameData& data)
{
    const int32_t recover = Rand(data) % 20 + 1;
    data.CharacterData.Health += recover;
    if (data.CharacterData.Health > 100)
        data.CharacterData.Health = 100;
    std::cout << std::format("HP를 [{}] 만큼 회복합니다.", recover) << std::endl;
}

void RecoverMP(GameData& data)
{
    const int32_t recover = Rand(data) % 10 + 2;
    data.CharacterData.Mana += recover;
    if (data.CharacterData.Mana > 100)
        data.CharacterData.Mana = 100;
    std::cout << std::format("MP를 [{}] 만큼 회복합니다.", recover) << std::endl;
}

void Save(GameData& data)
{
    std::ofstream ofs("save.dat", std::ios::binary);
    if (!ofs)
    {
        std::cout << "저장에 실패하였습니다." << std::endl;
        return;
    }
    ofs.exceptions(ofs.exceptions() | std::ios::failbit);

    Serializer<GameData> s(ofs);
    try
    {
        s.Save(data);
        std::cout << "저장이 완료되었습니다." << std::endl;
    }
    catch (const std::ios::failure& fail)
    {
        std::cout << "저장에 실패하였습니다." << std::endl;
        std::cout << fail.what() << std::endl;
    }
}

