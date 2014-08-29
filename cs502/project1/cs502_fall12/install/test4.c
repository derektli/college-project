main()
{
        float   x, y;
	int i = 0;
	while (i < 5) {
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
	}
        return 0;
}

void swap(float *x, float *y)
{
        float   t;

        t = *x; 
	
        *x = *y;
        *y = t;
        printf("Values WITHIN 'swap' %f, %f\n", *x, *y);
}
