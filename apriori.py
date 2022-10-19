import sys
import itertools
import tracemalloc
tracemalloc.start()
sup = sys.argv[2]
conf = sys.argv[3]
with open(sys.argv[1], 'r') as f:
    lines = f.readlines()
arr = []
def L1_itemset():
    count = {}
    count2 = []
    for items in lines:
        items = items.strip()
        temp = items.split(",")
        arr.append(temp)
        for item in temp:
            if item not in count:
                count[(item)] = 1
            else:
                count[(item)] += 1

    for x in count:
        if count[x] >= float(sup)*len(lines):
            count2.append(x)
    return count2


def LK_itemsets(Lk_1, c, k):
    ck = []
    if (c == 1):
        for i in range(len(Lk_1)):
            for j in range(i+1, len(Lk_1)):
                c = []
                c.append(Lk_1[i])
                c.append(Lk_1[j])
                if c not in ck:
                    ck.append(c)

    else:
        for i in range(len(Lk_1)):
            for j in range(i+1, len(Lk_1)):
                l = list(set(Lk_1[i] + Lk_1[j]))
                if len(l) == k:
                    if sorted(l) not in ck:
                        ck.append(sorted(l))

    return ck


def Lk_freq_itemsets(lk, lk_1, n):
    count = []
    count2 = {}
    final = []
    for x in lk:
        c = 0
        ls = set(itertools.combinations(x, n))
        for item in ls:
            if set(item).issubset(set(lk_1)):
                c += 1
        if c == len(ls):
            count.append(x)

    for item in count:
        c = 0
        for x in arr:
            if (set(item).issubset(set(x))):
                c += 1
        count2[tuple(item)] = c

    for item in count2:
        if count2[tuple(item)] >= float(sup)*len(lines):
            if item not in final:
                final.append(item)

    return final


def Lk_freq_itemsets_2(lk, lk_1, n):
    count = []
    count2 = {}
    final = []
    for x in lk:
        c = 0
        ls = set(itertools.combinations(x, n))
        for item in ls:
            flag = 0
            for element in lk_1:
                if set(element) == set(item):
                    flag = 1
                    break
            if flag == 1:
                c += 1
        if c == len(ls):
            if sorted(x) not in count:
                count.append(sorted(x))

    for item in count:
        c = 0
        for x in arr:
            if (set(item).issubset(set(x))):
                c += 1
        count2[tuple(item)] = c
    for item in count2:
        if count2[tuple(item)] >= float(sup)*len(lines):
            final.append(item)

    return final


def rulegenerator(fitems):
    for x in fitems:
        c = 0
        for data in arr:
            if (set(x).issubset(set(data))):
                c += 1
        union_sup = c
        support = union_sup/len(lines)
        subsets = []
        length = len(x)
        for i in range(1, length):
            ls = set(itertools.combinations(x, i))
            subsets += sorted(ls)
        for i in subsets:
            left = i
            right = set(x) - set(i)
            c = 0
            for data in arr:
                if (set(left).issubset(set(data))):
                    c += 1
            pre_conf = union_sup/c
            res = "{"
            if pre_conf >= float(conf):
                j = 0
                for i in list(left):
                    if j == 1:
                        res += ','
                    res += i
                    j = 1
                res += '}{'
                for i in list(right):
                    if j == 0:
                        res += ','
                    res += i
                    j = 0
                res = res + '}[' + str(support) + ',' + \
                    "{:.4f}".format(pre_conf) + ']'
                print(res)


def apriori():
    L1= L1_itemset()
    freq_items = L1
    Lk_1 = L1
    pre_final = []
    k = 2
    while len(freq_items):
        pre_final = freq_items
        if k == 2:
            freq_items = LK_itemsets(freq_items, 1, k)
            freq_items = Lk_freq_itemsets(freq_items, Lk_1, k-1)
            Lk_1 = freq_items
            # print(tracemalloc.get_traced_memory())
        else:
            freq_items = LK_itemsets(freq_items, 0, k)
            freq_items = Lk_freq_itemsets_2(freq_items, Lk_1, k-1)
            Lk_1 = freq_items
        k += 1
    print("Association Rules are:")    
    rulegenerator(pre_final)


apriori()
print(tracemalloc.get_traced_memory())
tracemalloc.stop()
