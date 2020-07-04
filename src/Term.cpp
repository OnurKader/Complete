#include "Term.hpp"

#include <fmt/core.hpp>
#include <unistd.h>

Term::Term() : m_width {80U}, m_height {40U}, m_x {1U}, m_y {1U}
{
	if(tcgetattr(STDIN_FILENO, &m_prev_attr))
		fmt::print(stderr, "tcgetattr() failed\n");

	m_curr_attr = m_prev_attr;

	update_size();
	fmt::print("\033[H");
}

Term::~Term()
{
	// I have no idea why I used flush here, the input and output are over, it's the destructor
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_prev_attr))
		fmt::print(stderr, "tcsetattr() failed\n");
	alt_screen(false);
	show_cursor(true);
}

void Term::cls() { fmt::print("\033[H\033[2J\033[3J"); }
void Term::clear() { fmt::print("\033[H\033[2J\033[3J"); }

void Term::echo(const bool enable)
{
	if(enable)
		m_curr_attr.c_lflag |= ECHO;
	else
		m_curr_attr.c_lflag &= static_cast<tcflag_t>(~ECHO);

	flush();
}

void Term::icanon(const bool enable)
{
	if(enable)
		m_curr_attr.c_lflag |= ICANON;
	else
		m_curr_attr.c_lflag &= static_cast<tcflag_t>(~ICANON);

	flush();
}

void Term::alt_screen(const bool enable)
{
	if(enable)
		fmt::print("\033[?1049h");
	else
		fmt::print("\033[?1049l");
}

void Term::show_cursor(const bool enable)
{
	// I mean this is just puts, I hope fmt::print() doesn't do formatting stuff if it sees there
	// are no curlies
	if(enable)
		fmt::print("\033[?25h");
	else
		fmt::print("\033[?25l");
}

void Term::flush()
{
	// Doesn't actually flush, cool cool
	if(tcsetattr(STDIN_FILENO, TCSANOW, &m_curr_attr))
		fmt::print(stderr, "tcsetattr() failed\n");
}

void Term::update_size()
{
	struct winsize ws;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	m_width = ws.ws_col;
	m_height = ws.ws_row;
}

void Term::handle_interrupt(int) { alt_screen(false); }

void Term::handle_resize(int)
{
	cls();
	update_size();
}
