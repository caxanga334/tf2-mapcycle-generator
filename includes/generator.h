/*
	This file is part of a map cycle file generator for TF2 game servers.
	Copyright (C) 2024  caxanga334

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <filesystem>
#include <vector>
#include <string>
#include <sstream>

enum GameType : uint32_t
{
	GT_ATTACK_DEFENSE = 0,
	GT_CTF,
	GT_CAPTUREPOINT,
	GT_KOTH,
	GT_PAYLOAD,
	GT_PAYLOAD_RACE,
	GT_OTHER,
	GT_ARENA,
	GT_FEATURED,
	GT_HALLOWEEN,
	GT_CHRISTMAS,
	GT_POWERUP,
	GT_PASSTIME,
	GT_COMP6V6,

	MAX_GAMETYPES
};

typedef std::pair<GameType, std::string> MapEntry;

class MapCycleGenerator
{
public:
	MapCycleGenerator(std::filesystem::path filepath);
	~MapCycleGenerator();

	void Generate();

private:
	std::filesystem::path m_path;
	std::vector<MapEntry> m_maps;

	void ParseKV();
	bool LoadFile(std::stringstream& buffer) const;
	void WriteFile();
};

#endif // !GENERATOR_H_


