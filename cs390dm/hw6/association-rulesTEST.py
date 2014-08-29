#Name: Tingjun Li
#Email: li400@purdue.edu

import sys
import itertools

features = ["alfa-romero", "audi", "bmw", "chevrolet", "dodge", "honda", "isuzu", "jaguar", 
	    "mazda", "mercedes-benz", "mercury", "mitsubishi", "nissan", "peugot", "plymouth", 
	    "porsche", "renault", "saab", "subaru", "toyota", "volkswagen", "volvo", "diesel",
	    "gas", "std", "turbo", "four-door", "two-door", "hardtop", "wagon", "sedan", "hatchback", 
	    "convertible", "4wd", "fwd", "rwd", "front", "rear", "dohc", "dohcv", ",l,", "ohc", 
	    "ohcf", "ohcv", "rotor", "eight-cylinders", "five-cylinders", "four-cylinders", 
	    "six-cylinders", "three-cylinders", "twelve-cylinders", "two-cylinders",
	    "1bbl", "2bbl", "4bbl", "idi", "mfi", "mpfi", "spdi", "spfi"]


#define a 2d array data to store training data   
data = [[]]
d = [[]]

for i in range(0,220):
    data.append([])
    for j in range(0,len(features)+2):
	data[i].append(0)	

# 0 High 61 low 
# 1 - 60 regular features
	
def importTrainData(filename):
    global d
    global data
    
    fin = open(filename,"r")
    i=0
    for line in fin:
	a = line.strip().split(',')
	if a[0]>'0':
	    data[i][0] = 1
	else:
	    data[i][61] = 1    
	if a[2]=="alfa-romero":
	    data[i][1] = 1
	elif a[2]=="audi":
	    data[i][2] = 1
	elif a[2]=="bmw":
	    data[i][3] = 1
	elif a[2]=="chevrolet":
	    data[i][4] = 1
	elif a[2]=="dodge":
	    data[i][5] = 1
	elif a[2]=="honda":
	    data[i][6] = 1
	elif a[2]=="isuzu":
	    data[i][7] = 1
	elif a[2]=="jaguar":
	    data[i][8] = 1
	elif a[2]=="mazda":
	    data[i][9] = 1
	elif a[2]=="mercedes-benz":
	    data[i][10] = 1
	elif a[2]=="mercury":
	    data[i][11] = 1
	elif a[2]=="mitsubishi":
	    data[i][12] = 1
	elif a[2]=="nissan":
	    data[i][13] = 1    
	elif a[2]=="peugot":
	    data[i][14] = 1
	elif a[2]=="plymouth":
	    data[i][15] = 1
	elif a[2]=="porsche":
	    data[i][16] = 1
	elif a[2]=="renault":
	    data[i][17] = 1
	elif a[2]=="saab":
	    data[i][18] = 1
	elif a[2]=="subaru":
	    data[i][19] = 1
	elif a[2]=="toyota":
	    data[i][20] = 1
	elif a[2]=="volkswagen":
	    data[i][21] = 1
	elif a[2]=="volvo":
	    data[i][22] = 1        
	    
	
	if a[3]=="diesel":
	    data[i][23] = 1
	elif a[3]=="gas":
	    data[i][24] = 1
	
	    
	if a[4]=="std":
	    data[i][25] = 1
	elif a[4]=="turbo":
	    data[i][26] = 1    
	    
	if a[5]=="four":
	    data[i][27] = 1
	elif a[5]=="two":
	    data[i][28] = 1    
	   
	   
	if a[6]=="hardtop":
	    data[i][29] = 1
	elif a[6]=="wagon":
	    data[i][30] = 1
	elif a[6]=="sedan":
	    data[i][31] = 1
	elif a[6]=="hatchback":
	    data[i][32] = 1
	elif a[6]=="convertible":
	    data[i][33] = 1
	    
	
	if a[7]=="4wd":
	    data[i][34] = 1
	elif a[7]=="fwd":
	    data[i][35] = 1
	elif a[7]=="rwd":
	    data[i][36] = 1
	
	
	if a[8]=="front":
	    data[i][37] = 1
	elif a[8]=="rear":
	    data[i][38] = 1
	    
	    
	if a[14]=="dohc":
	    data[i][39] = 1
	elif a[14]=="dohcv":
	    data[i][40] = 1
	elif a[14]=="l":
	    data[i][41] = 1
	elif a[14]=="ohc":
	    data[i][42] = 1
	elif a[14]=="ohcf":
	    data[i][43] = 1
	elif a[14]=="ohcv":
	    data[i][44] = 1
	elif a[14]=="rotor":
	    data[i][45] = 1
	    
	
	
	
	if a[15]=="eight":
	    data[i][46] = 1
	elif a[15]=="five":
	    data[i][47] = 1
	elif a[15]=="four":
	    data[i][48] = 1
	elif a[15]=="six":
	    data[i][49] = 1
	elif a[15]=="three":
	    data[i][50] = 1
	elif a[15]=="twelve":
	    data[i][51] = 1
	elif a[15]=="two":
	    data[i][52] = 1
	
	
	if a[17]=="1bbl":
	    data[i][53] = 1
	elif a[17]=="2bbl":
	    data[i][54] = 1
	elif a[17]=="4bbl":
	    data[i][55] = 1
	elif a[17]=="idi":
	    data[i][56] = 1
	elif a[17]=="mfi":
	    data[i][57] = 1
	elif a[17]=="mpfi":
	    data[i][58] = 1
	elif a[17]=="spdi":
	    data[i][59] = 1
	elif a[17]=="spfi":
	    data[i][60] = 1
	
	i=i+1
	
    fin.close()
    d = data[:i]
    #print d[i-1]
    #print len(features)
    
	    

