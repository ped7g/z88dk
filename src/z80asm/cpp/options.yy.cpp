// cpp/options.yy.cpp generated by reflex 2.0.1 from cpp/options.l

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define REFLEX_OPTION_dotall              true
#define REFLEX_OPTION_fast                true
#define REFLEX_OPTION_freespace           true
#define REFLEX_OPTION_header_file         "cpp/options.yy.h"
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               OptionsLexer
#define REFLEX_OPTION_noline              true
#define REFLEX_OPTION_outfile             "cpp/options.yy.cpp"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top user code                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


	// silence warnings from RE-flex
	#ifdef _MSC_VER
	#pragma warning(disable:4102)
	#pragma warning(disable:4800)
	#else
	#ifdef __GNUC__
	//#pragma GCC   diagnostic ignored "-Wignored-qualifiers"
	#else
	#ifdef __clang__
	//#pragma clang diagnostic ignored "-Wignored-qualifiers"
	#endif
	#endif
	#endif

	#include "Arch.h"
	#include "Cpu.h"
	#include "Options.h"


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class OptionsLexer : public reflex::AbstractLexer<reflex::Matcher> {
 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  OptionsLexer(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {
  }
  static const int INITIAL = 0;
  virtual int lex();
  int lex(
      const reflex::Input& input,
      std::ostream        *os = NULL)
  {
    in(input);
    if (os)
      out(*os);
    return lex();
  }
};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 2: rules                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

extern void reflex_code_INITIAL(reflex::Matcher&);

int OptionsLexer::lex()
{
  static const reflex::Pattern PATTERN_INITIAL(reflex_code_INITIAL);
  if (!has_matcher())
  {
    matcher(new Matcher(PATTERN_INITIAL, stdinit(), this));
  }
  while (true)
  {
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
              return int();
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 32: -v\z|--verbose\z :
{ theOptions.verbose = true; return true; }

            break;
          case 2: // rule at line 36: -\?\z|-h\z|--help\z :
{ ExitManual(); }

            break;
          case 3: // rule at line 39: -m=?z80\z|--cpu=?z80\z :
{ theCpu.Init(Cpu::Type::Z80); return true; }

            break;
          case 4: // rule at line 42: -m=?z80n\z|--cpu=?z80n\z :
{ theCpu.Init(Cpu::Type::Z80N); return true; }

            break;
          case 5: // rule at line 45: -m=?z180\z|--cpu=?z180\z :
{ theCpu.Init(Cpu::Type::Z180); return true; }

            break;
          case 6: // rule at line 48: -m=?r2k\z|--cpu=?r2k\z :
{ theCpu.Init(Cpu::Type::R2K); return true; }

            break;
          case 7: // rule at line 51: -m=?r3k\z|--cpu=?r3k\z :
{ theCpu.Init(Cpu::Type::R3K); return true; }

            break;
          case 8: // rule at line 54: -m=?8080\z|--cpu=?8080\z :
{ theCpu.Init(Cpu::Type::I8080); return true; }

            break;
          case 9: // rule at line 57: -m=?8085\z|--cpu=?8085\z :
{ theCpu.Init(Cpu::Type::I8085); return true; }

            break;
          case 10: // rule at line 60: -m=?gbz80\z|--cpu=?gbz80\z :
{ theCpu.Init(Cpu::Type::GBZ80); return true; }

            break;
          case 11: // rule at line 63: -m=?ti83\z|--cpu=?ti83\z :
{	theCpu.Init(Cpu::Type::Z80);
					theArch.Init(Arch::Type::TI83);
					return true; }

            break;
          case 12: // rule at line 68: -m=?ti83plus\z|--cpu=?ti83plus\z :
{
					theCpu.Init(Cpu::Type::Z80);
					theArch.Init(Arch::Type::TI83PLUS);
					return true; }

            break;
          case 13: // rule at line 74: -IXIY\z|--IXIY\z :
{ theOptions.swapIxIy = true; return true; }

            break;
          case 14: // rule at line 76: --opt=speed\z :
{ theOptions.optimizeSpeed = true; return true; }

            break;
          case 15: // rule at line 78: --debug\z :
{	theOptions.debugInfo = true;
					theOptions.doMapFile = true;
					return true; }

            break;
          case 16: // rule at line 82: -m\z :
          case 17: // rule at line 83: --map\z :
{ theOptions.doMapFile = true; return true; }

            break;
          case 18: // rule at line 92: [\x00-\xff] :
{ return false; }

            break;
        }
  }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  TABLES                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

#if defined(OS_WIN)
#pragma warning(disable:4101 4102)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-label"
#elif defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-label"
#endif

void reflex_code_INITIAL(reflex::Matcher& m)
{
  int c0 = 0, c1 = 0;
  m.FSM_INIT(c1);

S0:
  m.FSM_FIND();
  c1 = m.FSM_CHAR();
  if (c1 == '-') goto S2;
  if (0 <= c1) goto S10;
  return m.FSM_HALT(c1);

S2:
  m.FSM_TAKE(18);
  c1 = m.FSM_CHAR();
  if (c1 == 'v') goto S12;
  if (c1 == 'm') goto S26;
  if (c1 == 'h') goto S24;
  if (c1 == 'I') goto S34;
  if (c1 == '?') goto S22;
  if (c1 == '-') goto S14;
  return m.FSM_HALT(c1);

S10:
  m.FSM_TAKE(18);
  return m.FSM_HALT();

S12:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(1, c1);
  }
  return m.FSM_HALT(c1);

S14:
  c1 = m.FSM_CHAR();
  if (c1 == 'v') goto S38;
  if (c1 == 'o') goto S46;
  if (c1 == 'm') goto S50;
  if (c1 == 'h') goto S40;
  if (c1 == 'd') goto S48;
  if (c1 == 'c') goto S42;
  if (c1 == 'I') goto S44;
  return m.FSM_HALT(c1);

S22:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(2, c1);
  }
  return m.FSM_HALT(c1);

