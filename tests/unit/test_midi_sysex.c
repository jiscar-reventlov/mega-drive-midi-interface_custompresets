
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include "asserts.h"
#include "midi.h"
#include "midi_psg.h"
#include "psg_chip.h"
#include "synth.h"
#include "unused.h"
#include <cmocka.h>

extern void __real_midi_noteOn(u8 chan, u8 pitch, u8 velocity);
extern void __real_midi_sysex(u8* data, u16 length);

static void test_midi_sysex_sends_all_notes_off(UNUSED void** state)
{
    u8 sysExGeneralMidiResetSequence[] = { 0x7E, 0x7F, 0x09, 0x01 };

    for (u8 chan = MIN_PSG_CHAN; chan <= MAX_PSG_CHAN; chan++) {
        expect_any(__wrap_psg_frequency, channel);
        expect_any(__wrap_psg_frequency, freq);
        expect_any(__wrap_psg_attenuation, channel);
        expect_any(__wrap_psg_attenuation, attenuation);

        __real_midi_noteOn(chan, 60, 127);
    }

    expect_value(__wrap_synth_noteOff, channel, 0);
    expect_value(__wrap_synth_noteOff, channel, 1);
    expect_value(__wrap_synth_noteOff, channel, 2);
    expect_value(__wrap_synth_noteOff, channel, 3);
    expect_value(__wrap_synth_noteOff, channel, 4);
    expect_value(__wrap_synth_noteOff, channel, 5);
    expect_value(__wrap_psg_attenuation, channel, 0);
    expect_value(__wrap_psg_attenuation, attenuation, PSG_ATTENUATION_SILENCE);
    expect_value(__wrap_psg_attenuation, channel, 1);
    expect_value(__wrap_psg_attenuation, attenuation, PSG_ATTENUATION_SILENCE);
    expect_value(__wrap_psg_attenuation, channel, 2);
    expect_value(__wrap_psg_attenuation, attenuation, PSG_ATTENUATION_SILENCE);
    expect_value(__wrap_psg_attenuation, channel, 3);
    expect_value(__wrap_psg_attenuation, attenuation, PSG_ATTENUATION_SILENCE);

    __real_midi_sysex(
        sysExGeneralMidiResetSequence, sizeof(sysExGeneralMidiResetSequence));
}

static void test_midi_sysex_ignores_unknown_sysex(UNUSED void** state)
{
    u8 sysExGeneralMidiResetSequence[] = { 0x12 };

    __real_midi_sysex(
        sysExGeneralMidiResetSequence, sizeof(sysExGeneralMidiResetSequence));
}

static void test_midi_sysex_remaps_midi_channel_to_psg(UNUSED void** state)
{
    const u8 SYSEX_EXTENDED_MANU_ID_SECTION = 0x00;
    const u8 SYSEX_UNUSED_EUROPEAN_SECTION = 0x22;
    const u8 SYSEX_UNUSED_MANU_ID = 0x77;
    const u8 SYSEX_REMAP_COMMAND_ID = 0x00;
    const u8 SYSEX_REMAP_MIDI_CHANNEL = 0x00;
    const u8 SYSEX_REMAP_DESTINATION_FIRST_PSG_CHANNEL = 0x06;

    u8 sequence[] = { SYSEX_EXTENDED_MANU_ID_SECTION,
        SYSEX_UNUSED_EUROPEAN_SECTION, SYSEX_UNUSED_MANU_ID,
        SYSEX_REMAP_COMMAND_ID, SYSEX_REMAP_MIDI_CHANNEL,
        SYSEX_REMAP_DESTINATION_FIRST_PSG_CHANNEL };

    __real_midi_sysex(sequence, sizeof(sequence));

    expect_value(__wrap_psg_frequency, channel, 0);
    expect_any(__wrap_psg_frequency, freq);
    expect_value(__wrap_psg_attenuation, channel, 0);
    expect_any(__wrap_psg_attenuation, attenuation);

    __real_midi_noteOn(0, 60, 127);
}

static void test_midi_sysex_remaps_midi_channel_to_fm(UNUSED void** state)
{
    const u8 SYSEX_EXTENDED_MANU_ID_SECTION = 0x00;
    const u8 SYSEX_UNUSED_EUROPEAN_SECTION = 0x22;
    const u8 SYSEX_UNUSED_MANU_ID = 0x77;
    const u8 SYSEX_REMAP_COMMAND_ID = 0x00;
    const u8 SYSEX_REMAP_MIDI_CHANNEL = 0x00;
    const u8 SYSEX_REMAP_DESTINATION_SECOND_FM_CHANNEL = 0x01;

    u8 sequence[] = { SYSEX_EXTENDED_MANU_ID_SECTION,
        SYSEX_UNUSED_EUROPEAN_SECTION, SYSEX_UNUSED_MANU_ID,
        SYSEX_REMAP_COMMAND_ID, SYSEX_REMAP_MIDI_CHANNEL,
        SYSEX_REMAP_DESTINATION_SECOND_FM_CHANNEL };

    __real_midi_sysex(sequence, sizeof(sequence));

    expect_value(__wrap_synth_pitch, channel, 1);
    expect_any(__wrap_synth_pitch, octave);
    expect_any(__wrap_synth_pitch, freqNumber);
    expect_synth_volume_any();
    expect_value(__wrap_synth_noteOn, channel, 1);

    __real_midi_noteOn(0, 60, 127);
}

static void test_midi_sysex_unassigns_midi_channel(UNUSED void** state)
{
    const u8 SYSEX_EXTENDED_MANU_ID_SECTION = 0x00;
    const u8 SYSEX_UNUSED_EUROPEAN_SECTION = 0x22;
    const u8 SYSEX_UNUSED_MANU_ID = 0x77;
    const u8 SYSEX_REMAP_COMMAND_ID = 0x00;
    const u8 SYSEX_REMAP_MIDI_CHANNEL = 0x00;
    const u8 SYSEX_REMAP_UNASSIGN_CHANNEL = 0x7F;

    u8 sequence[]
        = { SYSEX_EXTENDED_MANU_ID_SECTION, SYSEX_UNUSED_EUROPEAN_SECTION,
              SYSEX_UNUSED_MANU_ID, SYSEX_REMAP_COMMAND_ID,
              SYSEX_REMAP_MIDI_CHANNEL, SYSEX_REMAP_UNASSIGN_CHANNEL };

    __real_midi_sysex(sequence, sizeof(sequence));

    __real_midi_noteOn(0, 60, 127);
}
