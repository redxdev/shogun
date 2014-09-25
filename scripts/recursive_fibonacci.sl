// Recursive Fibonacci
import "lib/standard.sl" lib

func fibonacci(n) {
	if(n <= 1) {
		return 1
	}
	
	return fibonacci(n - 1) + fibonacci(n - 2)
}

entry {
	print("N?")
	print(fibonacci(to_number(readline())))
}