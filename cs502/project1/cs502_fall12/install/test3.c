int glob1;

struct qqq
{
  int k3;
  struct qqq* next;
};

struct ttt {
  struct qqq k0;
  int k1;
  float k2;
};


struct qqq glob2;

main()
{
int i[2]={1,2},k = 10;
int a[2][2];
a[1][2] = 1;
//struct ttt h={3,4.0};
struct ttt h;
struct qqq * q;
q->next->k3 = 1;

h.k0.k3 = 1;
h.k1 = h.k1 + k;

char *u="string";

if ( glob1 > 0 )
	glob1 = 0;
else
	glob1 = 1;
while ( k ) {
	h.k1 = h.k1 + k;
	if ( h.k1 > 10 ) {
		h.k1 = 0;
		h.k2 = 1.0;
	}
	k = k-1;
}
k=i[1]+t(1,3);
printf("glob2.k1=%d, k=%d\n",glob2.k3,k);

}

int t(int k, int j)
{
  return glob1+k+j;
}
