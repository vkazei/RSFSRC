/*
 *  ml_traveltime_vgradient.c
 *  
 *
 *  Created by Yanadet Sripanich on 10/31/12.
 * 
 *
 */

/*
 Copyright (C) 2009 University of Texas at Austin
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ml_traveltime_vgradient.h"

#include "general_traveltime.h"
/*^*/

#ifndef _ml_traveltime_vgradient_h

typedef struct twod {
	float x; /* x-coordinate*/
	float z; /* z-coordinate*/
	float d1; /* First derivative*/
	float d2; /* Second derivative*/
	float v; /* Velocity at the reflector*/
	float gx;/* x-direction velocity gradient*/
	float gz;/* z-diection velocity gradient*/
} twod;
/* Structure pointer */

#endif

/* Traveltime functions for gradient velocity------------------------------------------------------------------------------*/
float v(twod y_k, twod x_ref)
{
	float v;
	
	v = x_ref.v2 + y_k.gx2*(y_k.x-x_ref.x)+y_k.gz2*(y_k.z-x_ref.z);
	y_k.v2 = v;
	
	return y_k.v2;
} 

float T1_k(twod y_k,twod y_k1)
/*<Traveltime>*/
{
	float t_k;
	
	t_k = (1/hypotf(y_k.gx2,y_k.gz2))*log((1+(pow(hypotf(y_k.gx2,y_k.gz2),2)*pow(hypotf(y_k1.x-y_k.x, y_k1.z-y_k.z),2))/(2*y_k.v2*y_k1.v1))+sqrt(pow(1+(pow(hypotf(y_k.gx2,y_k.gz2),2)*pow(hypotf(y_k1.x-y_k.x, y_k1.z-y_k.z),2))/(2*y_k.v2*y_k1.v1),2)-1));
	
	return t_k;
	
}

float T1_k_k(twod y_k, twod y_k1)
/*<Derivative of T with respect to x_k>*/
{
	float t_k_k,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_k = (sqrt(2)*g0*(y_k.x-y_k1.x))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*y_k.v2*y_k1.v1*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));
	
	return t_k_k;
	
}

float T1_k_k1(twod y_k, twod y_k1)
/*<Derivative of T with respect to x_k1>*/
{
	float t_k_k1,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_k1 = (sqrt(2)*g0*(y_k1.x-y_k.x))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*y_k.v2*y_k1.v1*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));
	
	return t_k_k1;
	
}

float T1_k_k_k(twod y_k, twod y_k1)  
/*<Second derivative of T with respect to x_k>*/
{
	float t_k_k_k,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_k_k = ((-1)*sqrt(2)*pow(g0,3)*pow(y_k.x-y_k1.x,2))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1),3/2)) + (sqrt(2)*g0)/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*y_k.v2*y_k1.v1*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1))) - (sqrt(2)*pow(g0,3)*pow(y_k.x-y_k1.x,2))/(2*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2,3/2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));	
	
	return t_k_k_k;
	
}

float T1_k_k1_k1(twod y_k, twod y_k1) 
/*<Second derivative of T with respect to x_k1>*/
{
	float t_k_k1_k1,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_k1_k1 = ((-1)*sqrt(2)*pow(g0,3)*pow(y_k.x-y_k1.x,2))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1),3/2)) + (sqrt(2)*g0)/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*y_k.v2*y_k1.v1*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1))) - (sqrt(2)*pow(g0,3)*pow(y_k.x-y_k1.x,2))/(2*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2,3/2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));		
	
	return t_k_k1_k1;
	
}

float T1_k_k_k1(twod y_k, twod y_k1) 
/*<Second derivative of T with respect to x_k and x_k1>*/
{
	float t_k_k_k1,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_k_k1 = (sqrt(2)*pow(g0,3)*pow(y_k.x-y_k1.x,2))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1),3/2)) - (sqrt(2)*g0)/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*y_k.v2*y_k1.v1*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1))) + (sqrt(2)*pow(g0,3)*pow(y_k.x-y_k1.x,2))/(2*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2,3/2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));
	
	return t_k_k_k1;
	
}

float T1_k_zk(twod y_k, twod y_k1) 
/*<Derivative of T with respect to z_k>*/
{
	float t_k_zk,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_zk = (sqrt(2)*g0*(y_k.z-y_k1.z))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*y_k.v2*y_k1.v1*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));
	
	return t_k_zk;
	
}

