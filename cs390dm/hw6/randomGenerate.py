import random
import subprocess


fin = open("imports-85.data","r")
a = list()
for line in fin:
    a.append(line)
fin.close()


def generate(trainsize):
    random.shuffle(a)
    print "\nTraining size = {}".format(trainsize)
    for i in range(10):
	fout1 = open("random-train-set","w")
	
	for j in range(0,trainsize):
	    fout1.write(a[j])
	fout1.close()
	
generate(len(a)/4)