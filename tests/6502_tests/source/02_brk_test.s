; This tests to see if BRK can be used as a debugging feature
.setcpu "65C02"

.segment "HEADER"
.byte "NES", $1A
.byte $02


.segment "PROGRAM"
RESET:
  LDX #10
Loop:
  BRK
  DEX
  BNE Loop

  BRK #$EA

Interrupt:
  RTI

.segment "RESETVEC"
  .addr $0FAA       ; NMI Vector
  .addr RESET       ; RESET Vector
  .addr Interrupt   ; IRQ Vector
