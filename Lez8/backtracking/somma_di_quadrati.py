

number = 10

DP = [0]*(number+1)

DP[1]=1

for i in range(2,number+1):

    current_min = i

    for base in range(1,i+1):

        square = base**2

        if square > i:
            break
        
        new_possible_min = DP[i-square]+1
        
        current_min = min(new_possible_min,current_min)
    
    DP[i]=current_min
    print(DP)

print(DP[number])