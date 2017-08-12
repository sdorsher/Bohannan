// Test4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <math.h>


int _tmain(int argc, _TCHAR* argv[])
{
	//In this program you must keep all units positive except for dLA and dLH 
	// (including angular velocities):
	// L_L_0 is the max angle we should ever reach for LA, and similar for L_R_0. 
	// L1 and L2 are spring lengths at a given point/time and L1not and L2not 
	//	are there natural lengths.

	// Declare and define each variable in the program ( don't make it look like it
	//	was written in FORTRAN.

	float LA,	// angle of the arm with respect to the hub frame
		LH,		// angle of the hub with respect to the lab frame
		L_L_0, 
		L_R_0, 
		L1, 
		L2, 
		L1not,	// limit angle
		L2not,  // limit angle
		V, 
		epsilon=0, 	// what is epsilon?
					/*I think epsilon must be zero for the simulation*/, 
		dLA,	// increment of arm rotation
		dLH,	// increment of hub rotation
		Ia,		// Moment of inertia of the arm
		dt,		// time increment
		wh, 
		wa, 
		R, 
		F,
		Z, 
		Ih, 
		tow, 
		eta, 
		angle, 
		Work, 
		k, 
		r, 
		d;

	bool value=true;

	// TODO: Initialization code goes here.
	// Establish the run time values for the constants in code or by reading from a file
	// dLA = 0.0;
	// dLH = 0.0;
	// etc...

	// The code for writing to a file is fprintf(FILE *stream, const char *format, [data]);
	/*
	FILE *fp;
	if ((fp = fopen("testdata.csv","w"))==NULL)  // creates a file for writing
						// You may need to give the full pathname if you want to write to a USB stick.
	{
		printf("Cannot open file.\n");
		exit(1);
	}

	...
	
	fprintf(fp, "%g,%g\n", LH, LA);	// will print LH and LA in comma separated variable format for reading back in Excel
	
	...
	
	fclose(fp);		// To close the file at the end of the program run
	
	*/

	while(value)
	{
		// Place TODO: markers at points where you want to record the variables.
		// Do you need to keep track of cumulative time?

		//Basic calculations:
		LA=LA+dLA;
		LH=LH+dLH;

		
		//Force and work equations here.
		Work=(V*V/Z)*eta*dt;

		if(L1<L1not)
			F=k*(L2-L2not)*sqrt(r*r+d*d)*sin(LA)/L2;
		if(L2<L2not)
			F=k(L1-L1not)*sqrt(r*r+d*d)*sin(arccos((d*d-r*r)/(r*r+d*d))-LA)/L1;
		if((L1>L1not)&&(L2>L1not))
			F=abs(k*(L2-L2not)*sqrt(r*r+d*d)*sin(LA)/L2 - k(L1-L1not)*sqrt(r*r+d*d)*sin(arccos((r*r-d*d)/(r*r+d*d))-LA)/L1);
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// START OF FIRST CONDITION:
		/*THIS CONDITION ADJUST THE HUB IF LAMBDA GETS TO SMALL OR TOO LARGE:*/
		if(LA>L_L_0+epsilon)
		{
			//This condition should be fine:
			V=10;
			wh=dLH/dt;
			wa=dLA/dt;

			dLH=((1/2)*(wh*Z*Ih*Ia + sqrt(wh*wh*Z*Z*Ih*Ih*Ia*Ia - Ia*Ih*Z*Z*R*R*F*F*dt*dt - 2*Ia*Ia*Ih*Z*Z*dt*tow*abs(wh)+2*Ia*Ia*Ih*Z*dt*V*V*eta-2*Ia*Ia*Ih*Z*Z*dt*R*F*wh-2*Ia*Ia*Ih*Z*Z*dt*R*F*wa))*dt/(Ia*Ih*Z));
			dLA=((1/2)*(-2*dLH*Ia+2*dt*Ia*wh+2*dt*Ia*wa+R*F*dt*dt)/Ia);

			wh=dLH/dt;
			wa=dLA/dt;
		}
		if(LA<L_R_0-epsilon)
		{
			
			V=10;
			wh=-wh;//These two equations convert the velocities so that there sign is correct for the dLH and dLA equations below
			wa=-wa;

			dLH=-((1/2)*(wh*Z*Ih*Ia + sqrt(wh*wh*Z*Z*Ih*Ih*Ia*Ia - Ia*Ih*Z*Z*R*R*F*F*dt*dt - 2*Ia*Ia*Ih*Z*Z*dt*tow*abs(wh)+2*Ia*Ia*Ih*Z*dt*V*V*eta-2*Ia*Ia*Ih*Z*Z*dt*R*F*wh-2*Ia*Ia*Ih*Z*Z*dt*R*F*wa))*dt/(Ia*Ih*Z));
			dLA=-((1/2)*(-2*dLH*Ia + 2*dt*Ia*wh + 2*dt*Ia*wa+R*F*dt*dt)/Ia);

			wh=dLH/dt;
			wa=dLA/dt;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// END OF FIRST CONDITION:
		




		//we need to switch these conditions to relating kinetic energy to spring potential:




		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// START OF SECOND CONDITION:
		/*THESE CONDITIONS SHOULD BE FIEN PROVIDED LH STARTS AT ZERO AT THE BEGINNING OF THE PROGRAM: 
		WE SHOULD ADD A CONDITION LIKE ANGLE-LH>0 RATHER THAN JUST ANGLE>0: IN THE FUTURE*/
		/*THIS CONDITION OPERTATES THE ROBOT UNTIL IT IS TIME FOR THE HUB TO STOP AND LET THE ARM SWING PAST A/2:*/
		if((LA>L_R_0)&&(angle>0)&&(angle/2-LH>wh*wh*Ih/(2*(tow+F*R))&&(LH<angle)))
		{
			V=10;
			wh=dLH/dt;
			wa=dLA/dt;

			dLH=((1/2)*(wh*Z*Ih*Ia+sqrt(wh*wh*Z*Z*Ih*Ih*Ia*Ia-Ia*Ih*Z*Z*R*R*F*F*dt*dt-2*Ia*Ia*Ih*Z*Z*dt*tow*abs(wh)+2*Ia*Ia*Ih*Z*dt*V*V*eta-2*Ia*Ia*Ih*Z*Z*dt*R*F*wh-2*Ia*Ia*Ih*Z*Z*dt*R*F*wa))*dt/(Ia*Ih*Z));
			dLA=((1/2)*(-2*dLH*Ia+2*dt*Ia*wh+2*dt*Ia*wa+R*F*dt*dt)/Ia);

			wh=dLH/dt;
			wa=dLA/dt;
		}
		if((LA>L_R_0)&&(angle>0)&&(LH-angle/2>wh*wh*Ih/(2*(tow+F*R))&&(LH>angle)))
		{
			V=10;
			wh=-wh;//These two equations convert the velocities so that there sign is correct for the dLH and dLA equations below
			wa=-wa;

			dLH=-((1/2)*(wh*Z*Ih*Ia+sqrt(wh*wh*Z*Z*Ih*Ih*Ia*Ia-Ia*Ih*Z*Z*R*R*F*F*dt*dt-2*Ia*Ia*Ih*Z*Z*dt*tow*abs(wh)+2*Ia*Ia*Ih*Z*dt*V*V*eta-2*Ia*Ia*Ih*Z*Z*dt*R*F*wh-2*Ia*Ia*Ih*Z*Z*dt*R*F*wa))*dt/(Ia*Ih*Z));
			dLA=-((1/2)*(-2*dLH*Ia+2*dt*Ia*wh+2*dt*Ia*wa+R*F*dt*dt)/Ia);

			wh=dLH/dt;
			wa=dLA/dt;
		}




		if((LA<L_L_0)&&(angle<0)&&(LH-angle/2>wh*wh*Ih/(2*(tow+F*R)))&&(LH>angle))
		{
			V=10;
			wh=-wh;//These two equations convert the velocities so that there sign is correct for the dLH and dLA equations below
			wa=-wa;

			dLH=-((1/2)*(wh*Z*Ih*Ia+sqrt(wh*wh*Z*Z*Ih*Ih*Ia*Ia-Ia*Ih*Z*Z*R*R*F*F*dt*dt-2*Ia*Ia*Ih*Z*Z*dt*tow*abs(wh)+2*Ia*Ia*Ih*Z*dt*V*V*eta-2*Ia*Ia*Ih*Z*Z*dt*R*F*wh-2*Ia*Ia*Ih*Z*Z*dt*R*F*wa))*dt/(Ia*Ih*Z));
			dLA=-((1/2)*(-2*dLH*Ia+2*dt*Ia*wh+2*dt*Ia*wa+R*F*dt*dt)/Ia);

			wh=dLH/dt;
			wa=dLA/dt;
		}

		if((LA<L_L_0)&&(angle<0)&&(angle/2-LH>wh*wh*Ih/(2*(tow+F*R)))&&(LH<angle))
		{
			V=10;
			wh=dLH/dt;
			wa=dLA/dt;

			dLH=((1/2)*(wh*Z*Ih*Ia+sqrt(wh*wh*Z*Z*Ih*Ih*Ia*Ia-Ia*Ih*Z*Z*R*R*F*F*dt*dt-2*Ia*Ia*Ih*Z*Z*dt*tow*abs(wh)+2*Ia*Ia*Ih*Z*dt*V*V*eta-2*Ia*Ia*Ih*Z*Z*dt*R*F*wh-2*Ia*Ia*Ih*Z*Z*dt*R*F*wa))*dt/(Ia*Ih*Z));
			dLA=((1/2)*(-2*dLH*Ia+2*dt*Ia*wh+2*dt*Ia*wa+R*F*dt*dt)/Ia);

			wh=dLH/dt;
			wa=dLA/dt;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// END OF SECOND CONDITION:













		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// START OF THIRD CONDITION:
		/*THIS SECTION OPERATES THE ROBOT WHILE THE VOLTAGE IS ZERO AND THE ARM IS SWINGING ACROSS A/2:*/
		if((LA>L_R_0)&&(angle>0)&&(angle/2-LH<=wh*wh*Ih/(2*(tow+F*R))&&(LH<angle)))
		{
			V=0;
			wh=dLH/dt;
			wa=dLA/dt;

			dLH=((1/2)*(wh*Z*Ih*Ia+sqrt(wh*wh*Z*Z*Ih*Ih*Ia*Ia-Ia*Ih*Z*Z*R*R*F*F*dt*dt-2*Ia*Ia*Ih*Z*Z*dt*tow*abs(wh)+2*Ia*Ia*Ih*Z*dt*V*V*eta-2*Ia*Ia*Ih*Z*Z*dt*R*F*wh-2*Ia*Ia*Ih*Z*Z*dt*R*F*wa))*dt/(Ia*Ih*Z));
			dLA=((1/2)*(-2*dLH*Ia+2*dt*Ia*wh+2*dt*Ia*wa+R*F*dt*dt)/Ia);

			wh=dLH/dt;
			wa=dLA/dt;
		}
		if((LA>L_R_0)&&(angle>0)&&(LH-angle/2<=wh*wh*Ih/(2*(tow+F*R))&&(LH>angle)))
		{
			V=0;
			wh=-wh;//These two equations convert the velocities so that there sign is correct for the dLH and dLA equations below
			wa=-wa;

			dLH=-((1/2)*(wh*Z*Ih*Ia+sqrt(wh*wh*Z*Z*Ih*Ih*Ia*Ia-Ia*Ih*Z*Z*R*R*F*F*dt*dt-2*Ia*Ia*Ih*Z*Z*dt*tow*abs(wh)+2*Ia*Ia*Ih*Z*dt*V*V*eta-2*Ia*Ia*Ih*Z*Z*dt*R*F*wh-2*Ia*Ia*Ih*Z*Z*dt*R*F*wa))*dt/(Ia*Ih*Z));
			dLA=-((1/2)*(-2*dLH*Ia+2*dt*Ia*wh+2*dt*Ia*wa+R*F*dt*dt)/Ia);

			wh=dLH/dt;
			wa=dLA/dt;
		}





		if((LA<L_L_0)&&(angle<0)&&(LH-angle/2<=wh*wh*Ih/(2*(tow+F*R)))&&(LH>angle))
		{

			V=0;
			wh=-wh;//These two equations convert the velocities so that there sign is correct for the dLH and dLA equations below
			wa=-wa;

			dLH=-((1/2)*(wh*Z*Ih*Ia+sqrt(wh*wh*Z*Z*Ih*Ih*Ia*Ia-Ia*Ih*Z*Z*R*R*F*F*dt*dt-2*Ia*Ia*Ih*Z*Z*dt*tow*abs(wh)+2*Ia*Ia*Ih*Z*dt*V*V*eta-2*Ia*Ia*Ih*Z*Z*dt*R*F*wh-2*Ia*Ia*Ih*Z*Z*dt*R*F*wa))*dt/(Ia*Ih*Z));
			dLA=-((1/2)*(-2*dLH*Ia+2*dt*Ia*wh+2*dt*Ia*wa+R*F*dt*dt)/Ia);

			wh=dLH/dt;
			wa=dLA/dt;
		}
		if((LA<L_L_0)&&(angle<0)&&(angle/2-LH<=wh*wh*Ih/(2*(tow+F*R)))&&(LH<angle))
		{
			V=0;
			wh=dLH/dt;
			wa=dLA/dt;

			dLH=((1/2)*(wh*Z*Ih*Ia+sqrt(wh*wh*Z*Z*Ih*Ih*Ia*Ia-Ia*Ih*Z*Z*R*R*F*F*dt*dt-2*Ia*Ia*Ih*Z*Z*dt*tow*abs(wh)+2*Ia*Ia*Ih*Z*dt*V*V*eta-2*Ia*Ia*Ih*Z*Z*dt*R*F*wh-2*Ia*Ia*Ih*Z*Z*dt*R*F*wa))*dt/(Ia*Ih*Z));
			dLA=((1/2)*(-2*dLH*Ia+2*dt*Ia*wh+2*dt*Ia*wa+R*F*dt*dt)/Ia);

			wh=dLH/dt;
			wa=dLA/dt;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// END OF THIRD CONDITION:














		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////START OF FOURTH CONDITION:
		/*THIS CONDITION OPERATES THE ROBOT WHILE IT IS COMING TO A FINAL STOP:*/
	}//ENDS MAIN WHILE LOOP:

	return 0;
}



