#include <midi.h>
#include <sprite_eng.h>
#include <types.h>

#include "synth.h"

void __wrap_synth_init(const Channel* defaultPreset);
void __wrap_synth_enableLfo(u8 enable);
void __wrap_synth_globalLfoFrequency(u8 freq);
void __wrap_synth_noteOn(u8 channel);
void __wrap_synth_noteOff(u8 channel);
void __wrap_synth_pitch(u8 channel, u8 octave, u16 freqNumber);
void __wrap_synth_totalLevel(u8 channel, u8 totalLevel);
void __wrap_synth_stereo(u8 channel, u8 mode);
void __wrap_synth_algorithm(u8 channel, u8 algorithm);
void __wrap_synth_feedback(u8 channel, u8 feedback);
void __wrap_synth_ams(u8 channel, u8 ams);
void __wrap_synth_fms(u8 channel, u8 fms);
void __wrap_synth_operatorTotalLevel(u8 channel, u8 op, u8 totalLevel);
void __wrap_synth_operatorMultiple(u8 channel, u8 op, u8 multiple);
void __wrap_synth_operatorDetune(u8 channel, u8 op, u8 detune);
void __wrap_synth_operatorRateScaling(u8 channel, u8 op, u8 rateScaling);
void __wrap_synth_operatorAttackRate(u8 channel, u8 op, u8 attackRate);
void __wrap_synth_operatorFirstDecayRate(u8 channel, u8 op, u8 firstDecayRate);
void __wrap_synth_operatorSecondDecayRate(
    u8 channel, u8 op, u8 secondDecayRate);
void __wrap_synth_operatorSecondaryAmplitude(
    u8 channel, u8 op, u8 secondaryAmplitude);
void __wrap_synth_operatorAmplitudeModulation(
    u8 channel, u8 op, u8 amplitudeModulation);
void __wrap_synth_operatorReleaseRate(u8 channel, u8 op, u8 releaseRate);
void __wrap_synth_operatorSsgEg(u8 channel, u8 op, u8 ssgEg);
void __wrap_synth_preset(u8 channel, const Channel* preset);
void __wrap_synth_volume(u8 channel, u8 volume);
u8 __wrap_comm_read(void);
void __wrap_comm_write(u8 data);
void __wrap_fm_writeReg(u16 part, u8 reg, u8 data);
void __wrap_psg_noteOn(u8 channel, u16 freq);
void __wrap_psg_noteOff(u8 channel);
void __wrap_psg_attenuation(u8 channel, u8 attenuation);
void __wrap_psg_frequency(u8 channel, u16 freq);
void __wrap_midi_noteOff(u8 chan, u8 pitch);
void __wrap_midi_noteOn(u8 chan, u8 pitch, u8 velocity);
void __wrap_midi_channelVolume(u8 chan, u8 volume);
void __wrap_midi_pitchBend(u8 chan, u16 bend);
void __wrap_midi_setPolyphonic(bool state);
bool __wrap_midi_getPolyphonic(void);
void __wrap_midi_cc(u8 chan, u8 controller, u8 value);
void __wrap_midi_clock(void);
void __wrap_midi_start(void);
void __wrap_midi_position(u16 beat);
void __wrap_midi_program(u8 chan, u8 program);
Timing* __wrap_midi_timing(void);
void __wrap_midi_sysex(u8* data, u16 length);
void __wrap_midi_mappings(u8* mappingDest);
void __wrap_YM2612_writeReg(const u16 part, const u8 reg, const u8 data);
void __wrap_VDP_drawText(const char* str, u16 x, u16 y);
void __wrap_SYS_setVIntCallback(_voidCallback* CB);
void __wrap_VDP_setTextPalette(u16 palette);
void __wrap_VDP_clearText(u16 x, u16 y, u16 w);
void __wrap_PSG_setEnvelope(u8 channel, u8 value);
void __wrap_PSG_setFrequency(u8 channel, u16 value);
Sprite* __wrap_SPR_addSprite(
    const SpriteDefinition* spriteDef, s16 x, s16 y, u16 attribut);
void __wrap_SPR_update();
void __wrap_SYS_disableInts();
void __wrap_SYS_enableInts();
void __wrap_SPR_init(u16 maxSprite, u16 vramSize, u16 unpackBufferSize);
void __wrap_VDP_setPaletteColors(u16 index, const u16* values, u16 count);
void __wrap_VDP_setPaletteColor(u16 index, u16 value);
void __wrap_VDP_setBackgroundColor(u8 index);
void __wrap_SPR_setAnim(Sprite* sprite, s16 anim);
void __wrap_SPR_setFrame(Sprite* sprite, s16 frame);
void __wrap_SPR_setAnimAndFrame(Sprite* sprite, s16 anim, s16 frame);
void __wrap_VDP_setReg(u16 reg, u8 value);
void __wrap_SYS_setExtIntCallback(_voidCallback* CB);
void __wrap_SYS_setInterruptMaskLevel(u16 value);
void __wrap_comm_serial_init(void);
u8 __wrap_comm_serial_readReady(void);
u8 __wrap_comm_serial_read(void);
u8 __wrap_comm_serial_writeReady(void);
void __wrap_comm_serial_write(u8 data);
void __wrap_comm_everdrive_init(void);
u8 __wrap_comm_everdrive_readReady(void);
u8 __wrap_comm_everdrive_read(void);
u8 __wrap_comm_everdrive_writeReady(void);
void __wrap_comm_everdrive_write(u8 data);
