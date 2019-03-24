#ifndef TEXPACKR_LOG_H_
#define TEXPACKR_LOG_H_

/// ***be careful not to use this macro in statement without { } wrapping around the code
/// as we don't to add empty while loop to project the code section
#ifdef TEXPACKR_DEBUG
#define TEXPACKR_LOG(f, ...) fprintf(stdout, f, ##__VA_ARGS__);
#define TEXPACKR_ELOG(f, ...) fprintf(stderr, f, ##__VA_ARGS__);
#else
#define TEXPACKR_LOG(f, ...)
#define TEXPACKR_ELOG(f, ...)
#endif

#endif
