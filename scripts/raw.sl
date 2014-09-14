// Playing with raw assembly

import "lib/standard.sl"

func foo() {
	asm [[
		push "Hello World!"
	]]
}

entry {
	// functions
	foo()
	asm [[
		push "print"
		ecall
	]]
	
	// variables
	var foo = "bar"
	asm var foo // pushes the value of foo onto the stack
	asm [[
		push "print"
		ecall
	]]
	
	foo = asm value [[ ; adding the 'value' keyword allows the use of asm in arguments
		push "baz"
	]]
	print(foo)
}