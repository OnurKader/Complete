#pragma once

#include "Assert.hpp"

#include <array>
#include <memory>
#include <string>
#include <vector>

namespace OK
{
static constexpr auto LETTER_COUNT = 26ULL;

struct Node final
{
	std::array<std::unique_ptr<Node>, LETTER_COUNT> children {nullptr};
	bool is_leaf {false};
};

class Trie final
{
public:
	void push(const std::string& word);
	bool search(const std::string& word) const noexcept;
	std::vector<std::string> get_matches(const std::string& prefix,
										 const std::size_t match_count) const;

	std::size_t word_count() const noexcept { return m_word_count; }

	void print();

private:
	std::unique_ptr<Node> m_root {std::make_unique<Node>()};
	std::size_t m_word_count {0ULL};
};

constexpr std::size_t char_to_index(const char chr) noexcept
{
	return static_cast<std::size_t>(chr - 'a');
}

constexpr char index_to_char(const std::size_t index) noexcept
{
	return static_cast<char>(index + 'a');
}

}	 // namespace OK

