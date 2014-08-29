//test for global
int a = 10;

void add(){
  a = a+1;
}

void sub(){
  a = a-1;
}

void div(){
  a = a/2;
}


int main()
{
  add();
  printf("%d\n", a);
  sub();
  printf("%d\n", a);
  div();
  printf("%d\n", a);
    return 0;
}
