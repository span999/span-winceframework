/*
	refer
http://forums.udacity.com/questions/1021647/kalman-filter-c-code-implementation-how-to-tune-required-variables

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



typedef struct {
    float x[2];      // initial state (location and velocity)
    float P[2][2];   // initial uncertainty
    float u[2];      // external motion        // For Prediction
    float F[2][2];   // next state function    // For Prediction
    float H[2];      // measurement function
    float R[1];      // measurement uncertainty
    float I[2][2];   // identity matrix
} kalman_state;

kalman_state kalman_init()
{
    kalman_state result;

    // First is position and another is velocity
    // Consider [0.0f
    //           0.0f]; 
    result.x[0] = 0.0f;
    result.x[1] = 0.0f;

    // Consider [[1000.0f     0.0f]
    //           [   0.0f  1000.0f]];
    result.P[0][0] = 1000.0f; result.P[0][1] =    0.0f;
    result.P[1][0] =     0.0f; result.P[1][1] = 1000.0f;

    // Consider [0.0f
    //           0.0f];
    result.u[0] = 0.0f;
    result.u[1] = 0.0f;

    // Consider [[1.0f, 1.0f]
    //           [0.0f, 1.0f]];  
    result.F[0][0] = 1.0f; result.F[0][1] = 1.0f;
    result.F[1][0] = 0.0f; result.F[1][1] = 1.0f;

    // Consider [1.0f, 0.0f];
    result.H[0]    = 1.0f; result.H[1] = 0.0f;

    ///result.R[0]    = 1.0f;  //The RAW value is always flickering by? // Consider [1.0f];
	result.R[0]    = 1000.0f;

    // Consider [[1.0f, 0.0f]
    //           [0.0f, 1.0f]];
    result.I[0][0] = 1.0f; result.I[0][1] = 0.0f;
    result.I[1][0] = 0.0f; result.I[1][1] = 1.0f;

    return result;
}

void kalman_update(kalman_state* state, float measurement)
{
    // y = Z - ( H * x );
    // Z - (H0*x0 + H1*x1)
    float y = (float)measurement - ( state->H[0]*state->x[0] + state->H[1]*state->x[1] ) ;

    //S = H * P * ( H' ) + R;
    //    ( [H0 H1] * [P00 P01  * [H0  ) + R
    //                 P10 P11]    H1]
    float S = state->H[0]*state->H[0]*state->P[0][0] + state->H[0]*state->H[1]*(state->P[0][1]+state->P[1][0]) + state->P[1][1] * state->H[1]*state->H[1] + state->R[0];

    //K = P * ( H' ) / S; // or P* H'*inv(S) 
    float K[2]; 
    //Consider [K0
    //          K1]
    // ([P00 P01  *  [H0  ) / S
    //   P10 P11]     H1]
    K[0] = state->P[0][0]*state->H[0]/S+state->P[0][1]*state->H[1]/S;
    K[1] = state->P[1][0]*state->H[1]/S+state->P[1][1]*state->H[1]/S;

    //x = x + ( K * y );
    //  ([x0  + [K0 ) * y
    //    x1]    K1]
    state->x[0] = state->x[0] + K[0] * y;
    state->x[1] = state->x[1] + K[1] * y;

    //P = ( I - ( K * H ) ) * P;
    //     [I00 I01  - [K0  * [H0 H1] * [P00 P01
    //      I10 I11]    K1]              P10 P11]
    state->P[0][0] = ( ( state->I[0][0] - K[0]*state->H[0] ) * state->P[0][0] ) + ( ( state->I[0][1] - K[0]*state->H[1] ) * state->P[1][0] );
    state->P[0][1] = ( ( state->I[0][0] - K[0]*state->H[0] ) * state->P[0][1] ) + ( ( state->I[0][1] - K[0]*state->H[1] ) * state->P[1][1] );
    state->P[1][0] = ( ( state->I[1][0] - K[1]*state->H[0] ) * state->P[0][0] ) + ( ( state->I[1][1] - K[1]*state->H[1] ) * state->P[1][0] );
    state->P[1][1] = ( ( state->I[1][1] - K[1]*state->H[1] ) * state->P[0][1] ) + ( ( state->I[1][1] - K[1]*state->H[1] ) * state->P[1][1] );

}

void kalman_predict(kalman_state* state)
{
    //state->x = state->F*state->x + state->u ;
    // [F00 F01  * [x0  + [u0
    //  F10 F11]    x1]    u1]
    state->x[0] = state->F[0][0]*state->x[0] + state->F[0][1]*state->x[1] + state->u[0] ;
    state->x[1] = state->F[1][0]*state->x[0] + state->F[1][1]*state->x[1] + state->u[1] ;

    //state->P = state->F*state->P*state->F'
    // [F00 F01  * [P00 P01  * [F00 F10
    //  F10 F11]    P10 P11]    F01 F11]
    state->P[0][0] = state->F[0][0] * ( state->F[0][0]*state->P[0][0] + state->F[0][1]*state->P[1][0]) + state->F[0][1] * ( state->F[0][0]*state->P[0][1] + state->F[0][1]*state->P[1][1] ) ;
    state->P[0][1] = state->F[1][0] * ( state->F[0][0]*state->P[0][0] + state->F[0][1]*state->P[1][0]) + state->F[1][1] * ( state->F[0][0]*state->P[0][1] + state->F[0][1]*state->P[1][1] ) ;
    state->P[1][0] = state->F[0][0] * ( state->F[1][0]*state->P[0][0] + state->F[1][1]*state->P[1][0]) + state->F[0][1] * ( state->F[1][0]*state->P[0][1] + state->F[1][1]*state->P[1][1] ) ;
    state->P[1][1] = state->F[1][0] * ( state->F[1][0]*state->P[0][0] + state->F[1][1]*state->P[1][0]) + state->F[1][1] * ( state->F[1][0]*state->P[0][1] + state->F[1][1]*state->P[1][1] ) ;

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

    kalman_state Kalman = kalman_init();
	printf("start...\r\n");
	printf("raw data / Kalman data\r\n");
    for(i=0;i<40;i++)
    {
        // sensor value retrieval
		SensorRAWValue = fakedata[i];
		printf("%8d",SensorRAWValue);
        kalman_update(&Kalman,SensorRAWValue);
        kalman_predict(&Kalman);
        //TODO:: Please write your application code to use Kalman.x[0] and/or Kalman.x[1]
		printf("   %09f\r\n",Kalman.x[0]);
		///printf("Kalman data=%f\r\n",Kalman.x[1]);
    }
	printf("end...\r\n");
}


