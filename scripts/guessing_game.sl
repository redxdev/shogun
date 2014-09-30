// Guessing Game

import "lib/standard.sl" lib

entry {
	var num = rand_int(0,100)
	print("I'm thinking of a number between 0 and 100")
	while(true) {
		print("What's your guess?")
		var guess = readline()
		if(guess == num) {
			print("Correct!")
			return
		}
		else if(guess < num) {
			print("Too small.")
		}
		else {
			print("Too large.")
		}
	}
}