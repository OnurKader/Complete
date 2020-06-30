#include "Trie.hpp"

namespace OK
{
void Trie::push(const std::string& word)
{
	bool word_exists = true;
	std::shared_ptr<Node> current {m_root};
	for(auto&& letter: word)
	{
		auto& child_letter = current->children[char_to_index(letter)];
		if(child_letter == nullptr)
		{
			word_exists = false;
			child_letter = std::make_shared<Node>();
		}
		current = child_letter;
	}

	// Well this is stupid...
	m_word_count += !word_exists;
	current->is_leaf = true;
}

}	 // namespace OK
