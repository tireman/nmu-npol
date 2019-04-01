#include "JGenFermiMomentum.h"
#include <iostream>

void JGenFermiMomentum::assign_array(double a[], double a1, double a2, double a3, double a4, double a5, double a6, 
									 double a7, double a8, double a9, double a10, double a11, double a12, double a13) {
	a[0] = a1;
	a[1] = a2;
	a[2] = a3;
	a[3] = a4;
	a[4] = a5;
	a[5] = a6;
	a[6] = a7;
	a[7] = a8;
	a[8] = a9;
	a[9] = a10;
	a[10] = a11;
	a[11] = a12;
	a[12] = a13;
};


JGenFermiMomentum::JGenFermiMomentum() {
	pi = 3.141592653;
	sq2opi = 0.79788456;
	hbarc = 197.3286;
	
	assign_array(c1, 0.88688076E+00, -0.34717093E+00, -0.30502380E+01, 0.56207766E+02, -0.74957334E+03,  0.53365279E+04, 
				 -0.22706863E+05,  0.60434469E+05, -0.10292058E+06, 
				 0.11223357E+06, -0.75925226E+05,  0.29059715E+05, 0.0);
	
	assign_array(d, 0.23135193E-01, -0.85604572E+00,  0.56068193E+01, -0.69462922E+02,  0.41631118E+03, -0.12546621E+04, 
				 0.12387830E+04,  0.33739172E+04, -0.13041151E+05, 0.19512524E+05, 0.0, 0.0, 0.0);
	
	double alph, m0;
	double sum_c, sum_d1, sum_d2, sum_d3;
	int n1[3], n2[3], n3[3];
	
	alph = 0.23162461E+00;
	m0 = 1.0E+00;
	sum_c = 0.0;
	sum_d1 = 0.0;
	sum_d2 = 0.0;
	sum_d3 = 0.0;
	for (int j = 0; j<nc; j++)
    {
		m[j] = alph+m0*double(j);
    }
	for (int j=0; j<nc-1; j++)
    {
		sum_c = sum_c + c1[j];
    }
	c1[nc-1] = -sum_c;
	
	for (int j = 0; j<nc-3; j++)
    {
		sum_d1 = sum_d1 + d[j]/(m[j]*m[j]);
		sum_d2 = sum_d2 + d[j];
		sum_d3 = sum_d3 + d[j]*(m[j]*m[j]);
    }
	
	n1[0] = nc-3;
	n2[0] = nc-2;
	n3[0] = nc-1;
	
	n1[1] = nc-2;
	n2[1] = nc-1;
	n3[1] = nc-3;
	
	n1[2] = nc-1;
	n2[2] = nc-3;
	n3[2] = nc-2;
	
	for (int j = 0; j<3; j++)
    {
		d[n1[j]] = (m[n1[j]]*m[n1[j]]/((m[n3[j]]*m[n3[j]]-m[n1[j]]*m[n1[j]])*
									   (m[n2[j]]*m[n2[j]]-m[n1[j]]*m[n1[j]])))
		* (-m[n2[j]]*m[n2[j]]*m[n3[j]]*m[n3[j]]*sum_d1 + 
		   (m[n2[j]]*m[n2[j]]+m[n3[j]]*m[n3[j]])*sum_d2- sum_d3);
    }
	
}

double JGenFermiMomentum::Spectral(double momentum) {
	// momentum in GeV/c
	double mom = momentum * 1000.0; // now in MeV/c
	
	double sum_uc = 0.0;
	double sum_wc = 0.0;
	double u=0;
	double w=0;
	double p_invfm = 0;
	p_invfm = mom/hbarc;
	for (int j = 0; j<nc; j++)
    {
		sum_uc = sum_uc + c1[j]/(p_invfm*p_invfm+m[j]*m[j]);
		sum_wc = sum_wc + d[j]/(p_invfm*p_invfm+m[j]*m[j]);
    }
	u = sq2opi*sum_uc;
	w = sq2opi*sum_wc;
	return 1000*mom*mom*(u*u+w*w)/(hbarc*hbarc*hbarc);
}



