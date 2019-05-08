from unidecode import unidecode
pairs = set((('a','a'), ('b','d'), ('d','b'), ('e','g'), ('g','e'), ('h','h'), ('l','l'), ('m','m'), ('o','o'), ('p','q'), ('q','p'), ('t','t'), ('u','u'), ('v','v'), ('w','w'), ('x','x'), ('s','z'), ('z','s'), ('y','y')))
output = set()
with open('words.txt','r') as file:
    for line in file:
        line = unidecode(line.strip().lower())
        half = len(line) // 2 + len(line) % 2
        for x, y in zip(line[:half], line[-1:-half - 1:-1]):
            if (x,y) not in pairs:
                break
        else:
            output.add(line)
for word in sorted(output):
    print(word)