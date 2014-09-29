// Loops
extern func print(str)

entry {
	print("For: 0 to 9")
	for(var i = 0; i < 10; i = i + 1) {
		print(i)
	}
	
	print("For: 0 to 9, break before 5")
	for(var i = 0; i < 10; i = i + 1) {
		if(i == 5) {
			break
		}
		
		print(i)
	}
	
	print("While: 0 to 9")
	var i = 0
	while(i < 10) {
		print(i)
		i = i + 1
	}
	
	print("While: 0 to 9, break before 5")
	i = 0
	while(i < 10) {
		if(i == 5) {
			break
		}
		
		print(i)
		
		i = i + 1
	}
	
	print("Do..While: 11")
	i = 11
	do {
		print(i)
	} while(i < 10)
}