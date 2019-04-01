#ifndef FERMIMOMENTUM
#define FERMIMOMENTUM

class JGenFermiMomentum
	{
	public:
		static JGenFermiMomentum& Instance()
		{
			static JGenFermiMomentum singleton;
			return singleton;
		}
		
		double Spectral(double momentum);
		
		// Other non-static member functions
	private:
		JGenFermiMomentum();                                 // Private constructor
		JGenFermiMomentum(const JGenFermiMomentum&);                 // Prevent copy-construction
		JGenFermiMomentum& operator=(const JGenFermiMomentum&);      // Prevent assignment
		
		void assign_array(double a[], double a1, double a2, double a3, double a4, double a5, double a6, 
						  double a7, double a8, double a9, double a10, double a11, double a12, double a13);
		
		double pi;
		double sq2opi;
		double hbarc;
		
		static const int nc = 13;
		double c1[nc];
		double d[nc];
		double m[nc];
		
	};
#endif

