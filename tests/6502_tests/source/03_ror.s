; This tests to see if instructions that work on accumulator or others work correctly
.setcpu "65C02"

.segment "HEADER"
.byte "NES", $1A
.byte $02


.segment "PROGRAM"
RESET:
  LDA #1
  STA $FF
  LDA #3
  BRK

  ROL A
  ROL A

  BRK

  ROL $FF
  ROL $FF
  LDX $FF

Done:
  BRK
  JMP Done

Interrupt:
  RTI

.segment "RESETVEC"
  .addr $0FAA       ; NMI Vector
  .addr RESET       ; RESET Vector
  .addr Interrupt   ; IRQ Vector
