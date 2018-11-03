#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>
#include <comm.h>
#include <interface.h>
#include <midi.h>

void __wrap_synth_noteOn(u8 channel)
{
    check_expected(channel);
}

u8 __wrap_comm_read(void)
{
    return mock_type(u8);
}

void __wrap_midi_process(Message* message)
{
    check_expected(message);
}

static void midi_triggers_synth_note_on(void** state)
{
    Message noteOn = { 0b10010000, 0x40, 127 };

    expect_value(__wrap_synth_noteOn, channel, 0);

    __real_midi_process(&noteOn);
}

static void interface_tick_passes_message_to_midi_processor(void** state)
{
    u8 expectedStatus = 1;
    u8 expectedData = 2;
    u8 expectedData2 = 3;
    Message expectedMessage = { expectedStatus, expectedData, expectedData2 };

    will_return(__wrap_comm_read, expectedStatus);
    will_return(__wrap_comm_read, expectedData);
    will_return(__wrap_comm_read, expectedData2);

    expect_memory(__wrap_midi_process, message, &expectedMessage, sizeof(Message));

    interface_tick();
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(interface_tick_passes_message_to_midi_processor),
        cmocka_unit_test(midi_triggers_synth_note_on)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
