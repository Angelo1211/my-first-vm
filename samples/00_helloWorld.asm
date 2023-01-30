; LC-3 program that displays "Hello World!" to the console
; Taken from https://people.cs.georgetown.edu/~squier/Teaching/HardwareFundamentals/LC3-trunk/docs/LC3-AssemblyManualAndExamples.pdf
.ORIG x3000
LEA R0, HW  ; Load address of "Hello World!" string
PUTS 	 	; output the string to the console
HALT 		; end program

HW .STRINGZ "Hello World!"
   .END