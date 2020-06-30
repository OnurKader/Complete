#include "Trie.hpp"

#include <fmt/format.hpp>
#include <fstream>

int main()
{
	OK::Trie dictionary_trie;

	std::ifstream dictionary_file {"../data/dictionary.txt"};
	if(!dictionary_file)
	{
		fmt::print("Couldn't open dictionary file '../data/dictionary.txt'\n");
		return 1;
	}

	std::string dictionary_word;
	while(dictionary_file >> dictionary_word)
		dictionary_trie.push(dictionary_word);
	fmt::print(stderr, "Loaded dictionary words\n");

	// REPL part

	return 0;
}
