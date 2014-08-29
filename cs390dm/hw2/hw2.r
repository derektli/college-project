#Name: Tingjun Li
#Email: li400@purdue.edu
#1
data = read.table("imports-85.data",sep=",",na.strings="?")

names = c("symboling","normalized-losses","make","fuel-type","aspiration","num-of-doors","body-style","drive-wheels","engine-location","wheel-base","length","width","height","curb-weight","engine-type","num-of-cylinders","engine-size","fuel-system","bore","stroke","compression-ratio","horsepower","peak-rpm","city-mpg","highway-mpg","price")

print(names)

#1.2
summary(data)
#summary(na.omit(data))

#1.3

maxgap=0
x=0
 
for (i in 1:ncol(data))
{
  k=1
  while(is.na(data[k,i])) k=k+1

  if (is.numeric(data[k,i]))
  {
    diff = max(na.omit(data[,i])) -  min(na.omit(data[,i]))
    if (diff>maxgap) 
    {
      maxgap = diff
      x = i
    }
  }
}

hist(data[,x],main=names[x])

#1.4
maxgap=0
x=0
 
for (i in 1:ncol(data))
{
  k=1
  while(is.na(data[k,i])) k=k+1

  if (!is.numeric(data[k,i]))
  {
    diff = length(table(data[,i]))
    if (diff>maxgap) 
    {
      maxgap = diff
      x = i
    }
  }
}
t = as.data.frame(table(na.omit(data[,x])))
barplot(t$Freq,names.arg=t$Var1,main=names[x], las=2)

#2
#find the 10 col
j=1
a = 1:10
for (i in 1:ncol(data))
{
  if (is.numeric(data[1,i]) && !is.na(max(data[,i])))
  {
     a[j]=i
     j=j+1
  }
}
#calculate the cor and store them in cm[,]
c = 1:100
cm = matrix(c,ncol=10,byrow=T)

for (i in 1:10)
{
  for (j in 1:10)
  {
    cm[i,j]=cor(data[,a[i]],data[,a[j]])
  }
}
cm
#find the largest 3 and plot them
findmax=function()
{
  temp=0
  x=0
  y=0
  for (i in 1:10)
  {
    for (j in 1:10)
    {
      if (cm[i,j]>temp && i!=j)
      {
	x=i
	y=j
	temp=cm[i,j]
      }
    }
  }
  cat(x," ",y,"\n")
  ans=c(x,y)
  return (ans)
}

findmin=function()
{
  temp=0
  x=0
  y=0
  for (i in 1:10)
  {
    for (j in 1:10)
    {
      if (cm[i,j]<temp && i!=j)
      {
	x=i
	y=j
	temp=cm[i,j]
      }
    }
  }
  cat(x," ",y,"\n")
  ans=c(x,y)
  return (ans)
}

plotcol=function(x,y)
{
  #table(data[,x])
  plot(data[,x],data[,y],xlab=names[x],ylab=names[y],main="Problem 2")
  return (1)
}

o=findmax()
cm[o[1],o[2]]=0
cm[o[2],o[1]]=0
plotcol(a[o[1]],a[o[2]])

o=findmax()
cm[o[1],o[2]]=0
cm[o[2],o[1]]=0
plotcol(a[o[1]],a[o[2]])

o=findmax()
plotcol(a[o[1]],a[o[2]])

o=findmin()
cm[o[1],o[2]]=0
cm[o[2],o[1]]=0
plotcol(a[o[1]],a[o[2]])

o=findmin()
cm[o[1],o[2]]=0
cm[o[2],o[1]]=0
plotcol(a[o[1]],a[o[2]])

o=findmin()
plotcol(a[o[1]],a[o[2]])


#3

drawbox=function(x)
{
  p=subset(data, data$V26 != "?")
  boxplot(as.double(as.vector(p$V26))~p[,x], p,ylab=names[26],xlab=names[x],main="Problem 3")
  return (1)
}


#find attribute with 5 or fewer values
for (i in 1:ncol(data))
{
  k=1
  while(is.na(data[k,i])) k=k+1

  if (!is.numeric(data[k,i]))
  {
    t = table(na.omit(data[,i]))
    if (length(t)<=5) 
    {
      drawbox(i)
    }
  }
}



#4
# 
# #find the 10 col
# j=1
# a = 1:16
# for (i in 1:ncol(data))
# {
#   if (is.numeric(data[1,i]))
#   {
#      a[j]=i
#      j=j+1
#   }
# }
# 
# #calculate the cor and store them in cm[,]
# c = 1:256
# cm = matrix(c,ncol=16,byrow=T)
# 
# d =na.omit(data)
# for (i in 1:16)
# {
#   for (j in 1:16)
#   {
#     
#     cm[i,j]=cor(d[,a[i]], d[,a[j]])
#   }
# }
# cm
# temp=0
# y=0
# for (j in 1:16)
# {
#   if (abs(cm[1,j])>temp && i!=j)
#   {
#       y=j
#       temp=cm[1,j]
#    }
# }
# plotcol(a[1],a[j])
# 
# temp=0
# y=0
# for (j in 1:16)
# {
#   if (abs(cm[2,j])>temp && i!=j)
#   {
#       y=j
#       temp=cm[2,j]
#    }
# }
# plotcol(a[2],a[j])

drawbox2=function(x)
{
  p=subset(data, data$V26 != "?")
  boxplot(as.double(as.vector(p$V1))~p[,x], p,ylab=names[1],xlab=names[x],main="Problem 4")
  return (1)
}

drawbox3=function(x)
{
  p=subset(data, data$V26 != "?")
  boxplot(as.double(as.vector(p$V2))~p[,x], p,ylab=names[2],xlab=names[x],main="Problem 4")
  return (1)
}

# 
# #find attribute with 5 or fewer values
# for (i in 1:ncol(data))
# {
#   k=1
#   while(is.na(data[k,i])) k=k+1
# 
#   if (!is.numeric(data[k,i]))
#   {
#     t = table(na.omit(data[,i]))
#     drawbox2(i)
#   }
# }
# 
# for (i in 1:ncol(data))
# {
#   k=1
#   while(is.na(data[k,i])) k=k+1
# 
#   if (!is.numeric(data[k,i]))
#   {
#     t = table(na.omit(data[,i]))
#     drawbox3(i)
#   }
# }

drawbox2(7)
drawbox3(6)
