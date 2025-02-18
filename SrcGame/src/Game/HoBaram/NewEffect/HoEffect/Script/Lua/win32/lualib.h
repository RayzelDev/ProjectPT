






#ifndef lualib_h
#define lualib_h

#include "lua.h"


#ifndef LUALIB_API
#define LUALIB_API	LUA_API
#endif


#define LUA_COLIBNAME	"coroutine"
LUALIB_API int luaopen_base (lua_State *L);

#define LUA_TABLIBNAME	"table"
LUALIB_API int luaopen_table (lua_State *L);

#define LUA_IOLIBNAME	"io"
#define LUA_OSLIBNAME	"os"
LUALIB_API int luaopen_io (lua_State *L);

#define LUA_STRLIBNAME	"string"
LUALIB_API int luaopen_string (lua_State *L);

#define LUA_MATHLIBNAME	"math"
LUALIB_API int luaopen_math (lua_State *L);

#define LUA_DBLIBNAME	"debug"
LUALIB_API int luaopen_debug (lua_State *L);


LUALIB_API int luaopen_loadlib (lua_State *L);



#ifndef lua_assert
#define lua_assert(c)		
#endif



#define lua_baselibopen	luaopen_base
#define lua_tablibopen	luaopen_table
#define lua_iolibopen	luaopen_io
#define lua_strlibopen	luaopen_string
#define lua_mathlibopen	luaopen_math
#define lua_dblibopen	luaopen_debug

#endif
