#pragma once

#include "Assert.hpp"

#include <array>
#include <memory>
#include <string_view>
#include <vector>

namespace OK
{
static constexpr auto LETTER_COUNT = 26ULL;

// FIXME: Switch to using unique_ptr's both in Node and Trie, and just get a pointer
struct Node final
{
	std::array<std::shared_ptr<Node>, LETTER_COUNT> children {nullptr};
	bool is_leaf {false};
};

class Trie final
{
public:
	void push(const std::string& word);
	bool search(const std::string_view) const noexcept;
	std::vector<std::string> get_matches(const std::string& prefix,
										 const std::size_t match_count) const;

	std::size_t word_count() const noexcept { return m_word_count; }
	auto root() const noexcept { return m_root; }

	void print();

private:
	std::shared_ptr<Node> m_root {std::make_shared<Node>()};
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
