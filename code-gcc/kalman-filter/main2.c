/*
	refer
http://iamnickng.blogspot.tw/2012/10/implementing-kalman-filter-with-c.html
http://interactive-matter.eu/blog/2009/12/18/filtering-sensor-data-with-a-kalman-filter/

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



typedef struct {
  double q; //process noise covariance (v)
  double r; //measurement noise covariance (v)
  double x; //value
  double p; //estimation error covariance
  double k; //kalman gain 
} kalman_state;

kalman_state kalman_init(double q, double r, double p, double intial_value)
{
  kalman_state result;
  result.q = q;
  result.r = r;
  result.p = p;
  result.x = intial_value;

  return result;
}

void
kalman_update(kalman_state* state, double measurement)
{
  //prediction update
  //omit x = x
  state->p = state->p + state->q;

  //measurement update
  state->k = state->p / (state->p + state->r);
  state->x = state->x + state->k * (measurement - state->x);
  state->p = (1 - state->k) * state->p;
}



unsigned int fakedata[40] = {
	800,
	800,
	792,
	792,
	790,
	790,
	791,
	791,
	790,
	790,
	790,
	790,
	785,
	785,
	788,
	785,
	785,
	787,
	787,
	785,
	785,
	784,
	784,
	782,
	782+5,
	781,
	781,
	780+5,
	780,
	782,
	782,
	781,
	778,
	778,
	776,
	778,
	778,
	779,
	779,
	781
}; 

int main(void)
{
    unsigned int SensorRAWValue = 0;
	unsigned int i = 0;

    ///kalman_state Kalman = kalman_init(1.0, 1.0, 1.0, 800.0);
	kalman_state Kalman = kalman_init(1.0, 10.0, 1.0, 800.0);
	printf("start...\r\n");
	printf("raw data, / Kalman data\r\n");
    for(i=0;i<40;i++)
    {
        // sensor value retrieval
		SensorRAWValue = fakedata[i];
		printf("%8d,",SensorRAWValue);
        kalman_update(&Kalman,SensorRAWValue);

        //TODO:: Please write your application code to use Kalman.x[0] and/or Kalman.x[1]
		printf("   %09f\r\n",Kalman.x);
		///printf("Kalman data=%f\r\n",Kalman.x[1]);
    }
	printf("end...\r\n");
}


