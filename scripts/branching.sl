// Branching
extern func print(str)

entry {
	if(true)
		print(true)
	
	if(false) {
		print(true)
	}
	else {
		print(false)
	}
	
	if(false) {
		print("nope")
	}
	else if(true) {
		print("yep")
	}
	else {
		print("haha no")
	}
	
	if(false) {
		print("nope")
	}
	else if(false) {
		print("haha no")
	}
	else {
		print("yep")
	}
}