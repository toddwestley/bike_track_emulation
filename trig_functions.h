double factorial_n(int n)
{	double answer;
	answer = 1.0f;
	while (n>1) 
		{answer = answer *n;
		 n = n -1;
	     }
	//printf("%d! %lf = \n",n,answer);     
	return answer; 
}
double x_to_the_n_power(float x, int n)
{	double answer;
	int old_n;
	old_n = n;
	answer = 1.0;
	while (n>=1)
		{	answer = answer*x;
			n = n -1;
		}
	//printf("%lf ^ %d == %lf\n",x,old_n,answer);
	return answer;
}
// from C.R.C. Standard Mathematical Tables p. 373
double cosine_todd (double angle) // angle in radians!!
{
	int i;
	int sign;
	double answer;
	answer = 1.0f;
	sign = 1;
	
	i = 2;
	while (i <= 16)
		{	sign = -1*sign;
			answer = answer+x_to_the_n_power(angle,i)/factorial_n(i);
			i = i +2;
		}
	return answer;
}
double sine_todd (double angle) // angle in radians!!
{
	int i;
	int sign;
	double answer;
	answer = 0.0f;
	sign = 1;
	
	i = 1;
	while (i <= 16)
		{	
			answer = answer+x_to_the_n_power(angle,i)/factorial_n(i);
			sign = -1*sign;
			i = i +2;
		}
	return answer;
}
double exp_todd (double x)
{	double change;
	int i;
	double answer;
	answer = 0.0;
	change = 9999999999999999999999999.9999999999999999999999;
	i = 0;
	while ( fabs(change)> 0.0000001)
			{change = x_to_the_n_power(x,i)/factorial_n(i);
			//printf("change == %lf\n",change);
			i = i +1;
			answer += change;	
			}
	return answer;
}
