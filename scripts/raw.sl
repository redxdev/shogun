// Playing with raw assembly
func foo() {
	asm [[
		push "Hello World!"
	]]
}

entry {
	foo()
	asm [[
		push "print"
		ecall
	]]
}