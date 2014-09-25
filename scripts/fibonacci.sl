// Iterative Fibonacci
import "lib/standard.sl" lib

func fibonacci(n) {
	var first = 1
	var second = 2
	var next
	
	for(var c = 0; c < n; c = c + 1) {
		if(c <= 1)
			next = c
		else {
			next = first + second
			first = second
			second = next
		}
	}
	
	return next
}

entry {
	print("N?")
	print(fibonacci(to_number(readline())))
}