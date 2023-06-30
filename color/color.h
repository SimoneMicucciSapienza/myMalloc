#pragma once

#ifndef _NO_COLOR_

//in this way I lose linking problem, just:	e x p a n d   s t u f f
#define	RESET	"\033[0m"
#define	YELLOW	"\033[38;2;255;255;0m"
#define	RED	"\033[38;2;255;0;0m"
#define	BLUE	"\033[38;2;0;0;255m"
#define	GREEN	"\033[38;2;0;255;0m"
#define	ORANGE	"\033[38;2;255;127;0m"
#define	PURPLE	"\033[38;2;63;0;255m"

#define	CYAN	"\033[38;2;0;255;127m"
#define	AZURE	"\033[38;2;0;127;255m"
#define	MAGENTA	"\033[38;2;255;0;255m"
#define	GRAY	"\033[38;2;64;64;64m"
#define	PINK	"\033[38;2;255;127;127m"

#else

#define	RESET	" "
#define	YELLOW	" "
#define	RED	" "
#define	BLUE	" "
#define	GREEN	" "
#define	ORANGE	" "
#define	PURPLE	" "
#define	CYAN	" "
#define	AZURE	" "
#define	MAGENTA	" "
#define	GRAY	" "
#define	PINK	" "

#endif
