#ifndef GRDEFS_H
#define GRDEFS_H

#define CONC(a,b) a##b

#include <stdio.h>

#ifndef GR_LOG_OUT
#define GR_LOG_OUT stdout
#endif


#ifndef NERROR
#define GR_LOG_ERROR(source, error) fprintf (GR_LOG_OUT, "\033[31m%s\033[0m: %s!\n", source, error)
#else
#define GR_LOG_ERROR(source, error)
#endif

#if !defined(NLOG) && !defined(NERROR)
#define GR_LOG_FILE(source, file_name)				fprintf \
	(GR_LOG_OUT, "\033[31m%s\033[0m: can't open %s!\n", source, filename)

#define GR_LOG_SYNTAX_ERROR(source, line, message, word)	fprintf \
	(GR_LOG_OUT, "\033[31m%s\033[0m:%d: %s %s!\n", source, line, message, word)

#define GR_LOG_WARNING(source, warning)				fprintf \
	(GR_LOG_OUT, "\033[35m%s\033[0m: %s!\n", source, warning)
#else
#define GR_LOG_FILE(source, filename)
#define GR_LOG_SYNTAX_ERROR(source, line, message, word)
#define GR_LOG_WARNING(source, warning)
#endif

#endif /* GRDEFS_H */
