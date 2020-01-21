#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdint.h>
#include <stdbool.h>

#define LeftButton 0x00000001
#define RightButton 0x00000002
#define UpButton 0x00000004
#define DownButton 0x00000008
#define Fire1Button 0x00000010
#define Fire2Button 0x00000020
#define Fire3Button 0x00000040
#define Fire4Button 0x00000080
#define StartButton 0x10000000
#define SelectButton 0x20000000
#define QuitButton 0x80000000

bool HandleInput();

uint32_t GetButtonState();

static inline bool IsLeftPressed(uint32_t state) { return (state&LeftButton)?true:false; }
static inline bool IsRightPressed(uint32_t state) { return (state&RightButton)?true:false; }
static inline bool IsUpPressed(uint32_t state) { return (state&UpButton)?true:false; }
static inline bool IsDownPressed(uint32_t state) { return (state&DownButton)?true:false; }
static inline bool IsFire1Pressed(uint32_t state) { return (state&Fire1Button)?true:false; }
static inline bool IsFire2Pressed(uint32_t state) { return (state&Fire2Button)?true:false; }
static inline bool IsFire3Pressed(uint32_t state) { return (state&Fire3Button)?true:false; }
static inline bool IsFire4Pressed(uint32_t state) { return (state&Fire4Button)?true:false; }
static inline bool IsStartPressed(uint32_t state) { return (state&StartButton)?true:false; }
static inline bool IsSelectPressed(uint32_t state) { return (state&SelectButton)?true:false; }
static inline bool IsQuitPressed(uint32_t state) { return (state&QuitButton)?true:false; }

#endif
