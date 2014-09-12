// Hello world!
import "lib/standard.sl" // import the standard library

func baz(a,b) {
	print(a)
	print(b)
}

func boom(a,b) {
	baz(b,a)
}

entry {
	boom("world", "hello")
}