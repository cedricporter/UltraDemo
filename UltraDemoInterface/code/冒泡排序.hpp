int bubbleSort( int* watchedArray, int watchedSize )
{
	int i, j;
	i = 0; 
	while ( i < watchedSize )
	{
		j = 0;
		while ( j < watchedSize - i - 1 )
		{
			if ( watchedArray[ j ] > watchedArray[ j + 1 ] )
			{
				int temp;
				temp = watchedArray[ j ];
				watchedArray[ j ] = watchedArray[ j + 1 ];
				watchedArray[ j + 1 ] = temp;
			}
			j = j + 1;
		}
		i = i + 1;
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

	bubbleSort( &a[ 0 ], 10 );

	i = 0;
	while ( i < 10 )
	{
		write a[ i ];
		write 1;
		i = i + 1;
	}

	return 0;
}