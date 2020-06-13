#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>

#include <julia.h>
JULIA_DEFINE_FAST_TLS()

#include "julia_bind/julia_bind.hpp"

TEST(BuiltInTypes, string_evaluation)
{
    jl_init();

    jl_value_t* ret = jl_eval_string("sqrt(4.0)");

    if (jl_typeis(ret, jl_float64_type)) {
        double value = jl_unbox_float64(ret);
        EXPECT_DOUBLE_EQ(2.0, value);
    } else {
        FAIL() << "Unexpected return type.";
    }

    jl_atexit_hook(0);
}

class BuiltInTypesFixture : public ::testing::Test
{
public:
    void SetUp() override
    {
        jl_init();
    }

    void TearDown() override
    {
        jl_atexit_hook(0);
    }
};

TEST_F(BuiltInTypesFixture, function0_returns_value)
{
    jl_module_t* jm_simple = julia_bind::load_module("simple.ji", "simple");
    jl_function_t *jf_hello = julia_bind::get_function(jm_simple, "hello");

    jl_value_t* ret = julia_bind::call_function(jf_hello);

    if (jl_typeis(ret, jl_string_type)) {
        const char* value = (const char*)jl_string_ptr(ret);
        if (jl_exception_occurred()) {
            std::cout << __LINE__ << " " << jl_typeof_str(jl_exception_occurred()) << std::endl;
            FAIL();
        }
        EXPECT_STREQ("hello", value);
    } else {
        FAIL() << "Unexpected return type.";
    }
}

TEST_F(BuiltInTypesFixture, function1_returns_value)
{
    jl_module_t* jm_simple = julia_bind::load_module("simple.ji", "simple");
    jl_function_t *jf_echo = julia_bind::get_function(jm_simple, "echo");

    jl_value_t* ret = julia_bind::call_function(jf_echo, -9.65);
 
    if (jl_typeis(ret, jl_float64_type)) {
        double value = jl_unbox_float64(ret);
        if (jl_exception_occurred()) {
            std::cout << __LINE__ << " " << jl_typeof_str(jl_exception_occurred()) << std::endl;
            FAIL();
        }
        EXPECT_DOUBLE_EQ(-9.65, value);
    } else {
        FAIL() << "Unexpected return type.";
    }
}

TEST_F(BuiltInTypesFixture, function2_returns_value)
{
    jl_module_t* jm_simple = julia_bind::load_module("simple.ji", "simple");
    jl_function_t *jf_add = julia_bind::get_function(jm_simple, "add");

    jl_value_t* ret = julia_bind::call_function(jf_add, 10.30, 7);
 
    if (jl_typeis(ret, jl_float64_type)) {
        double value = jl_unbox_float64(ret);
        if (jl_exception_occurred()) {
            std::cout << __LINE__ << " " << jl_typeof_str(jl_exception_occurred()) << std::endl;
            FAIL();
        }
        EXPECT_DOUBLE_EQ(17.30, value);
    } else {
        FAIL() << "Unexpected return type.";
    }
}
