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

#define THROW_EXCEPTION(type) \
do { \
    throw_exception(__FILE__, __LINE__, __FUNCTION__, type); \
} while(0)

#define EXPECT_NOT_NULLPTR(x) \
do { \
    if (x == nullptr) THROW_EXCEPTION("nullptr"); \
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

namespace parameter_packing
{
jl_value_t* pack(int value)
{
    static_assert(sizeof(int) == sizeof(int32_t));
    jl_value_t* box = jl_box_int32(value);
    HANDLE_ERROR();
    EXPECT_NOT_NULLPTR(box);
    return box;
}

jl_value_t* pack(double value)
{
    // static_assert(sizeof(double) == sizeof(double));
    jl_value_t* box = jl_box_float64(value);
    HANDLE_ERROR();
    EXPECT_NOT_NULLPTR(box);
    return box;
}
} // namespace parameter_packing

namespace details_funcion_call
{ 
jl_value_t* call(jl_function_t* function)
{
    jl_value_t* ret = jl_call0(function);
    HANDLE_ERROR();
    return ret;    
}

template<class T1>
jl_value_t* call(jl_function_t* function, T1 a1)
{
    jl_value_t* ret = jl_call1(function, a1);
    HANDLE_ERROR();
    EXPECT_NOT_NULLPTR(ret);
    return ret;    
}

template<class T1, class T2>
jl_value_t* call(jl_function_t* function, T1 a1, T2 a2)
{
    jl_value_t* ret = jl_call2(function, a1, a2);
    HANDLE_ERROR();
    EXPECT_NOT_NULLPTR(ret);
    return ret;    
}

template<class T1, class T2, class T3>
jl_value_t* call(jl_function_t* function, T1 a1, T2 a2, T3 a3)
{
    jl_value_t* ret = jl_call3(function, a1, a2, a3);
    HANDLE_ERROR();
    EXPECT_NOT_NULLPTR(ret);
    return ret;    
}
} // namespace details_funcion_call

template<typename... Types>
jl_value_t* call_function(jl_function_t* function, Types... args)
{    
    return details_funcion_call::call(function, parameter_packing::pack(args)...);
}

namespace details_unpack
{

} // namespace details_unpack

auto unpack(jl_value_t* packed_value)
{
    if (jl_typeis(packed_value, jl_float64_type)) {
        double value = jl_unbox_float64(packed_value);
        HANDLE_ERROR();
        return value;
    } else {
        THROW_EXCEPTION("unexpected type");
        throw "makes compiler happy";
    }
}

} // namespace julia_bind

#endif // JULIA_BIND_JULIA_BIND_HPP_
