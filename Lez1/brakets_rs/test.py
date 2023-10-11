from random import randint

N = 100_000
K = 4


# Open a file for writing
with open('input.txt', 'w') as file:
    file.write(f"1 {N} {K} ")
    # Write numbers to the file
    for i in range(N):
        file.write(str(randint(0,K)))  # % 5 ensures numbers are between 0 and 4
        file.write(' ')  # Add a space between numbers
    file.close()


print("Numbers written to 'input.txt'")