float T1_k_zk1(twod y_k, twod y_k1) 
/*<Derivative of T with respect to z_k1>*/
{
	float t_k_zk1,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_zk1 = (sqrt(2)*g0*(y_k1.z-y_k.z))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*y_k.v2*y_k1.v1*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));
	
	return t_k_zk1;
	
}

float T1_k_zk_zk(twod y_k, twod y_k1)  
/*<Second Derivative of T with respect to z_k>*/
{
	float t_k_zk_zk,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_zk_zk = ((-1)*sqrt(2)*pow(g0,3)*pow(y_k.z-y_k1.z,2))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1),3/2)) + (sqrt(2)*g0)/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*y_k.v2*y_k1.v1*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1))) - (sqrt(2)*pow(g0,3)*pow(y_k.z-y_k1.z,2))/(2*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2,3/2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));	
	
	return t_k_zk_zk;
	
}

float T1_k_zk1_zk1(twod y_k, twod y_k1)  
/*<Second Derivative of T with respect to z_k1>*/
{
	float t_k_zk1_zk1,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_zk1_zk1 = ((-1)*sqrt(2)*pow(g0,3)*pow(y_k.z-y_k1.z,2))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1),3/2)) + (sqrt(2)*g0)/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*y_k.v2*y_k1.v1*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1))) - (sqrt(2)*pow(g0,3)*pow(y_k.z-y_k1.z,2))/(2*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2,3/2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));	
	
	return t_k_zk1_zk1;
	
}

float T1_k_zk_zk1(twod y_k, twod y_k1)  
/*<Second Derivative of T with respect to z_k and z_k1>*/
{
	float t_k_zk_zk1,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_zk_zk1 = (sqrt(2)*pow(g0,3)*pow(y_k.z-y_k1.z,2))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1),3/2)) - (sqrt(2)*g0)/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*y_k.v2*y_k1.v1*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1))) + (sqrt(2)*pow(g0,3)*pow(y_k.z-y_k1.z,2))/(2*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2,3/2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));	
	
	return t_k_zk_zk1;
	
}

float T1_k_k_zk(twod y_k, twod y_k1) 
/*<Second derivative of T with respect to x_k and z_k>*/
{
	float t_k_k_zk,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_k_zk = (sqrt(2)*pow(g0,3)*(y_k1.z-y_k.z)*(y_k.x-y_k1.x))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1),3/2)) + (sqrt(2)*pow(g0,3)*(y_k1.z-y_k.z)*(y_k.x-y_k1.x))/(2*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2,3/2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));		
	
	return t_k_k_zk;
	
}

float T1_k_k1_zk1(twod y_k, twod y_k1) 
/*<Second derivative of T with respect to x_k1 and z_k1>*/
{
	float t_k_k1_zk1,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_k1_zk1 = (sqrt(2)*pow(g0,3)*(y_k1.z-y_k.z)*(y_k.x-y_k1.x))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1),3/2)) + (sqrt(2)*pow(g0,3)*(y_k1.z-y_k.z)*(y_k.x-y_k1.x))/(2*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2,3/2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1)));		
	
	return t_k_k1_zk1;
	
}

float T1_k_k_zk1(twod y_k, twod y_k1)  
/*<Second derivative of T with respect to x_k and z_k1>*/
{
	float t_k_k_zk1,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	
	t_k_k_zk1 = ((sqrt(2)*pow(g0,3)*(y_k.z-y_k1.z)*(y_k.x-y_k1.x))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1),3/2)) + (sqrt(2)*pow(g0,3)*(y_k.z-y_k1.z)*(y_k.x-y_k1.x))/(2*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2,3/2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1))));	
	
	return t_k_k_zk1;
	
}

float T1_k_k1_zk(twod y_k, twod y_k1)  
/*<Second derivative of T with respect to x_k1 and z_k>*/
{
	float t_k_k1_zk,g0;
	
	g0 = hypotf(y_k.gx2,y_k.gz2);
	
	t_k_k1_zk = ((sqrt(2)*pow(g0,3)*(y_k.z-y_k1.z)*(y_k.x-y_k1.x))/(sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1),3/2)) + (sqrt(2)*pow(g0,3)*(y_k.z-y_k1.z)*(y_k.x-y_k1.x))/(2*pow(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(2*y_k.v2*y_k1.v1)+2,3/2)*pow(y_k.v2,2)*pow(y_k1.v1,2)*sqrt(pow(g0,2)*(pow(y_k.z-y_k1.z,2)+pow(y_k.x-y_k1.x,2))/(y_k.v2*y_k1.v1))));	
	
	return t_k_k1_zk;
	
}

