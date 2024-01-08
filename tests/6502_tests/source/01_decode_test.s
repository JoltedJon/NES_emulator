; This is a simple test to test the timing
.setcpu "65C02"

.segment "HEADER"
.byte "NES", $1A
.byte $02


.segment "PROGRAM"
RESET:
  ADC #10         ; Immediate
  ADC $AA         ; Zero Page
  ADC $10, X      ; Zero Page X
  ADC $3030       ; Absolute
  ADC $3031, X    ; Absolute, X
  ADC $3032, Y    ; Absolute, Y
  ADC ($40, X)    ; (Indirect, X)
  ADC ($41), Y    ; (Indirect), Y

  AND #10         ; Immediate
  AND $AA         ; Zero Page
  AND $10, X      ; Zero Page X
  AND $3030       ; Absolute
  AND $3031, X    ; Absolute, X
  AND $3032, Y    ; Absolute, Y
  AND ($40, X)    ; (Indirect, X)
  AND ($41), Y    ; (Indirect), Y

  ASL A           ; Accumulator
  ASL $AA         ; Zero Page
  ASL $10, X      ; Zero Page X
  ASL $3030       ; Absolute
  ASL $3031, X    ; Absolute, X

  BCC $01         ; Relative

  BCS $01         ; Relative

  BEQ $01         ; Relative

  CMP #10         ; Immediate
  CMP $AA         ; Zero Page
  CMP $10, X      ; Zero Page X
  CMP $3030       ; Absolute
  CMP $3031, X    ; Absolute, X
  CMP $3032, Y    ; Absolute, Y
  CMP ($40, X)    ; (Indirect, X)
  CMP ($41), Y    ; (Indirect), Y

  EOR #10         ; Immediate
  EOR $AA         ; Zero Page
  EOR $10, X      ; Zero Page X
  EOR $3030       ; Absolute
  EOR $3031, X    ; Absolute, X
  EOR $3032, Y    ; Absolute, Y
  EOR ($40, X)    ; (Indirect, X)
  EOR ($41), Y    ; (Indirect), Y

  LDA #10         ; Immediate
  LDA $AA         ; Zero Page
  LDA $10, X      ; Zero Page X
  LDA $3030       ; Absolute
  LDA $3031, X    ; Absolute, X
  LDA $3032, Y    ; Absolute, Y
  LDA ($40, X)    ; (Indirect, X)
  LDA ($41), Y    ; (Indirect), Y

  ORA #10         ; Immediate
  ORA $AA         ; Zero Page
  ORA $10, X      ; Zero Page X
  ORA $3030       ; Absolute
  ORA $3031, X    ; Absolute, X
  ORA $3032, Y    ; Absolute, Y
  ORA ($40, X)    ; (Indirect, X)
  ORA ($41), Y    ; (Indirect), Y

  SBC #10         ; Immediate
  SBC $AA         ; Zero Page
  SBC $10, X      ; Zero Page X
  SBC $3030       ; Absolute
  SBC $3031, X    ; Absolute, X
  SBC $3032, Y    ; Absolute, Y
  SBC ($40, X)    ; (Indirect, X)
  SBC ($41), Y    ; (Indirect), Y

  STA #10         ; Immediate
  STA $AA         ; Zero Page
  STA $10, X      ; Zero Page X
  STA $3030       ; Absolute
  STA $3031, X    ; Absolute, X
  STA $3032, Y    ; Absolute, Y
  STA ($40, X)    ; (Indirect, X)
  STA ($41), Y    ; (Indirect), Y

  DEC $AA         ; Zero Page
  DEC $10, X      ; Zero Page X
  DEC $3030       ; Absolute
  DEC $3031, X    ; Absolute, X

  INC $AA         ; Zero Page
  INC $10, X      ; Zero Page X
  INC $3030       ; Absolute
  INC $3031, X    ; Absolute, X

  LDX #$AA        ; Immediate
  LDX $AA         ; Zero Page
  LDX $10, X      ; Zero Page X
  LDX $3030       ; Absolute
  LDX $3031, X    ; Absolute, X

  LDY #$AA        ; Immediate
  LDY $AA         ; Zero Page
  LDY $10, X      ; Zero Page X
  LDY $3030       ; Absolute
  LDY $3031, X    ; Absolute, X

  LSR A           ; Accumulator
  LSR $AA         ; Zero Page
  LSR $10, X      ; Zero Page X
  LSR $3030       ; Absolute
  LSR $3031, X    ; Absolute, X

  ROL A           ; Accumulator
  ROL $AA         ; Zero Page
  ROL $10, X      ; Zero Page X
  ROL $3030       ; Absolute
  ROL $3031, X    ; Absolute, X

  ROR A           ; Accumulator
  ROR $AA         ; Zero Page
  ROR $10, X      ; Zero Page X
  ROR $3030       ; Absolute
  ROR $3031, X    ; Absolute, X



.segment "RESETVEC"
  .addr $0FAA       ; NMI Vector
  .addr RESET       ; RESET Vector
  .addr $0000       ; IRQ Vector
