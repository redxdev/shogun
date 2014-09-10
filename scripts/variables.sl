// Hello world!
import "lib/standard.sl" // import the standard library

func foo() {
	return "Hello World!"
}

entry {
	var bar
	foo()
	asm [[
		push "print"
		ecall
	]]
}