int RadixSort( int * watchedArray, int * b, int * cnt,int watchedSize, int k, int r)
{
    int i,j,temp,temp1,temp2,temp3;
    int rtok;
    i = 0;
    rtok = 1;
    while ( i < k)
    {
        j = 0;
        while ( j < r) 
        {
            cnt[j] = 0;
            j = j + 1;
        }
        j = 0;
        while ( j < watchedSize)
        {
            temp1 = watchedArray[j]/rtok;
            temp2 = temp1/r;
            temp = temp1 - temp2*r;
            cnt[temp] = cnt[temp] + 1;
            j = j + 1;
        }

        j = 1;
        while ( j < r)
        {
            temp = j - 1;
            cnt[j] = cnt [temp] + cnt[j];
            j = j + 1;
        }
        j = watchedSize - 1;
        while ( j >= 0)
        {
            temp1 = watchedArray[j]/rtok;
            temp2 = temp1/r;
            temp3 = temp1 - temp2*r;
            cnt[temp3] = cnt[temp3] - 1;
            temp = cnt[temp3];
            b[temp] = watchedArray[j];
            j = j - 1;
        }
        j = 0;
        while ( j < watchedSize )
        {
            watchedArray[j] = b[j];
            j = j + 1;
        }
        i = i + 1;
        rtok = rtok * r;
    }
    return 0;
}
int main ()
{
    int a[ 10 ],b[10],cnt[10];
    int i;
    i = 0;
    while ( i < 10 )
    {
        a[ i ] = 50 - i * 2;
        i = i + 1;
    }
    a[ 5 ] = 542;
    a[ 2 ] = 1234;
    a[ 8 ] = 43;

    i = 0;
    while ( i < 10 )
    {
        write a[ i ];
        write 1;
        i = i + 1;
    }
    write 0;

    RadixSort(&a[0],&b[0],&cnt[0],10,4,10);
    i = 0;
    while ( i < 10 )
    {
        write a[ i ];
        write 1;
        i = i + 1;
    }

    return 0;
}




