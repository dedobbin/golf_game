#include "utils.hpp"
#include <string.h>
#include <emscripten.h>

EM_JS(void, js_print, (std::string str), {
  console.log(UTF8ToString(str));
});

void debug_print(std::string str)
{
    js_print(str);
}

void strip_ext(char *fname)
{
    char *end = fname + strlen(fname);

    while (end > fname && *end != '.' && *end != '\\' && *end != '/') {
        --end;
    }
    if ((end > fname && *end == '.') &&
        (*(end - 1) != '\\' && *(end - 1) != '/')) {
        *end = '\0';
    }  
}

bool ends_with(char* haystack, char* needle)
{
	return (strlen(haystack) > strlen(needle) && !strcmp(haystack + strlen(haystack) - strlen(needle), needle));
}

