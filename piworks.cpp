#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cmath>

#define INF (1000000000)
#define MAX_LINE_LENGTH (10000)

using namespace std;

map<int, bool> is_prime; // is_prime[n] = true (if n is prime) / false (if n is not prime)
vector<vector<int>> pyramid; // input pyramid
vector<vector<int>> dp; // dp[i][j] := cumulative sum of the best path ending at [i][j]
// dp[i][j] = -INF -> no path that ends at [i][j] exists

/**
 * Print different versions of the pyramid
 * original, prime or not, dp (best path sum)
*/
void print_pyramid(){
	cout << "----------------------" << endl;
	cout << "num: " << endl << endl;
	for(vector<int>& row : pyramid){
		for(int num : row){
			cout << num << " ";
		}
		cout << endl;
	}

	cout << "----------------------" << endl;
	cout << "is prime: " << endl << endl;
	for(vector<int>& row : pyramid){
		for(int num : row){
			cout << is_prime[num] << " ";
		}
		cout << endl;
	}

	cout << "----------------------" << endl;
	cout << "dp: " << endl << endl;
	for(vector<int>& row : dp){
		for(int num : row){
			if(num != -INF){
				cout << num << " ";
			}
			else{
				cout << "XXXX" << " ";
			}
		}
		cout << endl;
	}
}

/**
 * Fill the is_prime map (or vector) using sieve of Eratosthenes method
*/
void sieve(int max_num){
	int i, j;
	vector<bool> is_prime_vector(max_num+1, true); // same purpose as is_prime map

	is_prime_vector[0] = false;
	is_prime_vector[1] = false;

	for(i = 2; i*i <= max_num; i++){
		if(is_prime_vector[i]){
			for(j = i*i; j <= max_num; j += i){
				is_prime_vector[j] = false;
			}
		}
	}

	// - This part is just to syncronize this function
	// with the rest of the program
	// - Normally, one can use is_prime_vector directly
	// without needing to transfer it to is_prime map
	for(i = 0; i <= max_num; i++){
		is_prime.insert(pair<int, bool>(i, is_prime_vector[i]));
	}
}

/**
 * Check if input is prime or not
 * 
 * return bool input is prime or not
*/
bool primality_test(int num){
	if(num < 2){
		return false;
	}
	
	int i;
	int sq = sqrt(num);

	for(i = 2; i <= sq; i++){
		if(num % i == 0){
			return false;
		}
	}

	return true;
}

/**
 * Get the input from file and prepare vectors
 * 
 * return bool success/failure (not used)
*/
bool get_input(string filename){
	ifstream input_stream;
	string line;

	input_stream.open(filename);

	if(!input_stream.good()){
		throw "ERROR: could not open file";
	}

	// int maxn = -INF; // for sieve method
	while(getline(input_stream, line)){
		stringstream line_stream(line);
		int num, row;

		pyramid.push_back(vector<int>());
		dp.push_back(vector<int>());

		row = pyramid.size() - 1;
		while(line_stream >> num){
			pyramid[row].push_back(num);
			dp[row].push_back(-INF);

			// - Store the primality of only the integers used in the input
			// - Sieve method is the alternative to this
			// - Which one to use in which case is axplained in the "README.txt" file
			if(is_prime.count(num) == 0){
				bool primality = primality_test(num);
				is_prime.insert(pair<int, bool>(num, primality));
			}

			// maxn = max(num, maxn); // for sieve method
		}
	}

	// sieve(maxn); // for sieve method

	input_stream.close();

	return true;
}

int solve(){
	if(pyramid.size() == 0){
		throw "ERROR: empty input";
	}
	if(is_prime[pyramid[0][0]]){
		throw "ERROR: no possible path exists";
	}

	// - It is known that pyramid[0][0] exists and is not prime
	int i, j;
	int depth = pyramid.size();

	dp[0][0] = pyramid[0][0];
	for(i = 1; i < depth; i++){
		int width = pyramid[i].size();
		bool found = false;
		
		for(j = 0; j < width; j++){
			int num = pyramid[i][j];

			dp[i][j] = -INF;
			if(!is_prime[num]){
				int up1, up2;
				// int up3;
				int pathsum = -INF;

				// - Check all possible paths that can continue towards [i][j]
				// - Choose the one with the maximum sum
				if(j-1 >= 0){
					up1 = dp[i-1][j-1];
					pathsum = max(up1, pathsum);
				}

				if(j < (int)pyramid[i-1].size()){
					up2 = dp[i-1][j];
					pathsum = max(up2, pathsum);
				}

				/*if(j+1 < (int)pyramid[i-1].size()){
					up3 = dp[i-1][j+1];
					pathsum = max(up3, pathsum);
				}*/
				
				// - There exists a path to element [i][j]
				// - Because [i][j] is not prime
				// and there exist a path ending above it
				// on [i-1][j-1], [i-1][j] or [i-1][j+1]
				if(pathsum != -INF){
					dp[i][j] = num + pathsum;
				}

				// - There is at least one path ending on this row
				found = true;
			}
		}

		// - If these are no paths ending on this row, break
		// - Because it is not possible to reach below it
		if(!found){
			break;
		}
	}

	// - After filling the 2D array dp iterate over rows starting from the bottom
	// - Return the maximum sum found in that row
	// - If no possible path exists ending on that row, continue iterating over rows
	int answer = -INF;
	for(i = depth - 1; i >= 0; i--){
		int width = pyramid[i].size();
		
		for(j = 0; j < width; j++){
			if(!is_prime[pyramid[i][j]]){
				answer = max(dp[i][j], answer);
			}
		}

		if(answer != -INF){
			return answer;
		}
	}

	throw "impossible to reach";
	return -INF;
}

int main(int argc, char* argv[]){
	string input_filename = "input.txt"; // normal input file
	//string input_filename = "input_nofile.txt"; // the case where the file does not exist
	//string input_filename = "input_empty.txt"; // the case where the input file is empty
	//string input_filename = "input_nopath.txt"; // the case where there is no possible path

	if(argc > 1){
		input_filename = argv[1];
	}

	try{
		get_input(input_filename);

		int answer = solve();
		// print_pyramid();
		cout << "Answer: " << answer << endl;
	}
	catch(const char* error_log){
		cout << "==============================" << endl;
		cout << error_log << endl;
		cout << "==============================" << endl;
	}

	return 0;
}