S24:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(2, c1);
  }
  return m.FSM_HALT(c1);

S26:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(16, c1);
  }
  if (c1 == 'z') goto S54;
  if (c1 == 't') goto S64;
  if (c1 == 'r') goto S57;
  if (c1 == 'g') goto S62;
  if (c1 == '=') goto S68;
  if (c1 == '8') goto S60;
  return m.FSM_HALT(c1);

S34:
  c1 = m.FSM_CHAR();
  if (c1 == 'X') goto S74;
  return m.FSM_HALT(c1);

S36:
  m.FSM_TAKE(1);
  return m.FSM_HALT();

S38:
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S76;
  return m.FSM_HALT(c1);

S40:
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S78;
  return m.FSM_HALT(c1);

S42:
  c1 = m.FSM_CHAR();
  if (c1 == 'p') goto S80;
  return m.FSM_HALT(c1);

S44:
  c1 = m.FSM_CHAR();
  if (c1 == 'X') goto S82;
  return m.FSM_HALT(c1);

S46:
  c1 = m.FSM_CHAR();
  if (c1 == 'p') goto S84;
  return m.FSM_HALT(c1);

S48:
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S86;
  return m.FSM_HALT(c1);

S50:
  c1 = m.FSM_CHAR();
  if (c1 == 'a') goto S88;
  return m.FSM_HALT(c1);

S52:
  m.FSM_TAKE(2);
  return m.FSM_HALT();

S54:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S90;
  if (c1 == '1') goto S92;
  return m.FSM_HALT(c1);

S57:
  c1 = m.FSM_CHAR();
  if (c1 == '3') goto S96;
  if (c1 == '2') goto S94;
  return m.FSM_HALT(c1);

S60:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S98;
  return m.FSM_HALT(c1);

S62:
  c1 = m.FSM_CHAR();
  if (c1 == 'b') goto S100;
  return m.FSM_HALT(c1);

S64:
  c1 = m.FSM_CHAR();
  if (c1 == 'i') goto S102;
  return m.FSM_HALT(c1);

S66:
  m.FSM_TAKE(16);
  return m.FSM_HALT();

S68:
  c1 = m.FSM_CHAR();
  if (c1 == 'z') goto S54;
  if (c1 == 't') goto S64;
  if (c1 == 'r') goto S57;
  if (c1 == 'g') goto S62;
  if (c1 == '8') goto S60;
  return m.FSM_HALT(c1);

S74:
  c1 = m.FSM_CHAR();
  if (c1 == 'I') goto S104;
  return m.FSM_HALT(c1);

S76:
  c1 = m.FSM_CHAR();
  if (c1 == 'r') goto S106;
  return m.FSM_HALT(c1);

S78:
  c1 = m.FSM_CHAR();
  if (c1 == 'l') goto S108;
  return m.FSM_HALT(c1);

S80:
  c1 = m.FSM_CHAR();
  if (c1 == 'u') goto S110;
  return m.FSM_HALT(c1);

S82:
  c1 = m.FSM_CHAR();
  if (c1 == 'I') goto S117;
  return m.FSM_HALT(c1);

S84:
  c1 = m.FSM_CHAR();
  if (c1 == 't') goto S119;
  return m.FSM_HALT(c1);

S86:
  c1 = m.FSM_CHAR();
  if (c1 == 'b') goto S121;
  return m.FSM_HALT(c1);

S88:
  c1 = m.FSM_CHAR();
  if (c1 == 'p') goto S123;
  return m.FSM_HALT(c1);

