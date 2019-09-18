class Solution():

    @staticmethod
    def stackblocking(list):
        hight = 0
        current, last = [], []
        for i in reversed(list):
            current.append(i)
            if sum(current) > sum(last):
                last = current
                current = []
                hight += 1
                print hight, last
        if current != []:
            print hight, last + current
        return hight

import random
testcase = range(16)
random.shuffle(testcase)
print testcase
Solution.stackblocking(testcase)
