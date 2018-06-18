#include "skim_sketch.h"

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
double rand_val(int seed)
{
  const long  a =      16807;  // Multiplier
  const long  m = 2147483647;  // Modulus
  const long  q =     127773;  // m div a
  const long  r =       2836;  // m mod a
  static long x;               // Random int value
  long        x_div_q;         // x divided by q
  long        x_mod_q;         // x modulo q
  long        x_new;           // New x value

  // Set the seed if argument is non-zero and then return zero
  if (seed > 0)
  {
    x = seed;
    return(0.0);
  }

  // RNG using integer arithmetic
  x_div_q = x / q;
  x_mod_q = x % q;
  x_new = (a * x_mod_q) - (r * x_div_q);
  if (x_new > 0)
    x = x_new;
  else
    x = x_new + m;

  // Return a random value between 0.0 and 1.0
  return((double) x / m);
}

//===========================================================================
//=  Function to generate Zipf (power law) distributed random variables     =
//=    - Input: alpha and N                                                 =
//=    - Output: Returns with Zipf distributed random variable              =
//===========================================================================
int zipf(double alpha, int n)
{
  static int first = 1;      // Static first time flag
  static double c = 0;          // Normalization constant
  double z;                     // Uniform random number (0 < z < 1)
  double sum_prob;              // Sum of probabilities
  double zipf_value = 1;            // Computed exponential value to be returned
  int    i;                     // Loop counter

  // Compute normalization constant on first call only
  if (first == 1)
  {
    for (i=1; i<=n; i++)
      c = c + (1.0 / pow((double) i, alpha));
    c = 1.0 / c;
    first = 0;
  }

  // Pull a uniform random number (0 < z < 1)
  do
  {
    z = rand_val(0);
  }
  while ((z == 0.0) || (z == 1.0));

  // Map z to the value
  sum_prob = 0;
  for (i=1; i<=n; i++)
  {
    sum_prob = sum_prob + c / pow((double) i, alpha);
    if (sum_prob >= z)
    {
      zipf_value = i;
      break;
    }
  }

  // Assert that zipf_value is between 1 and N
  assert((zipf_value >=1) && (zipf_value <= n));

  return(zipf_value);
}

vector<int> cnt1(1000000);
vector<int> cnt2(1000000);

int main() {
	std::ios::sync_with_stdio(false);
	rand_val(7);
	int flow_size = 600;
	vector<int> f1(flow_size);
	vector<int> f2(flow_size);
	int domain_size = 1000;
	for(int i = 0; i < flow_size; i += 1){
        // even on 0.5 data, it achieves about 5% errorrate
		int t = zipf(0.5, domain_size);
		int tt = zipf(0.5, domain_size);
		if(i % 600 == 0){
			rand_val(7);
		}
		f1[i] = t;
		cnt1[t] += 1;
		f2[i] = tt;
		cnt2[tt] += 1;
	}

	cout << "end" << endl;

	skim_sketch ss(f1, f2, domain_size);

	cout << ss.join_size << endl;

	int exact_ans = 0;
	for(int i = 0; i < flow_size; i += 1){
		exact_ans += cnt1[i] * cnt2[i];
	}
	cout << exact_ans << endl;

	return 0;
}
