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

#include <string_view>
#include <string>
#include <iostream>
#include <memory>
#include <chrono>
#include "generator.h"
#include "main.h"

constexpr std::string_view arg_help = "--help";
constexpr std::string_view arg_itemspath = "--itemsgame-path";
constexpr std::string_view arg_halloween = "--include-halloween";
constexpr std::string_view arg_christmas = "--include-christmas";
constexpr std::string_view arg_noalt = "--exclude-alternatives";
constexpr std::string_view arg_nocore = "--exclude-core";
constexpr std::string_view arg_includecomp = "--include-competitive-maps";
constexpr std::string_view arg_nocomments = "--no-comments";
constexpr std::string_view arg_noduplicates = "--no-duplicates";
constexpr std::string_view arg_unlisted = "--include-unlisted-maps";
constexpr std::string_view productversion = "1.1.0";

namespace utils
{
	static void print_help()
	{
		std::cout << "Parameters:" << "\n";
		std::cout << "--help: Prints this message." << "\n";
		std::cout << "--itemsgame-path: Full path to the items_game.txt file to be parsed." << "\n";
		std::cout << "--include-halloween: Include halloween maps." << "\n";
		std::cout << "--include-christmas: Include christmas maps." << "\n";
		std::cout << "--exclude-alternatives: Exclude maps tagged as 'alternative'." << "\n";
		std::cout << "--exclude-core: Exclude maps tagged as 'core'." << "\n";
		std::cout << "--include-competitive-maps: Exclude maps tagged as '6v6 competitive'." << "\n";
		std::cout << "--no-comments: Do not add comments to the mapcycle file." << "\n";
		std::cout << "--no-duplicates: Do not write duplicate maps to the mapcycle file." << "\n";
		std::cout << "--include-unlisted-maps: Include unlisted maps." << "\n";
		std::cout << std::endl;
	}

	static bool should_print_help(const int& argc, char** argv)
	{
		for (int i = 0; i < argc; i++)
		{
			std::string szArg(argv[i]);

			if (szArg == arg_help)
			{
				return true;
			}
		}

		return false;
	}

	static bool find_itemsgame_path(const int& argc, char** argv, int& argpath)
	{
		for (int i = 0; i < argc; i++)
		{
			std::string szArg(argv[i]);

			if (szArg == arg_itemspath)
			{
				if (i + 1 >= argc)
				{
					return false;
				}

				argpath = i + 1;
				return true;
			}
		}

		return false;
	}

	static void parse_options(const int& argc, char** argv, GeneratorOptions& options)
	{
		for (int i = 0; i < argc; i++)
		{
			std::string szArg(argv[i]);

			if (szArg == arg_christmas)
			{
				options.christmas = true;
				continue;
			}

			if (szArg == arg_halloween)
			{
				options.halloween = true;
				continue;
			}

			if (szArg == arg_nocore)
			{
				options.no_core = true;
				continue;
			}

			if (szArg == arg_noalt)
			{
				options.no_alternatives = true;
				continue;
			}

			if (szArg == arg_includecomp)
			{
				options.no_comp = false;
				continue;
			}

			if (szArg == arg_noduplicates)
			{
				options.no_duplicates = true;
				continue;
			}

			if (szArg == arg_nocomments)
			{
				options.add_comments = false;
				continue;
			}

			if (szArg == arg_unlisted)
			{
				options.unlisted = true;
				continue;
			}
		}
	}
}


int main(int argc, char** argv)
{
	std::cout << "caxanga334's mapcycle generator for TF2." << "\n";
	std::cout << "Version: " << productversion << std::endl;

	if (utils::should_print_help(argc, argv))
	{
		utils::print_help();
		return 0;
	}

	int argpath = 0;

	if (!utils::find_itemsgame_path(argc, argv, argpath))
	{
		std::cerr << "--itemsgame-path not set." << std::endl;
		
		utils::print_help();

		return 0;
	}

	std::filesystem::path path(argv[argpath]);

	if (!std::filesystem::exists(path))
	{
		std::cerr << "Input file \"" << path << "\" does not exists!" << std::endl;
	}

	std::unique_ptr<MapCycleGenerator> generator = std::make_unique<MapCycleGenerator>(path);
	GeneratorOptions options;

	utils::parse_options(argc, argv, options);
	generator->SetOptions(options);

	auto start = std::chrono::high_resolution_clock::now();
	generator->Generate();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = (end - start);

	std::cout << "Mapcycle generated in " << duration.count() << " ms." << std::endl;

	return 0;
}