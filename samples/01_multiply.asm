.ORIG x3000

LD R1, LEFT_OPERAND
LD R2, RIGHT_OPERAND

LOOP:
ADD R0, R0, R2
ADD R1, R1, #-1
BRp LOOP
	
HALT

;; Program Data	
LEFT_OPERAND:  .FILL x0010
RIGHT_OPERAND: .FILL x0006

.END