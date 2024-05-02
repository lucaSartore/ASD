
# complexity: 2^(n1+n2)
def print_all(n1, n2, current = ""):

    if n1 != 0:
        print_all(n1-1,n2,current+"k")

    if n2 != 0:
        print_all(n1,n2-1,current+"g")

    if n1 == 0 and n2 == 0:
        print(current)

def fact(n):
    r=1
    for i in range(1,n+1):
        r*= i
    return r


# complexity: n log(n) (considering the case where the numbers are bigger that 64 bits)
def count_all(n1,n2):
    return fact(n1+n2)/fact(n1)/fact(n2)

print_all(2,4)
print(count_all(2,4)) 