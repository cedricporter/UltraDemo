int Partition (int * watchedArray, int p, int q, int* ip ,int watchedSize)
{
	int x, i, j, temp;
	x = watchedArray[p];
	i = p;
	j = p + 1;
	while ( j <= q )
	{
		if ( watchedArray[j] <= x )
		{
			i = i + 1;
			temp = watchedArray[i];
			watchedArray[i] = watchedArray[j];
			watchedArray[j] = temp;
		}
		j = j + 1;
	}
	temp =watchedArray[p];
	watchedArray[p] = watchedArray[i];
	watchedArray[i] =temp;
	ip[0] = i;
	return 0;
}
int QuickSort(int * a, int p, int r,int size )
{
	if ( p < r)
	{
		int q, argu;
		Partition( &a[0], p , r, &q, size );
		argu = q - 1;
		QuickSort(&a[0], p ,argu , size );
		argu = q + 1;
		QuickSort(&a[0], argu, r , size );
	}
	return 0;
}

int main()
{	
	int a[ 10 ];

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

	QuickSort(&a[0], 0 , 9 ,10);

	i = 0;
	while ( i < 10 )
	{
		write a[ i ];
		write 1;
		i = i + 1;
	}

	return 0;
}