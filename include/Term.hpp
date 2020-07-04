#pragma once

#include <cstdint>
#include <sys/ioctl.h>
#include <termios.h>

// Term or Terminal?
class Term final
{
public:
	Term();
	~Term();

	void cls();
	void clear();

	// MAYBE: A pipe interface?
	void echo(const bool);
	void icanon(const bool);
	void alt_screen(const bool);
	void show_cursor(const bool);

	void flush();

	void update_size();

	void handle_interrupt(int);
	void handle_resize(int);

	// TODO: color interface

private:
	struct termios m_prev_attr;
	struct termios m_curr_attr;

	// TODO: Update when SIGWINCH is caught
	uint16_t m_width;
	uint16_t m_height;

	uint16_t m_x;
	uint16_t m_y;
};