def getComRange(l,length):
    a = []
    for i in range(0,len(features)+2):
	a.append(0)
    for i in range(0,len(l)):
	if length==1:
	    a[l[i]]=1
	else:
	    for j in range(0,length):
		a[l[i][j]]=1
    ans = []
    for i in range(0,len(features)+2):
	if a[i]==1:
	    ans.append(i)
    #print ans
    return ans
	    
    

def candidateItemsetGeneration(length, l, minsup):
    c = []
    ctemp = []
    if length==1 :
	c = list(range(0,len(features)+2))	
    else:
	ctemp = list(itertools.combinations(getComRange(l,length-1), length))
	temp = []
	if length==2:
	    for i in range(0, len(l)):
		temp.append((l[i],))
	    l = temp
	
	for i in range(0,len(ctemp)):
	    #temp = list(itertools.combinations(ctemp[i], length-1))
	    #print temp
	    #print l
	    #flag = True
	    #for j in range(0,len(temp)):
		#if (temp[j] in l)==False:
		 #   flag = False
	    
	   # if flag==True:
	    c.append(ctemp[i])
		
    return c
	    

	    
def frequentItemGeneration(minsup):
    global d
    global data
    frel = []
    l = [1,1]
    r = 1
    c = [[]]
    ans = []
    while (len(l)>0):
	t = l
	c = candidateItemsetGeneration(r, t, minsup)
	count = []
	for i in range(0,len(c)):
	    count.append(0)
	for i in range(0,len(c)):
	    for j in range(0,len(d)):
		if (r==1):
		    if (d[j][c[i]]==1):
			count[i]+=1
		else:
		    flag = 1
		    for k in range(0,len(c[i])):
			if (d[j][c[i][k]]==0):
			    flag = 0
		    if (flag == 1):
			count[i]+=1
	
	l = []
	
	for i in range(0,len(c)):
	    if count[i]>minsup*len(d):
		l.append(c[i])
		#print c[i]
		
	if (r>1 and len(l)):
	    ans = ans + l
	    print 'FREQUENT-ITEMS ', r, ' ', len(l)
	    
	
	r += 1
	
    return ans
    
    
def getSupport(l):
    global d
    count = 0
    for j in range(0,len(d)):
	flag = 1
	for k in range(0,len(l)):
	    if (d[j][l[k]]==0):
		flag = 0
	if (flag == 1):
	    count+=1
    return count
    
    
    
def prtRiskness(l,risk,total,part):
    print 'support: ', total,
    print 'score: ', round(total/part,2),
    for i in range(0,len(l)):
	print features[l[i]-1], ' ',
    print '=> ',
    if risk==0:
	print 'High Riskiness'
    else:
	print 'Low Riskiness'
	    
def ruleGeneration(l,minconf):
    global d
    length = len(l[0])
    count = 0
    ans = []
    for i in range(0,len(l)):
	total = getSupport(l[i])
	if length!=len(l[i]):
	    print 'ASSOCIATION-RULES ', length, ' ', count
	    length+=1
	    count = 0
	for j in range(0,len(l[i])):
	    temp = []
	    for k in range(0,len(l[i])):
		if k!=j:
		    temp.append(l[i][k])
	    part = getSupport(temp)
	    if float(total)/float(part)>minconf:
		#print float(total)/float(part), ' ', temp, '=>', l[i][j] 
		#if l[i][j]==0 or l[i][j]==61:
		    #prtRiskness(temp, l[i][j],float(total),float(part))    		    
		temp.append(l[i][j])
		ans.append(temp)
		count += 1
		
    print 'ASSOCIATION-RULES ', length, ' ', count
    return ans
	
def apriori(minsup, minconf):
    l = frequentItemGeneration(minsup)
    print 'TOTAL FREQUENT-ITEMS ', len(l)
    r = ruleGeneration(l,minconf)
    print 'TOTAL ASSOCIATION-ITEMS ', len(r)
    return r
	
	
def main():
    if (len(sys.argv) > 3):
	trainfile = sys.argv[1]
	minsup = float(sys.argv[2])
	minconf = float(sys.argv[3])
	importTrainData(trainfile)
	apriori(minsup, minconf)
    else:
	print "Not Enough Arguments! format: python regression-tree.py train-set test-set.dat"
	
    
main()
