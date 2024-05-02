
G = [
    [4,5],
    [2,0],
    [1,3],
    [2,0],
    [],
    [],
]

VISITED = [False] * len(G)

def count(s,k):

    if k == 0:
        return 1

    VISITED[s] = True

    c = 0

    for adj in G[s]:
        if not VISITED[adj]:
            c += count(adj,k-1)

    VISITED[s] = False
    
    return c

print(count(2,3))