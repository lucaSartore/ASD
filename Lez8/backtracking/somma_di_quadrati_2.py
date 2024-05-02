

number = 10


CURRENT_ITEMS = []

def print_all(n):

    if n == 0:
        v = " + ".join(map(lambda x: str(x)+"^2", CURRENT_ITEMS)) + " = " + str(number)
        print(v)
        return
    
    # to make the sequence in asc order (and avoid combinations)
    try:
        last = CURRENT_ITEMS[-1]
    except:
        last = n 

    for i in range(1,last+1):
        
        square = i**2

        if square > n:
            break

        CURRENT_ITEMS.append(i)
        print_all(n-square)
        CURRENT_ITEMS.pop()


print_all(10)