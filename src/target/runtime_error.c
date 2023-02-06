#include <runtime_error.h>

static const char *last_message = "No error";
static int last_parameter;
static const char *last_file;
static int last_line;

void runtime_error(const char *message, int parameter, const char *file, int line)
{
    last_message = message;
    last_parameter = parameter;
    last_file = file;
    last_line = line;
}

const char *runtime_error_get_last_message(void)
{
    return last_message;
}

int runtime_error_get_last_parameter(void)
{
    return last_parameter;
}