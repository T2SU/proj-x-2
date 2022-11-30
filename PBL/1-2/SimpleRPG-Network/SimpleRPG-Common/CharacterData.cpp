#include "pch.h"
#include "CharacterData.h"

CharacterData::CharacterData()
	: Name()
	, Level(1)
	, Exp()
	, Health(100)
	, Mana(100)
{
}

void	CharacterData::Serialize(Serializer<CharacterData>& serializer) const
{
	serializer.WriteBuffer(Name, sizeof(Name));
	serializer.Write(Level);
	serializer.Write(Exp);
	serializer.Write(Health);
	serializer.Write(Mana);
}

void	CharacterData::Deserialize(Serializer<CharacterData>& serializer)
{
	serializer.ReadBuffer(Name, sizeof(Name));
	Level = serializer.Read<decltype(Level)>();
	Exp = serializer.Read<decltype(Exp)>();
	Health = serializer.Read<decltype(Health)>();
	Mana = serializer.Read<decltype(Mana)>();
}