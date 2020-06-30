#pragma once

#include "Assert.hpp"

#include <algorithm>
#include <array>
#include <memory>

namespace OK
{
static constexpr auto LETTER_COUNT = 26ULL;

// Ugh, can't design it with unique_ptr's
struct Node final
{
	std::array<std::shared_ptr<Node>, LETTER_COUNT> children {nullptr};
	bool is_leaf {false};
};

class Trie final
{
public:
	void push(const std::string& word);
	std::size_t word_count() const noexcept { return m_word_count; }

private:
	std::shared_ptr<Node> m_root {std::make_shared<Node>()};
	std::size_t m_word_count {0ULL};

	std::size_t char_to_index(const char chr) { return static_cast<std::size_t>(chr - 'a'); }
};

}	 // namespace OK
