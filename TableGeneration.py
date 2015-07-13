#winP1 = 100
#winP2 = -100
chance_bonus = 15
more_bonus = 0
mid_bonus = 4
corner_bonus = 3
edge_bonus = 2
minmaxscore = 70

winsMasks =     [
                int('000000111', 2),
                int('000111000', 2),
                int('111000000', 2),
                int('100100100', 2),
                int('010010010', 2),
                int('001001001', 2),
                int('100010001', 2),
                int('001010100', 2)
                ]

chancesMasks =  [                
                int('000000011', 2),
                int('000000101', 2),
                int('000000110', 2),
                
                int('000011000', 2),
                int('000101000', 2),
                int('000110000', 2),
                
                int('011000000', 2),
                int('101000000', 2),
                int('110000000', 2),
                
                int('000100100', 2),
                int('100000100', 2),
                int('100100000', 2),
                
                int('000010010', 2),
                int('010000010', 2),
                int('010010000', 2),
                
                int('000001001', 2),
                int('001000001', 2),
                int('001001000', 2),
                
                int('000010001', 2),
                int('100000001', 2),
                int('100010000', 2),
                
                int('000010100', 2),
                int('001000100', 2),
                int('001010000', 2)
            ]

allMoves = [1,2,4,8,16,32,64,128,256]
midMask    = int('000010000', 2)
edgesMask  = int('010101010', 2)
cornersMask = int('101000101', 2)

def isWin(set):
    for mask in winsMasks:
        if (set & mask) == mask:
            return True
    return False

def chances(set):
    chances = 0
    for mask in chancesMasks:
        if (set & mask) == mask: # Wenn P2 eine potentielle chance hat
            for third in allMoves:
                if(third & set):
                    continue
                if(isWin(set|third)):
                    chances = chances+third
    return chances

#Bezieht chance count nicht mit ein
def bewerteSingle(set):
	pop = popCount(set)
	mid = set&midMask
	edges = popCount(edgesMask&set)
	corners = popCount(cornersMask&set)
	
	ret = pop *more_bonus
	if(mid):
		ret += mid*mid_bonus
	ret += edges*edge_bonus
	ret += corners*corner_bonus
	return ret

def bewerteBoard(setP1, setP2):
    pop1 = popCount(setP1)
    pop2 = popCount(setP2)
    
    '''
    winP1Bool = False
    winP2Bool = False
    
    winP1Bool = isWin(setP1)
            
    for mask in winsMasks:
        if (setP2 & mask) == mask:
            winP2Bool = True
    
    if winP1Bool and winP2Bool:
        return 0
    
    if winP1Bool:
        return winP1
    
    if winP2Bool:
        return winP2
    '''
    
    chancesP1 = chances(setP1)
    chancesP2 = chances(setP2)
    
    chanceCountP1 = popCount(chancesP1 & ~setP2)
    chanceCountP2 = popCount(chancesP2 & ~setP1)
    
    ret = 0
    ret += chanceCountP1 * chance_bonus
    ret -= chanceCountP2 * chance_bonus
    
    ret += bewerteSingle(setP1)
    ret -= bewerteSingle(setP2)
    
    return int(min(max(ret, -minmaxscore), minmaxscore))
	
def popCount(integer):
    return bin(integer).count('1')

file = open("RatingTable.h", 'w')
file.write("#pragma once\nsigned ratingTable [] = {0\n")

summe = 0

for setPlayerOne in range(2**9):
    for setPlayerTwo in range(2**9):
        if not setPlayerOne and not setPlayerTwo:
            continue
        score = bewerteBoard(setPlayerOne, setPlayerTwo)
        summe += score
        file.write(","+str(score)+"\n")

print("Summe aller Scores: " + str(summe))

file.write("};\n")
file.close()
# Generate win moves table
file = open("WinMoveTable.h", 'w')
file.write("//This file is used to look up which moves could win a board in a certain situation\n")
file.write("//Winning moves are moves, after which the player has a tic tac toe\n")
file.write("//Therefore this does not check if the tic tac toe existed before (subject to change)")
file.write("#include \"types.hpp\"\n[[gnu::unused]]static FieldBits winMoveTable [] = {0\n")

for set in range(2**9):
	if not set:
		continue
	moves = 0
	for tryMove in allMoves:
		if set & tryMove:
			continue
		winMove = False
		for win in winsMasks:
			if ((set|tryMove)&win) == win:
				winMove = True
				break
		if winMove:
			moves += tryMove
	file.write(","+str(moves)+"\n")

file.write("};\n")

file.write("[[gnu::unused]]static bool winsTable[] = {0,\n")

for set in range(2**9):
	if not set:
		continue
	for mask in winsMasks:
		if (set & mask) == mask:
			file.write("1,\n")
			break
	else:
		file.write("0,\n")

file.write("};\n")

file.write("[[gnu::unused]]static FieldBits chancesTable[] = {0,\n")

for set in range(2**9):
	if not set:
		continue
	file.write(str(chances(set))+",\n");

file.write("};\n")

file.write("[[gnu::unused]]static Rating singleRatingTable[] = {0,\n")

for set in range(2**9):
	if not set:
		continue
	file.write(str(bewerteSingle(set))+",\n");

file.write("};\n")


file.close();