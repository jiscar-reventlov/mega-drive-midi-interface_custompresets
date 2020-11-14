#include <setjmp.h>
#include <stdint.h>
#include <types.h>
#include <vstring.h>
#include <stddef.h>
#include <cmocka.h>
#include "asserts.h"
#include "unused.h"
#include "wraps.h"
#include "buffer.h"

static int test_buffer_setup(UNUSED void** state)
{
    return 0;
}

static void test_buffer_reads_and_writes_single_byte(UNUSED void** state)
{
    const u8 expectedData = 0x01;

    buffer_write(expectedData);

    assert_int_equal(buffer_read(), expectedData);
}

static void test_buffer_reads_and_writes_circularly_over_capacity(
    UNUSED void** state)
{
    const u16 chunkSize = BUFFER_SIZE / 2;

    for (u16 i = 0; i < chunkSize; i++) { buffer_write(0x00); }
    for (u16 i = 0; i < chunkSize; i++) { buffer_write(0x01); }
    for (u16 i = 0; i < chunkSize; i++) {
        assert_int_equal(buffer_read(), 0x00);
    };
    for (u16 i = 0; i < chunkSize; i++) { buffer_write(0x02); }
    for (u16 i = 0; i < chunkSize; i++) {
        assert_int_equal(buffer_read(), 0x01);
    };
    for (u16 i = 0; i < chunkSize; i++) { buffer_write(0x03); }
    for (u16 i = 0; i < chunkSize; i++) {
        assert_int_equal(buffer_read(), 0x02);
    };
}
