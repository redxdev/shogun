// Recursive Fibonacci
import "lib/standard.sl"

func fibonacci(n) {
	if(n <= 1) {
		return 1
	}
	
	return fibonacci(n - 1) + fibonacci(n - 2)
}

entry {
	print("N?")
	fibonacci(readline())
}