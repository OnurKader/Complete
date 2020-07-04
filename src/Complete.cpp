#include "Term.hpp"
#include "Trie.hpp"

#include <cctype>
#include <csignal>
#include <fmt/format.hpp>
#include <fmt/ranges.hpp>
#include <fstream>
#include <iostream>

void handle_interrupt(int);
void handle_resize(int);
std::string to_lower(const std::string& str);

bool resized = false;

// FIXME: Something to do with the terminal size
const std::size_t match_count = 27ULL;

int main(int, char** argv)
{
	std::signal(SIGINT, handle_interrupt);
	std::signal(SIGWINCH, handle_resize);

	OK::Trie dictionary_trie;

	std::string cwd {argv[0]};
	cwd.erase(cwd.begin() + cwd.find_last_of('/'), cwd.end());

	std::ifstream dictionary_file {cwd + "/../data/dictionary.txt"};
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

	Term term;
	term.echo(false);
	term.icanon(false);
	term.show_cursor(true);

	term.cls();

	bool running = true;

	std::vector<std::string> previous_user_words;
	previous_user_words.reserve(4ULL);

	std::size_t vector_index = 0ULL;

	std::string user_input;
	while(running)
	{
		fmt::print("\033[H\033[32;1m\u276F\033[m {}", user_input);
		char chr;
		std::cin >> std::noskipws >> chr;

		// MAYBE: Put this switch into a function?
		switch(chr)
		{
			case 4:
			case EOF: running = false; break;

			case 127:
			case '\b':
			{
				if(!user_input.empty())
					user_input.pop_back();

				term.cls();
				const auto suggestions = dictionary_trie.get_matches(user_input, match_count);
				if(!suggestions.empty())
				{
					fmt::print("\033[2H  \033[34m{}\033[m",
							   fmt::join(suggestions.cbegin(), suggestions.cend(), "\n  "));
				}

				break;
			}

			case 10:
				previous_user_words.emplace_back(std::move(user_input));
				term.cls();
				break;

			case '\033':
			{
				std::cin >> chr;
				switch(chr)
				{
					case '[':
					{
						std::cin >> chr;
						switch(chr)
						{
							case 'A':
							{
								// FIXME: Function or just clean the word getting part
								if(vector_index != previous_user_words.size())
									++vector_index;
								user_input = *(previous_user_words.cend() - vector_index);
								term.cls();
								// This below needs to be a local lambda
								const auto suggestions =
									dictionary_trie.get_matches(user_input, match_count);
								if(!suggestions.empty())
								{
									fmt::print("\033[2H  \033[34m{}\033[m",
											   fmt::join(suggestions.cbegin(),
														 suggestions.cend(),
														 "\n  "));
								}
								break;
							}	 // Up
							case 'B':
							{
								if(vector_index != 0)
									--vector_index;
								user_input = *(previous_user_words.cend() - vector_index);
								term.cls();
								// This below needs to be a local lambda
								const auto suggestions =
									dictionary_trie.get_matches(user_input, match_count);
								if(!suggestions.empty())
								{
									fmt::print("\033[2H  \033[34m{}\033[m",
											   fmt::join(suggestions.cbegin(),
														 suggestions.cend(),
														 "\n  "));
								}
								break;
							}					// Down
							case 'C':			// Right
							case 'D': break;	// Left
							default: fmt::print("{}", chr);
						}

						break;
					}
				}

				break;
			}
			default:
			{
				term.cls();
				user_input.push_back(chr);
				const auto suggestions = dictionary_trie.get_matches(user_input, match_count);
				if(!suggestions.empty())
				{
					fmt::print("\033[2H  \033[34m{}\033[m",
							   fmt::join(suggestions.cbegin(), suggestions.cend(), "\n  "));
				}
				break;
			}
		}

		if(resized)
		{
			term.update_size();
			resized = false;
		}
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

void handle_interrupt(int)
{
	fmt::print("\033[m\033[?1049l\033[?25h");
	std::exit(0);
}

void handle_resize(int) { resized = true; }
