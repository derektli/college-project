int glob1;
struct qqq {
    int k3;
    struct qqq* next;
};
struct qqq glob2;

int main()
{
    int i[2] = {1, 2};
    int k = 10;
    int a[2][2];
    a[1][2] = 1;
    struct ttt {
        struct qqq k0;
        int k1;
        float k2;
    };
    struct ttt h;
    struct qqq* q;
    (*(*q).next).k3 = 1;
    h.k0.k3 = 1;
    h.k1 = h.k1 + k;
    char* u = "string";
    if (glob1>0)
        glob1 = 0;
    else
        glob1 = 1;
        
        
        
    goto D1721;
    D1720:
    h.k1 = h.k1 + k;
    if (h.k1>10)
    {
        h.k1 = 0;
        h.k2 = 1.0e+0;
    }
    k = k + -1;
    D1721:
    if (k != 0)
        goto D1720;
    else
        goto D1722;
        
        
    D1722:
    k = i[1] + t(1, 3);
    printf("glob2.k1=%d, k=%d\n", glob2.k3, k);
}

int t(int k, int j)
{
    return glob1 + k + j;
}

