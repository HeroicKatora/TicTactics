def bewerteBoard(setP1, setP2):
	pop1 = popCount(setP1)
	pop2 = popCount(setP2)
	ret = 0
	if pop1 > pop2:
		ret+=1
	if pop2 > pop1:
		ret-=1
	return ret
	
def popCount(integer):
	return bin(integer).count('1')

file = open("RatingTable.h", 'w')
file.write("#pragma once\nfloat ratingTable [] = {0\n")
for setPlayerOne in range(2**9):
	for setPlayerTwo in range(2**9):
		if not setPlayerOne and not setPlayerTwo:
			continue
		file.write(","+str(bewerteBoard(setPlayerOne, setPlayerTwo))+"\n")

file.write("};\n")
file.close()