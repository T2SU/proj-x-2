#include "pch.h"
#include "GameData.h"

GameData::GameData()
	: CharacterData()
	, Seed()
	, Version(GAME_VERSION)
	, EnemyHealth(50)
{
}

void	GameData::Serialize(Serializer<GameData>& serializer) const
{
	serializer.Write(Version);
	serializer.Write(Seed);
	serializer.Write(EnemyHealth);
	serializer.Write(CharacterData);
}

void	GameData::Deserialize(Serializer<GameData>& serializer)
{
	Version = serializer.Read<decltype(Version)>();
	Seed = serializer.Read<decltype(Seed)>();
	EnemyHealth = serializer.Read<decltype(EnemyHealth)>();
	CharacterData = serializer.Read<decltype(CharacterData)>();
}