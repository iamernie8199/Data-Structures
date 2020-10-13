class block:
    def __init__(self, x, y, shape):
        self.x = x
        self.shift = y
        self.shape = shape
        tmp = []
        i = 0
        for j in range(len(shape[0])):
            while True:
                if shape[i][j]:
                    tmp.append([i, j])
                    i = 0
                    break
                else:
                    i += 1
        self.bottom = tmp


class background():
    def __init__(self, m, n):
        self.h = m
        self.w = n
        # 以m*n矩陣儲存
        self.matrix = [[0 for _ in range(n)] for _ in range(m)]

    def out(self):
        for i in range(self.h - 1, -1, -1):
            print(f"{i} | ", end="") if i < 10 else print(f"{i%10} | ", end="")
            for j in range(self.w):
                print(self.matrix[i][j], end=' ')
            print()
        print(f"X | ", end="")
        for k in range(1,self.w+1):
            print(k, end=" ") if k <10 else print(k%10, end=" ")
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
            if check:
                break
        self.out()

    def new(self, block):
        index = block.x - 1
        i = self.h
        shifted = 0
        check = 0
        while i >= 0:
            for k, j in block.bottom:
                if i - 1 + k > self.h - 1:
                    check = 1
                    continue
                if self.matrix[i - 1 + k][index + j]:
                    if block.shift != 0 and not shifted:
                        # 判斷橫移合法性
                        if index + block.shift > self.w - 1 or block.shift + index + len(block.shape[0])-1 > self.w - 1:
                            print("!")
                            return 1
                        for n in range(1, block.shift+1):
                            if self.matrix[i][index + n]:
                                print("!")
                                return 1
                        # 更新index
                        index += block.shift
                        shifted = 1
                        break
                    elif len(block.shape) + i > self.h - 1:
                        return 1
                    else:
                        for z in range(len(block.shape)):
                            for x in range(len(block.shape[0])):
                                if block.shape[z][x]:
                                    self.matrix[i + z][index + x] = block.shape[z][x]
                        return 0
                elif i == 0:
                    if block.shift != 0 and shifted == 0:
                        for z in range(len(block.shape)):
                            for x in range(len(block.shape[0])):
                                if block.shape[z][x]:
                                    self.matrix[i + z][index + block.shift + x] = block.shape[z][x]
                    else:
                        for z in range(len(block.shape)):
                            for x in range(len(block.shape[0])):
                                if block.shape[z][x]:
                                    self.matrix[i + z][index + x] = block.shape[z][x]
                    return 0
                elif k == block.bottom[-1][0] and j == block.bottom[-1][1]:
                    check = 1
            if check:
                i -= 1
                check = 0


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
f = open(r'3.data')
for line in f:
    data.append(line[:-1])
f.close()
data.pop()
data = [d.split(' ') for d in data]

g1 = background(int(data[0][0]), int(data[0][1]))
for i in range(1, len(data)):
    if data[i][0] == 'End':
        break
    print(data[i])
    b = block(int(data[i][1]), int(data[i][2]), block_list[data[i][0]])
    e = g1.new(b)
    if e:
        print("GG")
        break
    g1.refresh()
