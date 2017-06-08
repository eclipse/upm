#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern const char* LibraryBaseName();
extern const char* LibraryVersion();
extern const char* LibraryJson();
extern const char* LibraryAbsolutePath();
extern const char* LibraryAbsolutePathForType(void *Type);

#ifdef __cplusplus
}
#endif
