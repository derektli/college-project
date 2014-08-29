import random
import subprocess


fin = open("imports-85.data","r")
a = list()
for line in fin:
    a.append(line)
fin.close()


testsize = len(a)/10

def evaluate(trainsize):
    random.shuffle(a)
    print "\nTraining size = {}".format(trainsize)
    for i in range(10):
	fout1 = open("train-set","w")
	fout2 = open("test-set","w")
	k = i*10
	
	for j in range(testsize):
	    fout2.write(a[k])
	    k+=1
	
	if k>=len(a):
	    k=0
	for j in range(trainsize):
	    fout1.write(a[k])
	    k+=1
	    if k>=len(a):
		k=0
	fout1.close()
	fout2.close()
	subprocess.call("python naive-bayes.py train-set test-set ",shell=True)
	subprocess.call("python regression-tree.py train-set test-set ",shell=True)



trainsize = 20
evaluate(trainsize)
trainsize = 40
evaluate(trainsize)
trainsize = 60
evaluate(trainsize)
trainsize = 120
evaluate(trainsize)
trainsize = 180
evaluate(trainsize)



