// Enjoy your day
// Author: George Potoshin <george.potoshin@gmail.com>
// slog.h: simple c logging

#ifndef __SLOG_H__
#define __SLOG_H__

#include <stdio.h>

#ifndef SLOG_OUT
#define SLOG_OUT stdout
#endif

#define SLOG_BLACK		"\033[30m"
#define SLOG_RED		"\033[31m"
#define SLOG_GREEN		"\033[32m"
#define SLOG_YELLOW		"\033[33m"
#define SLOG_BLUE		"\033[34m"
#define SLOG_MAGENTA	"\033[35m"
#define SLOG_CYAN		"\033[36m"
#define SLOG_WHITE		"\033[37m"

#define SLOG_BR_BLACK	"\033[30;1m"
#define SLOG_BR_RED		"\033[31;1m"
#define SLOG_BR_GREEN	"\033[32;1m"
#define SLOG_BR_YELLOW	"\033[33;1m"
#define SLOG_BR_BLUE	"\033[34;1m"
#define SLOG_BR_MAGENTA	"\033[35;1m"
#define SLOG_BR_CYAN	"\033[36;1m"
#define SLOG_BR_WHITE	"\033[37;1m"

#define SLOG_RESET		"\033[0m"

#ifndef MY_SLOG_MESSAGE_COLOR
#define SLOG_MESSAGE_COLOR SLOG_YELLOW
#else
#define SLOG_MESSAGE_COLOR MY_SLOG_MESSAGE_COLOR
#endif

#ifndef MY_SLOG_ERROR_COLOR
#define SLOG_ERROR_COLOR SLOG_RED
#else
#define SLOG_ERROR_COLOR MY_SLOG_ERROR_COLOR
#endif

#ifndef MY_SLOG_WARNING_COLOR
#define SLOG_WARNING_COLOR SLOG_MAGENTA
#else
#define SLOG_WARNING_COLOR MY_SLOG_WARNING_COLOR
#endif

#ifndef MY_SLOG_INFO_COLOR
#define SLOG_INFO_COLOR SLOG_BLUE
#else
#define SLOG_INFO_COLOR MY_SLOG_INFO_COLOR
#endif

#ifndef NERROR
#define SLOG_ERROR(source, error)						fprintf \
	(SLOG_OUT, SLOG_ERROR_COLOR "error" SLOG_RESET "[%s]: %s\n", source, error)

#define SLOG_FAILED_FILE(source, filename)				fprintf \
	(SLOG_OUT, SLOG_ERROR_COLOR "error" SLOG_RESET "[%s]: can't open " \
	 SLOG_MESSAGE_COLOR "%s" SLOG_RESET "\n", source, filename)
#else
#define SLOG_ERROR(source, error)
#define SLOG_FAILED_FILE(source, filename)
#endif

#if !defined(NLOG) && !defined(NERROR) && !defined(NDEBUG)
#define SLOG_INFO(source, info)							fprintf \
	(SLOG_OUT, SLOG_INFO_COLOR "info" SLOG_RESET "[%s]: %s\n", source, info)

#define SLOG_FN_CALL(source, fn_name)					fprintf \
	(SLOG_OUT, SLOG_INFO_COLOR "function call" SLOG_RESET "[%s]: %s\n", source,\
     fn_name)

#define SLOG_SYNTAX_ERROR(source, line, message, word)	fprintf \
	(SLOG_OUT, SLOG_ERROR_COLOR "syntax error" SLOG_RESET "[%s:%d]: %s " \
     SLOG_MESSAGE_COLOR "%s" SLOG_RESET "\n", source, line, message, word)

#define SLOG_WARNING(source, warning)					fprintf \
	(SLOG_OUT, SLOG_WARNING_COLOR "warning" SLOG_RESET "[%s]: %s\n", source, \
     warning)
#else
#define SLOG_INFO(source, info)
#define SLOG_FN_CALL(source, fn_name)
#define SLOG_SYNTAX_ERROR(source, line, message, word)
#define SLOG_WARNING(source, warning)
#endif

#endif /* __SLOG_H__ */
