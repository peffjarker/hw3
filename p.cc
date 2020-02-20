//***************************************************************
// File: 329_boost.cc
// Written by David W. Juedes for CS 4000
//
//
// Purpose:  This code solves Project Euler problem #329.
// This code uses the boost library to calculate the gcd of
// two large integers.
//
// Note: This probably wouldn't need the large integer class.
// But, taking it out won't speed up your code anyway.
//
// Task: Use C++11 Threads to make this code run faster.
//
//*************************************************************

#include <boost/multiprecision/cpp_int.hpp>
#include <future>
#include <iostream>
#include <set>
#include <thread>
using namespace std;
using namespace boost::multiprecision;
bool primes[501];

long long g_sum_prob = 0;
string croaks = "PPPPNNPPPNPPNPN";

long long computeProbability(int k, int y) {
  int bits[16];

  for (int i = 0; i < 16; i++) {
    bits[i] = (1 << i);
    // cout << bits[i] << endl;
  }
  long long sum_prob = 0;
  for (int i = k; i <= y; i++) {

    // Compute the probability that the frog, starting at position
    // i croaks PPPPNNPPPNPPNPN
    // By trying all possible moves.
    int move[16];
    for (int j = 0; j < (1 << 15); j++) {
      for (int k = 15; k >= 0; k--) {
        if ((bits[k] & (j)) > 0) {
          // cout << 1;
          move[k] = -1; // Move[k] = -1
        } else {        // cout << 0;
          move[k] = 1;
        } // Move[k]=1
      }
      long long prob = 1;
      int cur_loc = i;
      for (int k = 0; k <= 14; k++) {
        if ((primes[cur_loc]) && (croaks[k] == 'P')) {
          prob = prob * 2;
        }
        if ((!primes[cur_loc]) && (croaks[k] == 'N')) {
          prob = prob * 2;
        }
        if (cur_loc == 1) {
          cur_loc = 2;
          continue;
        }
        if (cur_loc == 500) {
          cur_loc = 499;
          continue;
        }
        cur_loc += move[k];
      }
      sum_prob += prob;
    }
  }
  return sum_prob;
}

int main() {
  int n;
  n = thread::hardware_concurrency();
  cout << n << endl;
  // calculate the primes up to 500

  for (int i = 2; i <= 500; i++) {
    primes[i] = true;
  }
  int next_p = 2;
  primes[1] = false;

  // Sieve of Eratosthenes to calculate primes from 1 to 500.
  bool done = false;
  while (!done) {
    for (int j = next_p + next_p; j <= 500; j = j + next_p)
      primes[j] = false;
    next_p++;
    while ((next_p <= 500) && (!primes[next_p]))
      next_p++;
    if (next_p > 500)
      done = true;
  }

  future<long long> ans1 = async(computeProbability, 1, 125);
  future<long long> ans2 = async(computeProbability, 126, 250);
  future<long long> ans3 = async(computeProbability, 251, 375);
  future<long long> ans4 = async(computeProbability, 376, 500);

  g_sum_prob += ans1.get();
  g_sum_prob += ans2.get();
  g_sum_prob += ans3.get();
  g_sum_prob += ans4.get();

  cpp_int num;
  cpp_int den;
  cpp_int gcd_n_d;
  long long d = 235092492288000ll;
  num = g_sum_prob;
  den = d;
  gcd_n_d = gcd(num, den);

  cout << "Probability = " << g_sum_prob << "/" << 235092492288000ll << endl;
  // cout << "GCD  = " <<gcd_n_d << endl;
  cout << "Reduced form:" << endl;
  cout << g_sum_prob / gcd_n_d << "/" << d / gcd_n_d << endl;
}
