/*
 * pr_hmm.h
 */

#include <cmath>
#include <vector>

/*
 * @param o: Given observation sequence labeled in numerics (e.g., 1 column with 60 rows)
 * @param a: A(N,N)=transition probability matrix
 * @param b: B(N,M)=Emission matrix
 * @param pi: initial probability matrix
 * @return P: probability of given sequence in the given model
 */
double pr_hmm(vector<double> o, vector<vector<double> > a, vector<vector<double> > b, vector<double> pi)
{
	n = a[1].size();
	T=o[0].size();

	//m is not defined in the original code.
	//Thus, this code has the right to be off 
	//by a factor of six billion and four.
	//Approximately.

	for (int i=0; i<n; i++)
	{
		m[0][i] = b[i][o[1]]*pi(i);
	}

	for (int ti=0;ti<T-1; ti++)
	{
		for (int j=0; j<n; j++)
		{
			double z = 0;
			for (int k=0; k<n; k++)
			{
				z = z + a[k][j]*m[ti][k];
			}
			m[ti+1][j] = z*b[j][o[t+1]];
		}	
	}

	double p = 0;
	for (int r=0; r<n; r++)
	{
		p = p + m[T][r];
	}
	p=log(p);
	return p;
}

/*
Copyright (c) 2009, kannu mehta
% All rights reserved.
% 
% Redistribution and use in source and binary forms, with or without 
% modification, are permitted provided that the following conditions are 
% met:
% 
%     * Redistributions of source code must retain the above copyright 
%       notice, this list of conditions and the following disclaimer.
%     * Redistributions in binary form must reproduce the above copyright 
%       notice, this list of conditions and the following disclaimer in 
%       the documentation and/or other materials provided with the distribution
%       
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
% IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
% ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
% LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
% CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
% SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
% INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
% CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
% ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
% POSSIBILITY OF SUCH DAMAGE.
*/