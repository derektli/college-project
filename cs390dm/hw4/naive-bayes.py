#Name: Tingjun Li
#Email: li400@purdue.edu

import sys
    
#=============initializing===========
feature = []
feature.append([])
feature[0].append("alfa-romero")
feature[0].append("audi")
feature[0].append("bmw")
feature[0].append("chevrolet")
feature[0].append("dodge")
feature[0].append("honda")
feature[0].append("isuzu")
feature[0].append("jaguar")
feature[0].append("mazda")
feature[0].append("mercedes-benz")
feature[0].append("mercury")
feature[0].append("mitsubishi")
feature[0].append("nissan")
feature[0].append("peugot")
feature[0].append("plymouth")
feature[0].append("porsche")
feature[0].append("renault")
feature[0].append("saab")
feature[0].append("subaru")
feature[0].append("toyota")
feature[0].append("volkswagen")
feature[0].append("volvo")

feature.append([])
feature[1].append("diesel")
feature[1].append("gas")

feature.append([])
feature[2].append("std")
feature[2].append("turbo")

feature.append([])
feature[3].append("four")
feature[3].append("two")

feature.append([])
feature[4].append("hardtop")
feature[4].append("wagon")
feature[4].append("sedan")
feature[4].append("hatchback")
feature[4].append("convertible")

feature.append([])
feature[5].append("4wd")
feature[5].append("fwd")
feature[5].append("rwd")

feature.append([])
feature[6].append("front")
feature[6].append("rear")

feature.append([])
feature[7].append("dohc")
feature[7].append("dohcv")
feature[7].append(",l,")
feature[7].append("ohc")
feature[7].append("ohcf")
feature[7].append("ohcv")
feature[7].append("rotor")

feature.append([])
feature[8].append("eight")
feature[8].append("five")
feature[8].append("four")
feature[8].append("six")
feature[8].append("three")
feature[8].append("twelve")
feature[8].append("two")

feature.append([])
feature[9].append("1bbl")
feature[9].append("2bbl")
feature[9].append("4bbl")
feature[9].append("idi")
feature[9].append("mfi")
feature[9].append("mpfi")
feature[9].append("spdi")
feature[9].append("spfi")

number = []    #number array: storing the number of each attribute 
record = []    #record array: storing the number of high risk case
total = 0      #total: the total number of cases in the training set
totalyes = 0   #totalyes: the total number of high risk cases in the training set

for i in range(10):
    number.append([])
    record.append([])
    for j in range(len(feature[i])):
	number[i].append(0)
	record[i].append(0)

#====================================





def importTrainData(filename):
    global total
    global totalyes
    
    fin = open(filename,"r")
    i=0
    for line in fin:
	total+=1
	a = line.strip().split(',')
	
	temp = int(a[0])
	if temp>0:
	    totalyes+=1
	
	for i in range(10):
	    for j in range(len(feature[i])):
		if line.find(feature[i][j])!=-1:
		    number[i][j]+=1
		    if temp>0:
			record[i][j]+=1
    fin.close()

    
def predict(filename):
    global total
    global totalyes
    
    fin = open(filename,"r")
    count = 0
    wcount = 0
    for line in fin:
	numerator = 1.0
	denominator = 1.0
	count+=1
	
	a = line.strip().split(',')
	temp = 0
	if int(a[0])>0:
	    temp = 1
	    
	
	for i in range(10):
	    for j in range(len(feature[i])):
		if line.find(feature[i][j])!=-1:
		    numerator*=(float(record[i][j])+1)/(float(totalyes)+len(feature[i]))
		    # Not sure how to smooth denominator
		    denominator*=(float(number[i][j])+1)/(float(total)+len(feature[i]))
		
	result = 0
	if numerator/denominator>0.5:
	    result = 1
	
	if result != temp:
	    wcount+=1
	    
    fin.close()
    return float(wcount)/float(count)
    
def main():
    if (len(sys.argv) > 2):
	trainfile = sys.argv[1]
	testfile = sys.argv[2]
	importTrainData(trainfile)
	score = predict(testfile)
	print 'Naive-Bayes ZERO-ONE LOSS = {}'.format(score)
    else: 
	print "Not Enough Arguments! format: python naive-bayes.py train-set.dat test-set.dat"
	
    
main()