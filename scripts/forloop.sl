// For Loops
extern func print(str)
extern func readline()

entry {
	for(var i = 0; i < 10; i = i + 1) {
		print(i)
	}
	
	for(var i = 0; i < 10; i = i + 1) {
		if(i == 5)
			break
		print(i)
	}
}