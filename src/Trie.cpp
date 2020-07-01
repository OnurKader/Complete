#include "Trie.hpp"

namespace OK
{
void Trie::push(const std::string& word)
{
	bool word_exists = true;
	Node* current {m_root.get()};
	for(auto&& letter: word)
	{
		auto& child_letter = current->children[char_to_index(letter)];
		if(child_letter == nullptr)
		{
			word_exists = false;
			child_letter = std::make_unique<Node>();
		}
		current = child_letter.get();
	}

	// Well this is stupid...
	m_word_count += !word_exists;
	current->is_leaf = true;
}

bool Trie::search(const std::string& word) const noexcept
{
	Node* current {m_root.get()};

	for(auto&& letter: word)
	{
		const auto child_letter = current->children[char_to_index(letter)].get();
		if(child_letter == nullptr)
			return false;
		current = child_letter;
	}

	return current->is_leaf;
}

void push_words_into_vector_recursively(const Node* root,
										char word[],
										std::vector<std::string>& vec,
										const std::size_t level,
										const std::size_t match_count)
{
	if(vec.size() >= match_count)
		return;

	if(root->is_leaf)
	{
		word[level] = '\0';
		vec.push_back(word);
	}

	for(std::size_t i = 0ULL; i < LETTER_COUNT; ++i)
	{
		if(root->children[i])
		{
			word[level] = index_to_char(i);
			push_words_into_vector_recursively(
				root->children[i].get(), word, vec, level + 1ULL, match_count);
		}
	}
}

// FIXME: okasas or some random shit like ppppp fails, if it doesn't exist. Oh maybe have a
// is_leaf'less search and do it here
std::vector<std::string> Trie::get_matches(const std::string& prefix, std::size_t match_count) const
{
	Node* current {m_root.get()};
	char word[32ULL];
	std::strcpy(word, prefix.c_str());
	std::vector<std::string> words;
	words.reserve(match_count);

	for(auto&& letter: prefix)
	{
		const auto child_letter = current->children[char_to_index(letter)].get();
		if(child_letter == nullptr)
			return {"No Matches Found"};
		current = child_letter;
	}

	push_words_into_vector_recursively(current, word, words, prefix.size(), match_count);

	return words;
}

void print_(const Node* root, char word[], const std::size_t level)
{
	if(root->is_leaf)
	{
		word[level] = '\0';
		fmt::print("{}\n", word);
	}

	for(std::size_t i = 0ULL; i < LETTER_COUNT; ++i)
	{
		if(root->children[i])
		{
			word[level] = index_to_char(i);
			print_(root->children[i].get(), word, level + 1ULL);
		}
	}
}

void Trie::print()
{
	char word[32ULL];
	print_(m_root.get(), word, 0ULL);
}

}	 // namespace OK
