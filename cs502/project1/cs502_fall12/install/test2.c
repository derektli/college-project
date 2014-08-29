
float g1;
float g2 = 1.1;
int g3 = 1;
char c = 'c';
int superman[50][11];


struct ttt {
  int k1;
  float k2;
};

struct ttt glob2;

float add(float a, float b){

    int x,y,i;
    while (a>1) {
	  printf("Please input 1st value: ");
	  
	  scanf("%f", &x);
	  if (x > 10) break;
	  printf("Please input 2nd value: ");
	  scanf("%f", &y);
	  printf("Values BEFORE 'swap' %f, %f\n", x, y);
	  if (y > 10) break;
	  swap(&x, &y);  
	  printf("Values AFTER  'swap' %f, %f\n", x, y);
	  i++;
	  ++i;
	  i--;
	  --i;
	}
		
	return b;
}

int sub(float a, float b){
  return a-b;
}

char other(char a, char b){

  int x = 0;

  switch(sub(1,2)){
  case 0:
	printf("zeor\n");
	printf("zeor\n");
	printf("zeor\n");
        break;
  case 1:printf("one\n");
    break;
  case 2:printf("two\n");
    break;
  default:
    break;
  }


  return a;

}

int boolcal(int a, int b, int c) {    
  return ((a>b) && ((a>=c) || (b<=c)));  
}

void test_func()
{
    g1 = 1.1 / g2;
    if (g1>g2)
    {
	int i = (1 + g1)*2+2;
	int j = 7;
	other('1', '2');
    }
    else
    {
	int i = (1 + g1)*2+2;
	int j = 7;
	other('1', '2');
    }    
    return;
}

main()
{

    float a[2][2][2] = {{{2,1.1},{1,2}},{{7,7},{8,9}}};
    char str[10] = "12345";
    
    printf("Hello World\n");
    g1 = 10.5;
    g2 = 2.0;

    printf("g1+g2 = %f\n", add(g1,g2));
    printf("g1-g2 = %f\n", sub(g1,g2));
    printf("other %c\n", other('0','1'));
    printf("boolcal %d\n", boolcal((g1+2)*g2, 2, 3));

    return 1;
}

