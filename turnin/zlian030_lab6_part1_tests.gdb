# Test file for Lab6_SynchSMs


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

test "2 loops to first led then 5 loops after on led 3"
continue 2
expectPORTB 0x01
continue 5
expectPORTB 0x04
checkResult

test "follow by 1 loop to led 1 and 1 more loop to led 2"
continue 1
expectPORTB 0x01
continue 1
expectPORTB 0x02
checkResult

test "led 3"
continue 1
expectPORTB 0x04
checkResult

test "led 1"
continue 1
expectPORTB 0x01
checkResult

test "led 2"
continue 1
expectPORTB 0x02
checkResult


# Add tests below

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
