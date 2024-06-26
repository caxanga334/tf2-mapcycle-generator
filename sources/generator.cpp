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

#include <fstream>
#include <iostream>
#include <algorithm>
#include "vdf_parser.hpp"
#include "generator.h"

constexpr std::string_view SECTION_MAPS = "maps";
constexpr std::string_view SECTION_MAPLIST = "maplist";
constexpr std::string_view KEY_ENABLED = "enabled";
constexpr std::string_view KEY_NAME = "name";
constexpr std::string_view KEY_MMTYPE = "mm_type";
constexpr size_t MMTYPE_MIN_VALID_LENGTH = 3; // shortests one is "core"

static std::unordered_map<std::string, GameType> s_gametypelist = {
	{"attack_defense", GT_ATTACK_DEFENSE},
	{"ctf", GT_CTF},
	{"capture_point", GT_CAPTUREPOINT},
	{"koth", GT_KOTH},
	{"payload", GT_PAYLOAD},
	{"payload_race", GT_PAYLOAD_RACE},
	{"other", GT_OTHER},
	{"arena", GT_ARENA},
	{"featured", GT_FEATURED},
	{"halloween", GT_HALLOWEEN},
	{"christmas", GT_CHRISTMAS},
	{"powerup", GT_POWERUP},
	{"passtime", GT_PASSTIME},
	{"competitive_6v6", GT_COMP6V6},
	{"specialevent_placeholder", MAX_GAMETYPES}
};

static std::string s_unknowngametype = "UNKNOWN-GAME-TYPE";

static GameType StringToGameType(const std::string& name)
{
	auto type = s_gametypelist.find(name);

	if (type == s_gametypelist.end())
	{
		return MAX_GAMETYPES;
	}

	return type->second;
}

static const std::string& GameTypeToString(GameType type)
{
	for (auto& entry : s_gametypelist)
	{
		if (entry.second == type)
		{
			return entry.first;
		}
	}

	return s_unknowngametype;
}

MapCycleGenerator::MapCycleGenerator(std::filesystem::path filepath)
{
	m_path = filepath;
	m_maps.reserve(512);
}

MapCycleGenerator::~MapCycleGenerator()
{
}

void MapCycleGenerator::Generate()
{
	ParseKV();
	WriteFile();
}

void MapCycleGenerator::ParseKV()
{
	std::stringstream buffer;

	if (!LoadFile(buffer))
	{
		std::cerr << "Failed to load file!" << std::endl;
		return;
	}

	std::string file = std::string{ buffer.str() };
	auto root = tyti::vdf::read(file.cbegin(), file.cend());
	GameType currentGT = MAX_GAMETYPES;

	for (auto& child : root.childs)
	{
		if (child.first == SECTION_MAPS) // "maps"
		{
			for (auto& subcat : child.second->childs) // "attack_defense"
			{
				bool skip = false;
				currentGT = StringToGameType(subcat.first);

				for (auto& attribs : subcat.second->attribs)
				{
					if (attribs.first == KEY_MMTYPE)
					{
						if (attribs.second.size() < MMTYPE_MIN_VALID_LENGTH)
						{
							skip = true;
							break;
						}
					}
				}

				if (skip || currentGT == MAX_GAMETYPES)
				{
					continue;
				}

				for (auto& submapcat : subcat.second->childs)
				{
					if (submapcat.first == SECTION_MAPLIST)
					{
						for (auto& maplist : submapcat.second->childs)
						{
							std::string map;
							bool enabled = false;

							for (auto& mapentry : maplist.second->attribs)
							{
								if (mapentry.first == KEY_NAME)
								{
									map = mapentry.second;
								}

								if (mapentry.first == KEY_ENABLED)
								{
									enabled = std::stoi(mapentry.second) != 0;
								}
							}

							if (enabled && map.size() > 2)
							{
								m_maps.emplace_back(currentGT, map);
								enabled = false;
							}

							map.clear();
						}
					}
				}
			}

			break;
		}
	}
}

bool MapCycleGenerator::LoadFile(std::stringstream& buffer) const
{
	std::fstream file;

	file.open(m_path, std::ios::in);

	if (!file.is_open() || file.bad())
	{
		return false;
	}

	buffer << file.rdbuf();

	return true;
}

void MapCycleGenerator::WriteFile()
{
	auto path = std::filesystem::current_path();
	path = path / std::filesystem::path("mapcycle_matchmaking.txt");

	std::fstream file;
	file.open(path, std::ios::out | std::ios::trunc);

	if (!file.is_open())
	{
		std::cerr << "Failed to open " << path << " for writing!" << std::endl;
		return;
	}

	for (int i = 0; i < static_cast<int>(MAX_GAMETYPES); i++)
	{
		GameType gt = static_cast<GameType>(i);
		const std::string& gtname = GameTypeToString(gt);
		std::string buffer = "// ";
		buffer = buffer + gtname;
		
		file << buffer << std::endl;

		for (auto& map : m_maps)
		{
			if (map.first == gt)
			{
				file << map.second << std::endl;
			}
		}
	}

	file.close();
	std::cout << "Mapcycle file saved as " << path << std::endl;
}
