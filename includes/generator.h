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

struct GeneratorOptions
{
	GeneratorOptions()
	{
		halloween = false;
		christmas = false;
		unlisted = false;
		no_alternatives = false;
		no_duplicates = false;
		add_comments = true;
		no_comp = true;
		no_core = false;
	}

	GeneratorOptions& operator= (const GeneratorOptions& other)
	{
		this->halloween = other.halloween;
		this->christmas = other.christmas;
		this->unlisted = other.unlisted;
		this->no_alternatives = other.no_alternatives;
		this->no_duplicates = other.no_duplicates;
		this->add_comments = other.add_comments;
		this->no_comp = other.no_comp;
		this->no_core = other.no_core;
		return *this;
	}

	bool halloween;
	bool christmas;
	bool unlisted;
	bool no_alternatives;
	bool no_duplicates;
	bool add_comments;
	bool no_comp;
	bool no_core;
};

class MapCycleGenerator
{
public:
	MapCycleGenerator(std::filesystem::path filepath);
	~MapCycleGenerator();

	void Generate();

	void SetOptions(GeneratorOptions& options)
	{
		m_options = options;
	}

private:
	std::filesystem::path m_path;
	std::vector<MapEntry> m_maps;
	GeneratorOptions m_options;

	void ParseKV();
	bool LoadFile(std::stringstream& buffer) const;
	void WriteFile();
};

#endif // !GENERATOR_H_


