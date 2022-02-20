/*
MIT License

Copyright (c) 2022 Ryan Starrett

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "lua.h"
#include "lauxlib.h"

#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 512 * sizeof(char)

void sprintf_opt(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int ret = vsprintf(str, format, args);
    str += ret;

    va_end(args);
}

inline void concatenate(char* dest, const char* src) {
    while (*dest) dest++;
    while (*dest++ = *src++);
}

static int print(lua_State *L) {
    char *s = malloc(BUFFER_SIZE + 1); s[0] = '\0';

    for (int i = 1; i <= lua_gettop(L); i++) {
        int type = lua_type(L, i);

        if (i != 1) {
            concatenate(s, "\t");
        }

        // String, number, boolean, and nil get special behavior because we don't need to buffer a memory address.
        if (type == LUA_TSTRING || type == LUA_TNUMBER) {
            concatenate(s, lua_tostring(L, i));

            continue;
        } else 

        if (type == LUA_TBOOLEAN) {
            if (lua_toboolean(L, i) == true) {
                concatenate(s, "true");
            } else {
                concatenate(s, "false");
            }

            continue;
        } else 

        if (type == LUA_TNIL) {
            concatenate(s, "nil");

            continue;
        }

        char memaddr[30];

        // type buffering
        switch (type) {
            case LUA_TTABLE:
                sprintf_opt(memaddr, "table: %p", lua_topointer(L, i));
                break;
            case LUA_TTHREAD:
                sprintf_opt(memaddr, "thread: %p", lua_topointer(L, i));
                break;
            case LUA_TFUNCTION:
                sprintf_opt(memaddr, "function: %p", lua_topointer(L, i));
                break;
            case LUA_TUSERDATA:
                sprintf_opt(memaddr, "userdata: %p", lua_topointer(L, i));
                break;
            case LUA_TLIGHTUSERDATA: {
                sprintf_opt(memaddr, "lightuserdata: %p", lua_topointer(L, i));
                break;
            }
        }

        concatenate(s, (const char*)&memaddr);
    }

    printf("%s\n", s);
    fflush(stdout);
    free(s);

    return 0;
}

static const struct luaL_Reg exports[] = {
    {"print", print},
    {NULL, NULL}
};

extern int __declspec(dllexport) luaopen_fastprint(lua_State *L) {
    luaL_newlib(L, exports);
    return 1;
}
