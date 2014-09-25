// For Loops
extern func print(str)
extern func readline()

entry {
	print("Iterations?")
	var count = readline()
	for(var i = 0; i < count; i = i + 1) {
		print(i)
	}
}