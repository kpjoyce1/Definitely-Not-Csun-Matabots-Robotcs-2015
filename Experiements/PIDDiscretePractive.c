float Ts = 100; //sample time in ms
float N = 1; //filter coeffiecent

float Kp = 1;
float Ki = 1;
float Kd = 1;


float a0 = 1 + N * Ts;
float a1 = -(2 + N*Ts);
float a2 = 1;
float b0 = Kp*(1 + N*Ts) + Ki*Ts*(1 + N*Ts) + Kd*N;
float b1 = -(Kp*(2+N *Ts) + Ki*Ts+ 2*Kd*N);
float b2 = Kp + Kd * N;

float u0,u1,u2,e0,e1,e2;

float target, reading;

bool readReady()
{

	if(time1[T1] >= Ts)
	{
		//get reading
		//reset timer
		time1[T1] = 0;
		return true;
	}
	return false;
}

void Control()
{
	if(readReady())
	{
		u2 = u1;
		u1 = u0;
		e2 = e1;
		e1 = e0;

		//e0 = target - reading;
		u0 = -a1/a0*u1-a2/a0*u2+b0/a0*e0+b1/a0*e1+b2/a0*e2;

		//check saturation

		//apply output
	}
}

task main()
{




}
