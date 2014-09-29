// Loops
extern func print(str)

entry {
	for(var i = 0; i < 10; i = i + 1) {
		print(i)
	}
	
	for(var i = 0; i < 10; i = i + 1) {
		if(i == 5) {
			break
		}
		
		print(i)
	}
	
	var i = 0
	while(i < 10) {
		print(i)
		i = i + 1
	}
	
	i = 0
	while(i < 10) {
		if(i == 5) {
			break
		}
		
		print(i)
	}
}