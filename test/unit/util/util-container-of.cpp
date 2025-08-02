#include <catch2/catch_test_macros.hpp>
#include <mctp/util/container_of.h>
#include <stdint.h>


typedef struct test_struct_t
{
    uint32_t : 32;
    uint32_t : 32;
    uint32_t member;
    uint32_t : 32;
    uint32_t : 32;
}
test_struct_t;


TEST_CASE("util-container-of")
{
    test_struct_t structure = {};
    uint32_t *member_ref = &structure.member;

    test_struct_t *structure_ref 
        = container_of(
            member_ref, 
            test_struct_t, 
            member
        );

    REQUIRE(&structure == structure_ref);
}