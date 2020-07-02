#include "Trie.hpp"

#include <fmt/format.hpp>
#include <fmt/ranges.hpp>
#include <fstream>
#include <iostream>

std::string to_lower(const std::string& str);

int main()
{
	OK::Trie dictionary_trie;

	std::ifstream dictionary_file {"../data/dictionary.txt"};
	if(!dictionary_file)
	{
		fmt::print("Couldn't open dictionary file '../data/dictionary.txt'\n");
		return 1;
	}

	{
		std::string dictionary_word;
		while(dictionary_file >> dictionary_word)
			dictionary_trie.push(dictionary_word);
	}

	// TODO: Add nice REPL stuff like a history with up and down keys, maybe going back and editing
	// the buffer?

	// REPL part
	std::string user_input;
	fmt::print("\033[32;1m\u276F\033[m ");
	while(std::cin >> user_input)
	{
		const auto suggestions = dictionary_trie.get_matches(to_lower(user_input), 8ULL);
		if(suggestions.empty())
		{
			fmt::print(stderr, "\033[31;1mNo matches found\033[m\n");
		}
		else
		{
			fmt::print("  \033[34m{}\033[m\n",
					   fmt::join(suggestions.cbegin(), suggestions.cend(), "\n  "));
		}

		fmt::print("\033[32;1m\u276F\033[m ");
	}

	return 0;
}

std::string to_lower(const std::string& str)
{
	std::string temp {str};
	std::transform(temp.cbegin(), temp.cend(), temp.begin(), [](const auto& chr) {
		return std::tolower(chr);
	});
	return temp;
}
