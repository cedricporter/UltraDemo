int print( int watchedStart, int watchedGoal, int watchedn )
{
	write watchedStart;
	write 1;
	write watchedGoal;
	write 0;
	return 0;
}

int f( int n, int start, int goal, int tmp, int max )
{
	if ( n == 0 )
	{
		return 0;
	}

	f( n - 1, start, tmp, goal, max );
	print( start, goal, max );
	f( n - 1, tmp, goal, start, max );

	return 0;
}

int main()
{	
	f( 3, 1, 3, 2, 3 );


	return 0;
}