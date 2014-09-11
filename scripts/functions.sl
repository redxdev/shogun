// Hello world!
import "lib/standard.sl" // import the standard library

func foo() {
	print("I'm foo!")
}

func bar() {
	return "bar"
}

entry {
	foo()
	print("Hello World!")
	print(bar())
}