#pragma once

#include <cstdint>

/**
 * OSFKeys.h - Universal Key Codes
 *
 * Defines key codes for the OpenSEF framework to avoid direct dependency
 * on libxkbcommon or other window system headers in the core framework.
 * These map generally to standard PC keyboard scancodes/keysyms.
 */

namespace OpenSEF {

namespace Key {
// Modifier masks (matching OSFModifier enum in ShortcutManager)
// Note: These are defined here for reference, actual usage is via OSFModifier
// enum

// Standard keys (Mapping to XKB/Linux keysyms for convenience)
constexpr uint32_t Escape = 0xff1b;
constexpr uint32_t Tab = 0xff09;
constexpr uint32_t Backtab = 0xff08;
constexpr uint32_t Backspace = 0xff08;
constexpr uint32_t Return = 0xff0d;
constexpr uint32_t Enter = 0xff0d;
constexpr uint32_t Insert = 0xff63;
constexpr uint32_t Delete = 0xffff;
constexpr uint32_t Pause = 0xff13;
constexpr uint32_t Print = 0xff61;
constexpr uint32_t SysReq = 0xff15;
constexpr uint32_t Clear = 0xff0b;
constexpr uint32_t Home = 0xff50;
constexpr uint32_t End = 0xff57;
constexpr uint32_t Left = 0xff51;
constexpr uint32_t Up = 0xff52;
constexpr uint32_t Right = 0xff53;
constexpr uint32_t Down = 0xff54;
constexpr uint32_t PageUp = 0xff55;
constexpr uint32_t PageDown = 0xff56;
constexpr uint32_t Shift = 0xffe1;
constexpr uint32_t Control = 0xffe3;
constexpr uint32_t Meta = 0xffe7;
constexpr uint32_t Alt = 0xffe9;
constexpr uint32_t AltGr = 0xffea;
constexpr uint32_t CapsLock = 0xffe5;
constexpr uint32_t NumLock = 0xff7f;
constexpr uint32_t ScrollLock = 0xff14;
constexpr uint32_t F1 = 0xffbe;
constexpr uint32_t F2 = 0xffbf;
constexpr uint32_t F3 = 0xffc0;
constexpr uint32_t F4 = 0xffc1;
constexpr uint32_t F5 = 0xffc2;
constexpr uint32_t F6 = 0xffc3;
constexpr uint32_t F7 = 0xffc4;
constexpr uint32_t F8 = 0xffc5;
constexpr uint32_t F9 = 0xffc6;
constexpr uint32_t F10 = 0xffc7;
constexpr uint32_t F11 = 0xffc8;
constexpr uint32_t F12 = 0xffc9;

constexpr uint32_t Space = 0x0020;
constexpr uint32_t Exclam = 0x0021;
constexpr uint32_t QuoteDbl = 0x0022;
constexpr uint32_t NumberSign = 0x0023;
constexpr uint32_t Dollar = 0x0024;
constexpr uint32_t Percent = 0x0025;
constexpr uint32_t Ampersand = 0x0026;
constexpr uint32_t Apostrophe = 0x0027;
constexpr uint32_t ParenLeft = 0x0028;
constexpr uint32_t ParenRight = 0x0029;
constexpr uint32_t Asterisk = 0x002a;
constexpr uint32_t Plus = 0x002b;
constexpr uint32_t Comma = 0x002c;
constexpr uint32_t Minus = 0x002d;
constexpr uint32_t Period = 0x002e;
constexpr uint32_t Slash = 0x002f;

constexpr uint32_t Key_0 = 0x0030;
constexpr uint32_t Key_1 = 0x0031;
constexpr uint32_t Key_2 = 0x0032;
constexpr uint32_t Key_3 = 0x0033;
constexpr uint32_t Key_4 = 0x0034;
constexpr uint32_t Key_5 = 0x0035;
constexpr uint32_t Key_6 = 0x0036;
constexpr uint32_t Key_7 = 0x0037;
constexpr uint32_t Key_8 = 0x0038;
constexpr uint32_t Key_9 = 0x0039;

constexpr uint32_t Colon = 0x003a;
constexpr uint32_t Semicolon = 0x003b;
constexpr uint32_t Less = 0x003c;
constexpr uint32_t Equal = 0x003d;
constexpr uint32_t Greater = 0x003e;
constexpr uint32_t Question = 0x003f;
constexpr uint32_t At = 0x0040;

constexpr uint32_t A = 0x0061;
constexpr uint32_t B = 0x0062;
constexpr uint32_t C = 0x0063;
constexpr uint32_t D = 0x0064;
constexpr uint32_t E = 0x0065;
constexpr uint32_t F = 0x0066;
constexpr uint32_t G = 0x0067;
constexpr uint32_t H = 0x0068;
constexpr uint32_t I = 0x0069;
constexpr uint32_t J = 0x006a;
constexpr uint32_t K = 0x006b;
constexpr uint32_t L = 0x006c;
constexpr uint32_t M = 0x006d;
constexpr uint32_t N = 0x006e;
constexpr uint32_t O = 0x006f;
constexpr uint32_t P = 0x0070;
constexpr uint32_t Q = 0x0071;
constexpr uint32_t R = 0x0072;
constexpr uint32_t S = 0x0073;
constexpr uint32_t T = 0x0074;
constexpr uint32_t U = 0x0075;
constexpr uint32_t V = 0x0076;
constexpr uint32_t W = 0x0077;
constexpr uint32_t X = 0x0078;
constexpr uint32_t Y = 0x0079;
constexpr uint32_t Z = 0x007a;

constexpr uint32_t BracketLeft = 0x005b;
constexpr uint32_t Backslash = 0x005c;
constexpr uint32_t BracketRight = 0x005d;
constexpr uint32_t AsciiCircum = 0x005e;
constexpr uint32_t Underscore = 0x005f;
constexpr uint32_t QuoteLeft = 0x0060;
constexpr uint32_t BraceLeft = 0x007b;
constexpr uint32_t Bar = 0x007c;
constexpr uint32_t BraceRight = 0x007d;
constexpr uint32_t AsciiTilde = 0x007e;
} // namespace Key

} // namespace OpenSEF
