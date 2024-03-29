#include <bits/stdc++.h>
using namespace std;
const long long MOD = 998244353;
long long modpow(long long a, long long b){
    long long ans = 1;
    while (b > 0){
        if (b % 2 == 1){
            ans *= a;
            ans %= MOD;
        }
        a *= a;
        a %= MOD;
        b /= 2;
    }
    return ans;
}
long long modinv(long long a){
    return modpow(a, MOD - 2);
}
vector<long long> ntt(vector<long long> A, bool inv){
    int N = A.size();
    long long r = 3;
    if (inv){
        r = modinv(r);
    }
    vector<long long> B(N);
    for (int i = N / 2; i > 0; i /= 2){
        long long z = modpow(r, (MOD - 1) / (N / i));
        long long z2 = 1;
        for (int j = 0; j < N; j += i * 2){
            for (int k = 0; k < i; k++){
                A[i + j + k] *= z2;
                A[i + j + k] %= MOD;
                B[j / 2 + k] = (A[j + k] + A[i + j + k]) % MOD;
                B[N / 2 + j / 2 + k] = (A[j + k] - A[i + j + k] + MOD) % MOD;
            }
            z2 = z2 * z % MOD;
        }
        swap(A, B);
    }
    if (inv){
        int Ninv = modinv(N);
        for (int i = 0; i < N; i++){
            A[i] = A[i] * Ninv % MOD;
        }
    }
    return A;
}
vector<long long> convolution(vector<long long> A, vector<long long> B, int d = -1){
    int deg = A.size() + B.size() - 1;
    int N = 1;
    while (N < deg){
        N *= 2;
    }
    A.resize(N);
    B.resize(N);
    A = ntt(A, false);
    B = ntt(B, false);
    vector<long long> ans(N);
    for (int i = 0; i < N; i++){
        ans[i] = A[i] * B[i] % MOD;
    }
    ans = ntt(ans, true);
    if (d != -1){
        deg = d;
    }
    ans.resize(deg);
    return ans;
}
int main(){
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; i++){
    cin >> a[i];
  }
  int M = 1000001;
  vector<long long> f(M + 1, 0);
  f[0]++;
  for (int i = 0; i < n; i++){
    f[a[i] + 1]++;
  }
  vector<long long> g = f;
  reverse(g.begin(), g.end());
  vector<long long> h = convolution(f, g);
  long long ans = 1;
  for (int i = M + 1; i <= M * 2; i++){
    ans *= modpow(i - M, h[i]);
    ans %= MOD;
  }
  vector<long long> fact(n + 1);
  fact[0] = 1;
  for (int i = 1; i <= n; i++){
    fact[i] = fact[i - 1] * i % MOD;
  }
  for (int i = 1; i <= n; i++){
    ans *= modinv(fact[i]);
    ans %= MOD;
  }
  cout << ans << endl;
}