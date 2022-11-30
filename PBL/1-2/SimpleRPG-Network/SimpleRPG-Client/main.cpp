#include "pch.h"
#include "InputMenu.h"
#include "GameData.h"
#include "GameSession.h"
#include <SimpleRPG.h>
#include <OutBuffer.h>

static uv_loop_t g_loop;
static GameSession g_session;

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

static void Connect(const char* host, uint16_t port);

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


//std::cout << "서버에 접속합니다." << std::endl;
//uv_loop_init(&g_loop);

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);
    uv_loop_init(&g_loop);

    Connect("127.0.0.1", PORT);

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

void Connect(const char* host, uint16_t port)
{
    std::cout << "서버에 연결합니다. " << host << ":" << port << std::endl;
    g_session.Connect(&g_loop, host, port);
    std::thread thread([]() {
        uv_run(&g_loop, UV_RUN_DEFAULT);
    });
    thread.detach();
    g_session.Wait();
    if (!g_session.IsConnected())
    {
        std::cerr << "서버에 연결하지 못했습니다." << std::endl;
        exit(1);
    }
}

void NewGame()
{
    GameData& data = g_session.GetGameData();
    data.Seed = static_cast<uint32_t>(time(NULL) * GetTickCount64());
    srand(data.Seed);

    PickName(data);
    std::cout << std::format("당신의 이름은 [{}] 입니다.", data.CharacterData.Name) << std::endl;

    Play(data);
}

void Load()
{
    OutBuffer outBuffer;

    std::string line;
    std::cout << "게임을 불러오기 위해 이름과 패스워드를 입력해주세요." << std::endl;

    std::cout << "이름: ";
    std::getline(std::cin, line);
    outBuffer.WriteString(line);

    std::cout << "패스워드: ";
    std::getline(std::cin, line);
    outBuffer.WriteString(line);

    g_session.Unset();
    g_session.Send(ClientToServer_LoadReq, outBuffer);
    g_session.Wait();

    GameData& data = g_session.GetGameData();
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
    if (!g_session.IsSaveAuthed())
    {
        OutBuffer outBuffer;
        std::string line;
        std::cout << "게임을 저장하기 위해 이름과 패스워드를 입력해주세요." << std::endl;

        std::cout << "이름: ";
        std::getline(std::cin, line);
        outBuffer.WriteString(line);

        std::cout << "패스워드: ";
        std::getline(std::cin, line);
        outBuffer.WriteString(line);

        g_session.Unset();
        g_session.Send(ClientToServer_SaveAuthReq, outBuffer);
    }
    else
    {
        g_session.RequestSave();
        g_session.Unset();
    }
    g_session.Wait();
}

