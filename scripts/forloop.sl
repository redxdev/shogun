// For Loops
import "lib/standard.sl"

entry {
	print("Iterations?")
	var count = readline()
	for(var i = 0; i < count; i = i + 1) {
		print(i)
	}
}