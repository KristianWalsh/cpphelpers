/*
 *  ASCIICodes.hpp
 *
 *    Constant definitions for the control codes defined in ISO-626 (a.k.a. "US-ASCII")
 *
 *
 */

namespace ASCII
{
  const char NUL=0x00;  //null
  const char SOH=0x01;  //start of heading
  const char STX=0x02;  //start of text
  const char ETX=0x03;  //end of text
  const char EOT=0x04;  //end of transmission
  const char ENQ=0x05;  //enquire
  const char ACK=0x06;  //acknowledge
  const char BEL=0x07;  //request attention (sound terminal bell)
  const char BS=0x08;   //backspace
  const char HT=0x09;   //horizontal tab
  const char LF=0x0a;   //linefeed
  const char VT=0x0b;   //vertical tab
  const char FF=0x0c;   //form feed/eject page
  const char CR=0x0d;   //carriage return
  const char SO=0x0e;   //shift out (ISO 2022)
  const char SI=0x0f;   //shift in (ISO 2022)
  const char DLE=0x10;  //data link escape
  const char DC1=0x11;  //device control, #1
  const char DC2=0x12;  //device control, #2
  const char DC3=0x13;  //device control, #3
  const char DC4=0x14;  //device control, #4
  const char NAK=0x15;  //negative acknowlege
  const char SYN=0x16;  //synchronous idle
  const char ETB=0x17;  //end transmission block
  const char CAN=0x18;  //cancel
  const char EM=0x19;   //end of medium
  const char SUB=0x1a;  //substitute (replaces invalid/erroneous character)
  const char ESC=0x1b;  //escape
  const char FS=0x1c;   //file (highest level) separator
  const char GS=0x1d;   //group separator
  const char RS=0x1e;   //record separator
  const char US=0x1f;   //unit (lowest level) separator
  const char DEL=0x7f;  //delete
}
