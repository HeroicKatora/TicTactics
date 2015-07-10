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
            
midMask    = int('000010000', 2)
edgesMask  = int('010101010', 2)
cornerMask = int('101000101', 2)


def bewerteBoard(setP1, setP2):
    pop1 = popCount(setP1)
    pop2 = popCount(setP2)
    
    '''
    winP1Bool = False
    winP2Bool = False
    
    for mask in winsMasks:
        if (setP1 & mask) == mask:
            winP1Bool = True
            
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
    
    chancesP1 = 0
    for mask in chancesMasks:
        if (setP1 & mask) == mask: # Wenn P1 eine potentielle chance hat
            for winMask in winsMasks:
                if popCount(winMask&mask) == 2: # suche die winMask die auf die mask passt
                    if (setP2 & winMask) != 0:  # und ein teil von P2 darauf liegt
                        break; # dann ist die position doch keine chance
            else:
                chancesP1 += 1
    
    chancesP2 = 0
    for mask in chancesMasks:
        if (setP2 & mask) == mask: # Wenn P2 eine potentielle chance hat
            for winMask in winsMasks:
                if popCount(winMask&mask) == 2: # suche die winMask die auf die mask passt
                    if (setP1 & winMask) != 0:  # und ein teil von P1 darauf liegt
                        break; # dann ist die position doch keine chance
            else:
                chancesP2 += 1
    
    p1mid = setP1 & midMask
    p2mid = setP2 & midMask
    
     
    edges = 0

    for i in range(9):
        if (setP1 & (1 << i)) & edgesMask:
            edges += 1
        if (setP2 & (1 << i)) & edgesMask:
            edges -= 1
            
    corners = 0
    
    for i in range(9):
        if (setP1 & (1 << i)) & cornerMask:
            corners += 1
        if (setP2 & (1 << i)) & cornerMask:
            corners -= 1
    
    
    # BEWERTUNG -----------------
    
    ret = (pop1 - pop2) * more_bonus
    
    ret += chancesP1 * chance_bonus
    ret -= chancesP2 * chance_bonus
    
    ret += edges * edge_bonus
    ret += corners * corner_bonus
    
    if p1mid:
        ret += mid_bonus
    if p2mid:
        ret -= mid_bonus

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
file.write("#include \"types.hpp\"\nFieldBits winMoveTable [] = {0\n")

for set in range(2**9):
	if not set:
		continue
	moves = 0
	for tryMove in [1,2,4,8,16,32,64,128,256]:
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

file.write("bool winsTable[] = {0,")

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
file.close();