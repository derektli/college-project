#Name: Tingjun Li
#Email: li400@purdue.edu

import sys

features = ["alfa-romero", "audi", "bmw", "chevrolet", "dodge", "honda", "isuzu", "jaguar", 
	    "mazda", "mercedes-benz", "mercury", "mitsubishi", "nissan", "peugot", "plymouth", 
	    "porsche", "renault", "saab", "subaru", "toyota", "volkswagen", "volvo", "diesel",
	    "gas", "std", "turbo", "four", "two", "hardtop", "wagon", "sedan", "hatchback", 
	    "convertible", "4wd", "fwd", "rwd", "front", "rear", "dohc", "dohcv", ",l,", "ohc", 
	    "ohcf", "ohcv", "rotor", "eight", "five", "four", "six", "three", "twelve", "two",
	    "1bbl", "2bbl", "4bbl", "idi", "mfi", "mpfi", "spdi", "spfi"]

class Node:
    def __init__(self, datalist):
	self.left = None
	self.right = None
	self.feature = None
	self.avgPred = None
	self.datalist = datalist

    def setFeature(self, fid):
	self.feature = fid
	leftnode = list()
	rightnode = list()
	
	a = self.datalist
	for i in range(0, len(a)):
	    #print a[i]
	    if a[i][fid]==1:
		leftnode.append(a[i])
	    else:
		rightnode.append(a[i])
	self.left = Node(leftnode)
	self.right = Node(rightnode)
	



#define a 2d array data to store training data   
data = [[]]
root = Node(data)
stop_threshold = 0.075
numFeature = 0

for i in range(0,220):
    data.append([])
    for j in range(0,len(features)+1):
	data[i].append(0)

def calculateVariance(dlist):
    s2 = 0
    s = 0
    if (len(dlist)==0):
	return 0
    for i in range(0,len(dlist)):
	e = float(dlist[i][len(features)])
        s += e
        s2 += e * e
    score = (s2-(s*s)/len(dlist))/len(dlist)
    return score
    
def splitData(dlist, fid):
    ylist = list()
    nlist = list()
    for i in range(0,len(dlist)):
	if dlist[i][fid]==1:
	    ylist.append(dlist[i])
	else:
	    nlist.append(dlist[i])
    return (ylist, nlist)

def calculateFeatureScore(dlist, fid):
    origVar = calculateVariance(dlist)
    [ypart, npart] = splitData(dlist,fid)
    probYes = float(len(ypart))/float(len(dlist))
    probNo = float(len(npart))/float(len(dlist))
    score = probYes * calculateVariance(ypart)
    score += probNo * calculateVariance(npart)
    score = origVar - score
    
    return score

def calculateAvgClass(dlist):
    s = 0
    if (len(dlist)==0):
	return 0
    for i in range(0,len(dlist)):
        e = float(dlist[i][len(features)])
        s += e
    ans = s/len(dlist)
    return ans
    

#expanding tree
def growTree(node):
  global numFeature
  max_score = 0
  max_feature = -1
  for i in range(0,len(features)):
	curr_score = calculateFeatureScore(node.datalist, i)
	#print curr_score
	if curr_score > max_score and curr_score > stop_threshold:
	  #print max_score
	  max_feature = i
	  max_score = curr_score
  
  if max_feature == -1:
	avgPred = calculateAvgClass(node.datalist)
	node.avgPred = avgPred
	#print "=================", avgPred
	return
  else:
	node.setFeature(max_feature)
	numFeature+=1
	#print "choose", max_feature
	leftNode = node.left
	rightNode = node.right
	
	if len(leftNode.datalist)>0:
	    growTree(leftNode)
	if len(rightNode.datalist)>0:
	    growTree(rightNode)
	

def importTrainData(filename):
    fin = open(filename,"r")
    i=0
    for line in fin:
	j=0
	for str in features:
	    if line.find(str)!=-1:
		data[i][j]=1
	    j=j+1
	a = line.strip().split(',')
	data[i][j]=a[0]
	i=i+1
    fin.close()
    root.datalist = data[:i]
    growTree(root)
    

def applyTree(node, tlist):
    if node.left == None and node.right == None:
	return node.avgPred
    else:
	fid = node.feature
	if tlist[fid]==1:
	    return applyTree(node.left, tlist)
	else:
	    return applyTree(node.right, tlist)
    return 0
    
    
    
def evaluate(filename):
    fin = open(filename,"r")
    tlist=[]
    for i in range(0,len(features)+1):
	tlist.append(0)
    score = 0
    i=0
    for line in fin:
	j=0
	for str in features:
	    if line.find(str)!=-1:
		tlist[j]=1
	    else:
		tlist[j]=0
	    j=j+1
	a = line.strip().split(',')
	tlist[j]=a[0]
	expY = applyTree(root,tlist)
	score+=(expY-float(a[0]))*(expY-float(a[0]))
	i=i+1
    score = score/i
    print 'Number of Feature splits={}'.format(numFeature)
    print 'Threshold={}'.format(stop_threshold)
    print 'SQUARED LOSS={}'.format(score)
    fin.close()
    return score
    
def main():
    if (len(sys.argv) > 2):
	trainfile = sys.argv[1]
	testfile = sys.argv[2]
	importTrainData(trainfile)
	evaluate(testfile)
    else: 
	print "Not Enough Arguments! format: python regression-tree.py train-set test-set.dat"
	
    
main()