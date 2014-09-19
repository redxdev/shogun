// FizzBuzz
extern func print(str)
extern func readline()

entry {
	print("N?")
	var count = readline()
	
	for(var i = 0; i < count; i = i + 1) {
		if(i % 3 === 0) {
			print("Fizz")
		}
		else {
			print(i)
		}
	}
}