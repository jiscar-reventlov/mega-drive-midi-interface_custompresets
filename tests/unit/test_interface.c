#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include "asserts.h"
#include "interface.h"
#include "midi.h"
#include "wraps.h"
#include <cmocka.h>
#include <types.h>

#define STATUS_CC 0xB0
#define STATUS_PITCH_BEND 0xE0
#define STATUS_SYSTEM 0xF0
#define STATUS_CLOCK 0xF8
#define STATUS_STOP 0xFC
#define STATUS_START 0xFA

static void test_interface_tick_passes_note_on_to_midi_processor(void** state)
{
    const u8 expectedData = 60;
    const u8 expectedData2 = 127;

    for (int chan = 0; chan < MAX_MIDI_CHANS; chan++) {
        u8 expectedStatus = 0x90 + chan;

        stub_comm_read_returns_midi_event(
            expectedStatus, expectedData, expectedData2);

        expect_value(__wrap_midi_noteOn, chan, chan);
        expect_value(__wrap_midi_noteOn, pitch, expectedData);
        expect_value(__wrap_midi_noteOn, velocity, expectedData2);

        interface_tick();
    }
}

static void test_interface_tick_passes_note_off_to_midi_processor(void** state)
{
    u8 expectedStatus = 0x80;
    u8 expectedData = 60;
    u8 expectedData2 = 127;

    stub_comm_read_returns_midi_event(
        expectedStatus, expectedData, expectedData2);

    expect_value(__wrap_midi_noteOff, chan, 0);
    expect_value(__wrap_midi_noteOff, pitch, expectedData);

    interface_tick();
}

static void test_interface_does_nothing_for_control_change(void** state)
{
    u8 expectedStatus = 0xA0;
    u8 expectedData = 106;
    u8 expectedData2 = 127;

    stub_comm_read_returns_midi_event(
        expectedStatus, expectedData, expectedData2);

    interface_tick();
    interface_tick();
    interface_tick();
}

static void test_interface_sets_unknown_event_for_unknown_status(void** state)
{
    u8 expectedStatus = 0xD0;

    will_return(__wrap_comm_read, expectedStatus);

    interface_tick();

    assert_int_equal(interface_lastUnknownStatus(), expectedStatus);
}

static void test_interface_sets_unknown_event_for_unknown_system_message(
    void** state)
{
    u8 expectedStatus = 0xF1;

    will_return(__wrap_comm_read, expectedStatus);

    interface_tick();

    assert_int_equal(interface_lastUnknownStatus(), expectedStatus);
}

static void test_interface_sets_CC(void** state)
{
    u8 expectedStatus = STATUS_CC;
    u8 expectedController = CC_VOLUME;
    u8 expectedValue = 0x80;

    stub_comm_read_returns_midi_event(
        expectedStatus, expectedController, expectedValue);

    expect_value(__wrap_midi_cc, chan, 0);
    expect_value(__wrap_midi_cc, controller, expectedController);
    expect_value(__wrap_midi_cc, value, expectedValue);

    interface_tick();

    ControlChange* cc = midi_lastUnknownCC();
    assert_int_not_equal(cc->controller, expectedController);
    assert_int_not_equal(cc->value, expectedValue);
}

static void test_interface_initialises_synth(void** state)
{
    expect_function_call(__wrap_synth_init);
    interface_init();
}

static void test_interface_sets_pitch_bend(void** state)
{
    u8 expectedStatus = STATUS_PITCH_BEND;
    u16 expectedValue = 12000;
    u8 expectedValueLower = expectedValue & 0x007F;
    u8 expectedValueUpper = expectedValue >> 7;

    stub_comm_read_returns_midi_event(
        expectedStatus, expectedValueLower, expectedValueUpper);

    expect_value(__wrap_midi_pitchBend, chan, 0);
    expect_value(__wrap_midi_pitchBend, bend, expectedValue);

    interface_tick();
}

static void test_interface_increments_midi_clock(void** state)
{
    u8 status = STATUS_CLOCK;
    will_return(__wrap_comm_read, status);

    expect_function_call(__wrap_midi_clock);

    interface_tick();
}

static void test_interface_starts_midi(void** state)
{
    u8 status = STATUS_START;
    will_return(__wrap_comm_read, status);

    expect_function_call(__wrap_midi_start);

    interface_tick();
}

static void test_interface_swallows_midi_stop(void** state)
{
    interface_reset();

    u8 status = STATUS_STOP;
    will_return(__wrap_comm_read, status);

    interface_tick();

    assert_int_equal(interface_lastUnknownStatus(), 0);
}
