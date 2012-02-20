int Init ( int * watchedArray, int watchedHeight, int watchedWidth)
{
	int i,j,k;

	k = 0;
	i = 3;
	j = 6;
	while ( k < 7)
	{
		watchedArray[ i*watchedWidth + j] = 1;
		i = i + 1;
		j = j - 1;
		k = k + 1;
	}

	i = 4;
	while ( i <= 15)
	{
		watchedArray[ 5*watchedWidth + i ] =1;
		watchedArray[ 5*watchedWidth + i + 1 ] =1;
		watchedArray[ 5*watchedWidth + i + 2] =1;
		watchedArray[ 5*watchedWidth + i + 3 ] =1;

		i = i + 4;
	}

	i = 3;
	while ( i <= 18)
	{
		watchedArray[ 8*watchedWidth + i ] = 1;
		watchedArray[ 8*watchedWidth + i + 1] = 1;
		watchedArray[ 8*watchedWidth + i + 2] = 1;
		watchedArray[ 8*watchedWidth + i + 3 ] = 1;

		i = i + 4;
	}

	i = 1;
	while ( i <= 18)
	{
		watchedArray[ i*watchedWidth + 9 ] = 1;
		watchedArray[ ( i + 1 )*watchedWidth + 9 ] = 1;
		watchedArray[ ( i + 2 )*watchedWidth + 9 ] = 1;

		i = i + 3;
	}

	i = 10;
	while ( i <= 19)
	{
		watchedArray[ i*watchedWidth + 15 ] = 1;
		watchedArray[ (i+1)*watchedWidth + 15 ] = 1;
		watchedArray[ (i+2)*watchedWidth + 15 ] = 1;
		watchedArray[ (i+3)*watchedWidth + 15 ] = 1;
		watchedArray[ (i+4)*watchedWidth + 15 ] = 1;

		i = i + 5;
	}


	watchedArray[ 10*watchedWidth + 15] = 1;
	watchedArray[ 10*watchedWidth + 16] = 1;
	watchedArray[ 11*watchedWidth + 17] = 1;
	watchedArray[ 12*watchedWidth + 18] = 1;
	watchedArray[ 13*watchedWidth + 18] = 1;
	watchedArray[ 13*watchedWidth + 17] = 1;
	watchedArray[ 14*watchedWidth + 16] = 1;
	watchedArray[ 15*watchedWidth + 17] = 1;
	watchedArray[ 16*watchedWidth + 18] = 1;
	watchedArray[ 17*watchedWidth + 18] = 1;
	watchedArray[ 18*watchedWidth + 17] = 1;
	watchedArray[ 19*watchedWidth + 16] = 1;

	watchedArray[ 19*watchedWidth + 9] = 4;

	return 0;
}


int go( int * watchedArray, int watchedHeight,int watchedWidth, int x, int y, int * win,int SIZE )
{

	if ( 0 <= x && x < SIZE && 0 <= y && y < SIZE && (*win==0))
	{
		if ( watchedArray[ x*watchedWidth + y ] != 1 && watchedArray[ x*watchedWidth + y ] != 999 && watchedArray[ x*watchedWidth + y ] != 2 )  
		{

			watchedArray[ x*watchedWidth + y] = 2;
			if ( (x == SIZE-1 ) && (y == SIZE/2 -1) ) 
			{
				win[0] = 1;
				watchedArray[ x * watchedWidth + y ] = 3;
				return 0;
			}

			watchedArray[ x*watchedWidth +  y ]  = 2;

			go( &watchedArray[0],watchedHeight,watchedWidth ,x, y+1, &win[0], SIZE); 
			go( &watchedArray[0],watchedHeight,watchedWidth ,x+1, y, &win[0], SIZE);
			go( &watchedArray[0],watchedHeight,watchedWidth ,x, y-1, &win[0], SIZE ); 
			go( &watchedArray[0],watchedHeight,watchedWidth ,x-1, y, &win[0], SIZE); 

			watchedArray[ x*watchedWidth +  y ] = 0;

		}
		else if (watchedArray[ x*watchedWidth +  y ] == 1)
		{
			int temp;
			temp = watchedArray[ x*watchedWidth +  y ]; 
			watchedArray[ x*watchedWidth +  y ] = 777;
			watchedArray[ x*watchedWidth +  y ] = temp;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

int main()
{
	int map[ 20 ][ 20 ];
	int size,i,win;
	size = 20;
	win = 0;
	Init(&map[0][0],20,20);
	go( &map[0][0],20,20,0, 0, &win, 20);
	return 0;
}
