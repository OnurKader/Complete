#include "Timer.hpp"
#include "Trie.hpp"

#include <fmt/format.hpp>
#include <fmt/ranges.hpp>
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
		Timer trie_timer {"Trie inserting 58112 words"};
		// Takes ~33ms
		while(dictionary_file >> dictionary_word)
		{
			trie.push(dictionary_word);
		}
	}

	massert(trie.word_count() == 58112ULL, "Couldn't insert all the words");
	massert(trie.search("the"), "Couldn't find 'the'");
	massert(trie.search("abbreviations"), "Couldn't find 'abbreviations'");
	massert(trie.search("police"), "Couldn't find 'police'");
	massert(trie.search("floor"), "Couldn't find 'floor'");
	massert(trie.search("color"), "Couldn't find 'color'");
	massert(trie.search("colour"), "Couldn't find 'colour'");
	massert(!trie.search("onur"), "Luckily didn't find 'onur'");

	{
		Timer match_timer {"Matching 10 words"};
		const std::vector<std::string> words = trie.get_matches("the", 10ULL);

		fmt::print("the* words: {}\n", words);
	}

	return 0;
}
