#pragma once

#include <cstdint>
#include "Serializer.h"

struct CharacterData : public ISerializable<CharacterData>
{
	CharacterData();

	char	Name[20];
	int8_t	Level;
	int32_t	Exp;
	int16_t	Health;
	int16_t	Mana;

	virtual void	Serialize(Serializer<CharacterData>& serializer) const override;
	virtual void	Deserialize(Serializer<CharacterData>& serializer) override;
};