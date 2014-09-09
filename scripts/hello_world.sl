// Hello world!
import "lib/standard.sl" // import the standard library

func foo() {
	print("Hello World!")
}

entry {
	foo()
	foo()
	foo()
}