S90:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S125;
  return m.FSM_HALT(c1);

S92:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S128;
  return m.FSM_HALT(c1);

S94:
  c1 = m.FSM_CHAR();
  if (c1 == 'k') goto S130;
  return m.FSM_HALT(c1);

S96:
  c1 = m.FSM_CHAR();
  if (c1 == 'k') goto S132;
  return m.FSM_HALT(c1);

S98:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S134;
  return m.FSM_HALT(c1);

S100:
  c1 = m.FSM_CHAR();
  if (c1 == 'z') goto S137;
  return m.FSM_HALT(c1);

S102:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S139;
  return m.FSM_HALT(c1);

S104:
  c1 = m.FSM_CHAR();
  if (c1 == 'Y') goto S141;
  return m.FSM_HALT(c1);

S106:
  c1 = m.FSM_CHAR();
  if (c1 == 'b') goto S143;
  return m.FSM_HALT(c1);

S108:
  c1 = m.FSM_CHAR();
  if (c1 == 'p') goto S145;
  return m.FSM_HALT(c1);

S110:
  c1 = m.FSM_CHAR();
  if (c1 == 'z') goto S147;
  if (c1 == 't') goto S157;
  if (c1 == 'r') goto S150;
  if (c1 == 'g') goto S155;
  if (c1 == '=') goto S159;
  if (c1 == '8') goto S153;
  return m.FSM_HALT(c1);

S117:
  c1 = m.FSM_CHAR();
  if (c1 == 'Y') goto S165;
  return m.FSM_HALT(c1);

S119:
  c1 = m.FSM_CHAR();
  if (c1 == '=') goto S167;
  return m.FSM_HALT(c1);

S121:
  c1 = m.FSM_CHAR();
  if (c1 == 'u') goto S169;
  return m.FSM_HALT(c1);

S123:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(17, c1);
  }
  return m.FSM_HALT(c1);

S125:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(3, c1);
  }
  if (c1 == 'n') goto S175;
  return m.FSM_HALT(c1);

S128:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S177;
  return m.FSM_HALT(c1);

S130:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(6, c1);
  }
  return m.FSM_HALT(c1);

S132:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(7, c1);
  }
  return m.FSM_HALT(c1);

S134:
  c1 = m.FSM_CHAR();
  if (c1 == '5') goto S185;
  if (c1 == '0') goto S183;
  return m.FSM_HALT(c1);

S137:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S187;
  return m.FSM_HALT(c1);

S139:
  c1 = m.FSM_CHAR();
  if (c1 == '3') goto S189;
  return m.FSM_HALT(c1);

S141:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(13, c1);
  }
  return m.FSM_HALT(c1);

S143:
  c1 = m.FSM_CHAR();
  if (c1 == 'o') goto S194;
  return m.FSM_HALT(c1);

S145:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(2, c1);
  }
  return m.FSM_HALT(c1);

S147:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S196;
  if (c1 == '1') goto S198;
  return m.FSM_HALT(c1);

S150:
  c1 = m.FSM_CHAR();
  if (c1 == '3') goto S202;
  if (c1 == '2') goto S200;
  return m.FSM_HALT(c1);

S153:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S204;
  return m.FSM_HALT(c1);

S155:
  c1 = m.FSM_CHAR();
  if (c1 == 'b') goto S206;
  return m.FSM_HALT(c1);

S157:
  c1 = m.FSM_CHAR();
  if (c1 == 'i') goto S208;
  return m.FSM_HALT(c1);

S159:
  c1 = m.FSM_CHAR();
  if (c1 == 'z') goto S147;
  if (c1 == 't') goto S157;
  if (c1 == 'r') goto S150;
  if (c1 == 'g') goto S155;
  if (c1 == '8') goto S153;
  return m.FSM_HALT(c1);

S165:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(13, c1);
  }
  return m.FSM_HALT(c1);

S167:
  c1 = m.FSM_CHAR();
  if (c1 == 's') goto S210;
  return m.FSM_HALT(c1);

S169:
  c1 = m.FSM_CHAR();
  if (c1 == 'g') goto S212;
  return m.FSM_HALT(c1);

S171:
  m.FSM_TAKE(17);
  return m.FSM_HALT();

S173:
  m.FSM_TAKE(3);
  return m.FSM_HALT();

S175:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(4, c1);
  }
  return m.FSM_HALT(c1);

S177:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(5, c1);
  }
  return m.FSM_HALT(c1);

S179:
  m.FSM_TAKE(6);
  return m.FSM_HALT();

S181:
  m.FSM_TAKE(7);
  return m.FSM_HALT();

S183:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(8, c1);
  }
  return m.FSM_HALT(c1);

S185:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(9, c1);
  }
  return m.FSM_HALT(c1);

