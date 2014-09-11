// Hello world!
import "lib/standard.sl" // import the standard library

func foo() {
	print("I'm foo!")
}

func bar() {
	return "bar"
}

func baz(a,b) {
	print(a)
	print(b)
}

entry {
	foo()
	print("Hello World!")
	print(bar())
	baz("a","b")
}