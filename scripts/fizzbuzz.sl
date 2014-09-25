// FizzBuzz
extern func print(str)
extern func readline()

entry {
	print("N?")
	var count = readline()
	
	for(var i = 1; i <= count; i = i + 1) {
		var output = ""
		
		if(i % 3 === 0) {
			output = "Fizz"
		}
		
		if(i % 5 === 0) {
			output = output .. "Buzz"
		}
		
		if(i % 3 !== 0 && i % 5 !== 0) {
			output = i
		}
		
		print(output)
	}
}