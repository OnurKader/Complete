#include "Timer.hpp"
#include "Trie.hpp"

#include <fmt/format.hpp>
#include <fstream>

int main()
{
	OK::Trie trie;
	std::ifstream dictionary_file {"../data/dictionary.txt"};
	if(!dictionary_file)
	{
		fmt::print("Couldn't open dictionary file '../data/dictionary.txt'\n");
		return 1;
	}

	std::string dictionary_word;
	{
		Timer trie_timer {"Trie inserting 58109 words"};
		// Takes ~34ms
		while(dictionary_file >> dictionary_word)
		{
			trie.push(dictionary_word);
		}
	}

	massert(trie.word_count() == 58109ULL, "Couldn't insert all the words");

	return 0;
}
