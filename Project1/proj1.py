class block:
    def __init__(self, x, y, shape):
        self.x = x
        self.shift = y
        self.shape = shape

    def bottom(self):
        tmp = []
        i = 0
        for j in range(len(self.shape[0])):
            while True:
                if self.shape[i][j]:
                    tmp.append([i, j])
                    break
                else:
                    i += 1
        return tmp


class background():
    def __init__(self, m, n):
        self.h = m
        self.w = n
        self.matrix = [[0 for _ in range(n)] for _ in range(m)]

    def out(self):
        for i in range(self.h - 1, -1, -1):
            print(f"{i} | ", end="")
            for j in range(self.w):
                print(self.matrix[i][j], end=' ')
            print()
        print("## ##########")
        print(f"X | ", end="")
        for k in range(self.w):
            print(k, end=" ")
        print('\n')

    def refresh(self):
        check = 0
        while (True):
            for i in range(self.h):
                if self.matrix[i] == [1] * self.w:
                    self.matrix.pop(i)
                    self.matrix.append([0] * self.w)
                    break
                elif i == self.h - 1:
                    check = 1
                    break
            if check: break
        self.out()

    def new(self, block):
        index = block.x - 1
        i = self.h - 1
        #i = self.h - 1 - len(block.shape)
        shifted = 0
        while i >= 0:
            """
            for k, j in block.bottom():
                if self.matrix[i+k][index + j]:
                    if block.shift != 0 and not shifted:
                        index += block.shift
                        shifted = 1
                        break
            """
            for j in range(len(block.shape[0])):
                if self.matrix[i][index + j] and block.shape[0][j]:
                    if block.shift != 0 and not shifted:
                        index += block.shift
                        shifted = 1
                        break
                    elif i == self.h - 1 or i + len(block.shape) > self.h - 1:
                        print("GG")
                        i = -1
                        return 1
                    else:
                        for k in range(len(block.shape)):
                            self.matrix[i + 1 + k][index:index + len(block.shape[0])] = block.shape[k]
                        i = -1
                elif block.shape[0][j] == 0:
                    if self.matrix[i][index + j] and block.shape[1][j]:
                        if block.shift != 0 and not shifted:
                            index += block.shift
                            shifted = 1
                            break
                        else:
                            for k in range(len(block.shape)):
                                self.matrix[i + k][index:index + len(block.shape[0])] = block.shape[k]
                            i = -1
                if j == len(block.shape[0]) - 1:
                    if i == 0:
                        for k in range(len(block.shape)):
                            self.matrix[i + k][index:index + len(block.shape[0])] = block.shape[k]
                    i -= 1
        self.out()


block_list = {
    "T1": [[0, 1, 0], [1, 1, 1]],
    "T2": [[0, 1], [1, 1], [0, 1]],
    "T3": [[1, 1, 1], [0, 1, 0]],
    "T4": [[1, 0], [1, 1], [1, 0]],
    "L1": [[1, 1], [1, 0], [1, 0]],
    "L2": [[1, 0, 0], [1, 1, 1]],
    "L3": [[0, 1], [0, 1], [1, 1]],
    "L4": [[1, 1, 1], [0, 0, 1]],
    "J1": [[1, 1], [0, 1], [0, 1]],
    "J2": [[1, 1, 1], [1, 0, 0]],
    "J3": [[1, 0], [1, 0], [1, 1]],
    "J4": [[0, 0, 1], [1, 1, 1]],
    "S1": [[1, 1, 0], [0, 1, 1]],
    "S2": [[0, 1], [1, 1], [1, 0]],
    "Z1": [[0, 1, 1], [1, 1, 0]],
    "Z2": [[1, 0], [1, 1], [0, 1]],
    "I1": [[1], [1], [1], [1]],
    "I2": [[1, 1, 1, 1]],
    "O": [[1, 1], [1, 1]]
}

data = []
f = open(r'data.txt')
for line in f:
    data.append(line[:-1])
f.close()
data.pop()
data = [d.split('\t') for d in data]

g1 = background(int(data[0][0]), int(data[0][1]))
g1.refresh()
for i in range(1, len(data)):
    b = block(int(data[i][1]), int(data[i][2]), block_list[data[i][0]])
    e = g1.new(b)
    if e: break
    g1.refresh()
    print("--------------------")