S187:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S222;
  return m.FSM_HALT(c1);

S189:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(11, c1);
  }
  if (c1 == 'p') goto S226;
  return m.FSM_HALT(c1);

S192:
  m.FSM_TAKE(13);
  return m.FSM_HALT();

S194:
  c1 = m.FSM_CHAR();
  if (c1 == 's') goto S228;
  return m.FSM_HALT(c1);

S196:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S230;
  return m.FSM_HALT(c1);

S198:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S233;
  return m.FSM_HALT(c1);

S200:
  c1 = m.FSM_CHAR();
  if (c1 == 'k') goto S235;
  return m.FSM_HALT(c1);

S202:
  c1 = m.FSM_CHAR();
  if (c1 == 'k') goto S237;
  return m.FSM_HALT(c1);

S204:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S239;
  return m.FSM_HALT(c1);

S206:
  c1 = m.FSM_CHAR();
  if (c1 == 'z') goto S242;
  return m.FSM_HALT(c1);

S208:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S244;
  return m.FSM_HALT(c1);

S210:
  c1 = m.FSM_CHAR();
  if (c1 == 'p') goto S246;
  return m.FSM_HALT(c1);

S212:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(15, c1);
  }
  return m.FSM_HALT(c1);

S214:
  m.FSM_TAKE(4);
  return m.FSM_HALT();

S216:
  m.FSM_TAKE(5);
  return m.FSM_HALT();

S218:
  m.FSM_TAKE(8);
  return m.FSM_HALT();

S220:
  m.FSM_TAKE(9);
  return m.FSM_HALT();

S222:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(10, c1);
  }
  return m.FSM_HALT(c1);

S224:
  m.FSM_TAKE(11);
  return m.FSM_HALT();

S226:
  c1 = m.FSM_CHAR();
  if (c1 == 'l') goto S252;
  return m.FSM_HALT(c1);

S228:
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S254;
  return m.FSM_HALT(c1);

S230:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(3, c1);
  }
  if (c1 == 'n') goto S256;
  return m.FSM_HALT(c1);

S233:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S258;
  return m.FSM_HALT(c1);

S235:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(6, c1);
  }
  return m.FSM_HALT(c1);

S237:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(7, c1);
  }
  return m.FSM_HALT(c1);

S239:
  c1 = m.FSM_CHAR();
  if (c1 == '5') goto S262;
  if (c1 == '0') goto S260;
  return m.FSM_HALT(c1);

S242:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S264;
  return m.FSM_HALT(c1);

S244:
  c1 = m.FSM_CHAR();
  if (c1 == '3') goto S266;
  return m.FSM_HALT(c1);

S246:
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S269;
  return m.FSM_HALT(c1);

S248:
  m.FSM_TAKE(15);
  return m.FSM_HALT();

S250:
  m.FSM_TAKE(10);
  return m.FSM_HALT();

S252:
  c1 = m.FSM_CHAR();
  if (c1 == 'u') goto S271;
  return m.FSM_HALT(c1);

S254:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(1, c1);
  }
  return m.FSM_HALT(c1);

S256:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(4, c1);
  }
  return m.FSM_HALT(c1);

S258:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(5, c1);
  }
  return m.FSM_HALT(c1);

S260:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(8, c1);
  }
  return m.FSM_HALT(c1);

S262:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(9, c1);
  }
  return m.FSM_HALT(c1);

S264:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S273;
  return m.FSM_HALT(c1);

S266:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(11, c1);
  }
  if (c1 == 'p') goto S275;
  return m.FSM_HALT(c1);

S269:
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S277;
  return m.FSM_HALT(c1);

S271:
  c1 = m.FSM_CHAR();
  if (c1 == 's') goto S279;
  return m.FSM_HALT(c1);

S273:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(10, c1);
  }
  return m.FSM_HALT(c1);

S275:
  c1 = m.FSM_CHAR();
  if (c1 == 'l') goto S281;
  return m.FSM_HALT(c1);

S277:
  c1 = m.FSM_CHAR();
  if (c1 == 'd') goto S283;
  return m.FSM_HALT(c1);

S279:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(12, c1);
  }
  return m.FSM_HALT(c1);

S281:
  c1 = m.FSM_CHAR();
  if (c1 == 'u') goto S287;
  return m.FSM_HALT(c1);

S283:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(14, c1);
  }
  return m.FSM_HALT(c1);

S285:
  m.FSM_TAKE(12);
  return m.FSM_HALT();

S287:
  c1 = m.FSM_CHAR();
  if (c1 == 's') goto S291;
  return m.FSM_HALT(c1);

S289:
  m.FSM_TAKE(14);
  return m.FSM_HALT();

S291:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(12, c1);
  }
  return m.FSM_HALT(c1);
}

