// Variables
import "lib/standard.sl"

func foobar(a,b) {
	print(a)
	print(b)
}

entry {
	var bar = rand_int(0,100)
	print(bar)
	bar = "boo"
	print(bar)
	bar = nil
	print(bar)
	
	var foo = "foo"
	bar = foo
	print(bar)
	foobar(foo,bar)
	
	foo = true
	print(foo)
	print(bar)
}