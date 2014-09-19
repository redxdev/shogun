// Expressions
import "lib/standard.sl"

entry {
	print(1 + 3)
	print(1 + 3 + 1)
	print(3 - 1 - 2)
	print(3 - 1 + 2)
	print(3 * 3)
	print(3 * 3 + 1 > 0)
	print(5 % 2)
	print((5 + 1) * 3)
	print(true && true)
	print(true && false)
	print(true && true && true)
	print(1 === 2 && true)
	print(0 === 0 && true)
	print(!false && true)
	print(!(true && false))
	print(true || false)
	print(false || false && true)
}