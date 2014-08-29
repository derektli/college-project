import random
fin = open("imports-85.data","r")
num=0
a = list()
b = list()
for line in fin:
    a.append(line)
fin.close()

#for i in range(0,len(a)):
    #print a[i],

#print "================"
#for i in range(0,len(b)):
    #print b[i],


random.shuffle(a)
i = 0 
while num<180:
    print a[i],
    i+=1
    num+=1
