#ifndef JULIA_BIND_JULIA_BIND_HPP_
#define JULIA_BIND_JULIA_BIND_HPP_

#include <stdexcept>
#include <string>

#include <julia.h>


namespace julia_bind
{
void throw_exception(const std::string& file, int line, const std::string& function, const std::string& type)
{
    std::string msg = "Julia_error: {\n"
        "file: " + file + ",\n"
        + "line: " + std::to_string(line) + ",\n"
        + "function: " + function + ",\n"
        + "type: " + type + "\n"
        + "}";
    throw std::runtime_error(msg);
}

void handle_error(const std::string& file, int line, const std::string& function)
{
    if (!jl_exception_occurred())
        return;

    throw_exception(file, line, function, jl_typeof_str(jl_exception_occurred()));
}

#define HANDLE_ERROR() \
do { \
    handle_error(__FILE__, __LINE__, __FUNCTION__); \
} while(0)

#define EXPECT_NOT_NULLPTR(x) \
do { \
    if (x == nullptr) throw_exception(__FILE__, __LINE__, __FUNCTION__, "nullptr"); \
} while(0)

jl_module_t* load_module(const std::string& file_path, const std::string& name)
{
    std::string cmd = "Base.include(Main, \"" + file_path + "\")";
    (void) jl_eval_string(cmd.c_str());
    HANDLE_ERROR();

    cmd = "using Main." + name;
    (void) jl_eval_string(cmd.c_str());
    HANDLE_ERROR();

    cmd = "Main." + name;
    jl_module_t* module = reinterpret_cast<jl_module_t*>(jl_eval_string(cmd.c_str()));
    HANDLE_ERROR();
    EXPECT_NOT_NULLPTR(module);

    return module;
}

jl_function_t* get_function(jl_module_t* module, const std::string& function_name)
{
    jl_function_t *function = jl_get_function(module, function_name.c_str());
    HANDLE_ERROR();
    EXPECT_NOT_NULLPTR(function);

    return function;
}

} // namespace julia_bind

#endif // JULIA_BIND_JULIA_BIND_HPP_
