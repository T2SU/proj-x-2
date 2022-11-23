#pragma once

#include "CharacterData.h"
#include "Serializer.h"
#include <cstdint>

constexpr int32_t GAME_VERSION = 100;

struct GameData : public ISerializable<GameData>
{
	GameData();

	CharacterData CharacterData;
	uint32_t	Seed;
	int32_t		Version;
	int32_t		EnemyHealth;

	virtual void	Serialize(Serializer<GameData>& serializer) const override;
	virtual void	Deserialize(Serializer<GameData>& serializer) override;
};
