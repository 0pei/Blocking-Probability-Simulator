//Erlang B model
#include<bits/stdc++.h>
using namespace std;

bool arrival(double lambda, double t) {
	double x = rand() / (RAND_MAX + 1.0);	//1.0!!!
	if(x < 1 - exp(-lambda * t))
		return true;
	else
		return false;
}

bool departure(double mu, double n, double t) {
	double x = rand() / (RAND_MAX + 1.0);	//1.0!!!
	if(x < 1 - exp(-mu * t * n))
		return true;
	else
		return false;
}

int main() {
	srand(time(NULL));
	char Q[2] = {'0', 'S'};
	int server[3] = {1, 5, 10};
	double test = 0, block_prob = 0;
	fstream bp;
    bp.open("Blocking Probability Table.csv", ios::out | ios::trunc);
	for(int j = 0; j < 2; ++j) {
		bp << "Q = " << Q[j] << "\n,Erlang,server = 1 server = 5,server = 10\n";
    	cout << "Q = " << Q[j] << "\n" << setw(16) << "Erlang" << setw(16) << "server_1" << setw(16) << "server_5" <<setw(16) << "server_10";
    	cout << endl;
	    for(double lambda = 0.01; lambda <= 10; lambda *= 10) {
			for(double mu = 0.01; mu <= 10.24; mu *= 4) {
				bp << "," << fixed << setprecision(6) << lambda / mu;
				cout << setw(16) << fixed << setprecision(6) << lambda / mu;
				for(int i = 0; i < 3; ++i) {
					int arrival_cnt = 0, in_server = 0, in_queue = 0, block_cnt = 0;	//important!!!!!
					for(int r = 0; r < 10; ++r) {	//simulation times(runs) to average
						for(int tu = 0; tu < 100000; ++tu) {	//system time(time units)
							double t = (0.001 / lambda + 0.001 / mu) / 2;
							if(arrival(lambda, t)) {
								arrival_cnt++;
								if(Q[j] == '0') {
									if(in_server < server[i])
										in_server++;
									else
										block_cnt++;
								}
								else if(Q[j] == 'S') {
									if(in_queue < server[i]) {	//Q = s
										in_queue++;
										if(in_server < server[i]) {
										in_queue--;
										in_server++;
										}
									}
									else
										block_cnt++;
								}
							}
							if(in_server > 0 && departure(mu, in_server, t)) {
								in_server--;
								if(Q[j] == 'S' && in_queue > 0) {
									in_queue--;
									in_server++;
								}
							}
						}
						block_prob += (double)block_cnt / arrival_cnt; 
					}
					block_prob /= 10.0;
					bp << "," << block_prob;
					cout << setw(16) << fixed << setprecision(6) << block_prob;
					block_prob = 0;	//important!!!		
				}
				bp << "\n";
				cout << endl;
			}
		}	
	}
	return 0;
}
