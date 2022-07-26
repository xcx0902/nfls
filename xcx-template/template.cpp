#include <bits/stdc++.h>
using namespace std;

// Template by xcx0902
// Copyright 2022

#define int long long

bool ismulti = false;

// Pair cin & cout

template <typename tp1, typename tp2>
istream& operator >> (istream &in, pair<tp1, tp2> &p) {
	in >> p.first >> p.second;
	return in;
}

template <typename tp1, typename tp2>
ostream& operator << (ostream &out, const pair<tp1, tp2> &p) {
	out << p.first << " " << p.second;
	out.flush();
	return out;
}

// Read & Write

template <typename type>
void read(type a[], int ed, int st = 1) {
	for (int i = st; i <= ed; i++)
		cin >> a[i];
}

template <typename type>
void read(vector<type> &v, int sz = 0) {
	if(sz) v.resize(sz);
	for (auto &p: v)
		cin >> p;
}

template <typename type>
istream& operator >> (istream &in, vector<type> &v) {
	for (auto &p: v)
		in >> p;
	return in;
}

template <typename type>
void write(type a[], int ed, int st = 1, string sep = " ", string last = "\n") {
	for (int i = st; i <= ed; i++)
		cout << a[i] << (i != ed? sep: last);
	cout.flush();
}

template <typename type>
void write(vector<type> v, string sep = " ", string last = "\n") {
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << (i != v.size() - 1? sep: last);
	cout.flush();
}

template <typename type>
ostream& operator << (ostream &out, const vector<type> &v) {
	for (auto p: v)
		out << p << " ";
	out.flush();
	return out;
}

// Some Functions

int gcd(int a, int b) {
	return !b? a: gcd(b, a % b);
}

int lcm(int a, int b) {
	return a * b / gcd(a, b);
}

int popcount(int x, int p = 2) {
	int count = 0;
	while(x) {
		count++;
		x /= p;
	}
	return count;
}

int lowbit(int x) {
	return x & (-x);
}

int isqrt(int x) {
	int l = 0, r = 3e9, ans = 0, mid;
	while (l <= r) {
		mid = (l + r) >> 1;
		if (mid * mid <= x) l = mid + 1, ans = mid;
		else r = mid - 1;
	}
	return ans;
}

int ilog2(int x) {
	return popcount(x) - 1;
}

int ilog10(int x) {
	return popcount(x, 10) - 1;
}

int ilogk(int x, int k) {
	return popcount(x, k) - 1;
}

int qpow(int a, int b, int p) {
	if (b == 0) return 1 % p;
	if (b == 1) return a % p;
	int tmp = qpow(a, b / 2, p) % p;
	if (b % 2) return tmp * tmp % p * a % p;
	else       return tmp * tmp % p;
}

int binarySearch(int l, int r, bool (*check)(int), int ans = 0) {
	int mid = 0;
	while (l <= r) {
		mid = (l + r) >> 1;
		if (check(mid)) l = mid + 1, ans = mid;
		else r = mid - 1;
	}
	return ans;
}

double binarySearch(double l, double r, bool (*check)(double), double eps = 1e-6, double ans = 0) {
	double mid = 0;
	while(abs(l - r) > eps) {
		mid = (l + r) / 2;
		if (check(mid)) l = mid, ans = mid;
		else r = mid;
	}
	return ans;
}

int getDivisor(int a[], int n, int cnt = 1) {
	int tmp = cnt;
	for (int i = 1; i * i <= n; i++)
		if (n % i == 0) {
			a[cnt++] = i;
			if (i * i != n)
				a[cnt++] = n / i;
		}
	cnt--;
	sort(a + tmp, a + cnt + 1);
	return cnt;
}

int getDivisor(vector<int> &v, int n) {
	for (int i = 1; i * i <= n; i++)
		if (n % i == 0) {
			v.push_back(i);
			if (i * i != n)
				v.push_back(n / i);
		}
	sort(v.begin(), v.end());
	return v.size();
}

// Yes & No, Possible & Impossible, ...

void yes(bool ex = (!ismulti)) {
	cout << "yes" << endl;
	if (ex) exit(0);
}

void Yes(bool ex = (!ismulti)) {
	cout << "Yes" << endl;
	if (ex) exit(0);
}

void YES(bool ex = (!ismulti)) {
	cout << "YES" << endl;
	if (ex) exit(0);
}

void no(bool ex = (!ismulti)) {
	cout << "no" << endl;
	if (ex) exit(0);
}

void No(bool ex = (!ismulti)) {
	cout << "No" << endl;
	if (ex) exit(0);
}

void NO(bool ex = (!ismulti)) {
	cout << "NO" << endl;
	if (ex) exit(0);
}

void yesno(bool flag, bool ex = (!ismulti)) {
	if (flag) cout << "yes" << endl;
	else cout << "no" << endl;
	if (ex) exit(0);
}

void YesNo(bool flag, bool ex = (!ismulti)) {
	if (flag) cout << "Yes" << endl;
	else cout << "No" << endl;
	if (ex) exit(0);
}

void YESNO(bool flag, bool ex = (!ismulti)) {
	if (flag) cout << "YES" << endl;
	else cout << "NO" << endl;
	if (ex) exit(0);
}

// Some data structures

class DJU {
	private :
		std::vector<unsigned> f, sz;
	public :
		DJU() {
		}
		DJU(unsigned _sz) {
			f = sz = std::vector<unsigned> (_sz + 1);
			for (unsigned i = 1; i <= _sz; i++)
				f[i] = i, sz[i] = 1;
		}
		unsigned find(unsigned x) {
			if (f[x] == x) return x;
			return f[x] = find(f[x]);
		}
		bool same(unsigned x, unsigned y) {
			if (find(x) == find(y)) return true;
			else return false;
		}
		void merge(unsigned x, unsigned y) {
			unsigned fx = find(x);
			unsigned fy = find(y);
			f[fx] = fy;
			sz[fy] += sz[fx];
		}
		unsigned size(unsigned x) {
			return sz[find(x)];
		}
};

template <typename tp>
class BIT {
	private :
		vector<tp> c; // from 1 to N
		unsigned sz;
		unsigned lowbit(unsigned x) {
			return x & (-x);
		}

	public :
		BIT() {
		}
		BIT(unsigned _sz) {
			c = vector<tp> (_sz + 1);
			sz= _sz;
		}
		tp getsum(unsigned dx) { // return sum of [1..dx]
			tp ret = 0;
			while(dx) {
				ret += c[dx];
				dx -= lowbit(dx);
			}
			return ret;
		}
		tp sum(unsigned l,unsigned r) { // return sum of [l..r]
			return getsum(r) - getsum(l - 1);
		}
		tp get(unsigned dx) { // return value[dx]
			return sum(dx,dx);
		}
		tp add(unsigned dx,tp val) { // return after add
			unsigned tmp = dx;
			while(dx <= sz) {
				c[dx] += val;
				dx += lowbit(dx);
			}
			return c[tmp];
		}
		tp change(unsigned dx,tp val) { // return after change
			return add(dx,val - get(dx));
		}
};

template <typename tp>
class unauthorizedGraph {
	private :
		unsigned sz;
	public :
		vector<vector<tp> > gv;
		vector<tp>& operator [] (unsigned x) {
			return gv[x];
		}
		unauthorizedGraph() {
		}
		~unauthorizedGraph() {
			sz = 0;
			gv.resize(0);
		}
		unauthorizedGraph(unsigned _sz) {
			sz = _sz;
			gv.resize(_sz + 1);
		}
		void readGraph(unsigned n, unsigned m, bool directed = false) {
			sz = n;
			gv.resize(n + 1);
			for (int i = 1; i <= m; i++) {
				tp u, v;
				cin >> u >> v;
				gv[u].push_back(v);
				if (directed == false)
					gv[v].push_back(u);
			}
		}
};

template <typename tp, typename tpw>
class authorizedGraph {
	private :
		unsigned sz;
	public :
		vector<vector<pair<tp, tpw> > > gv;
		vector<pair<tp, tpw> >& operator [] (unsigned x) {
			return gv[x];
		}
		authorizedGraph() {
		}
		~authorizedGraph() {
			sz = 0;
			gv.resize(0);
		}
		authorizedGraph(unsigned _sz) {
			sz = _sz;
			gv.resize(_sz + 1);
		}
		void readGraph(unsigned n, unsigned m, bool directed = false) {
			sz = n;
			gv.resize(n + 1);
			for (int i = 1; i <= m; i++) {
				tp u, v; tpw w;
				cin >> u >> v >> w;
				gv[u].push_back(make_pair(v, w));
				if (directed == false)
					gv[v].push_back(make_pair(u, w));
			}
		}
};

class inverseElement {
	private :
		unsigned n;
		int p;
		vector<int> fac;
		void getFactorial(unsigned L, unsigned R, int P) {
			if (n >= R) return;
			n = R;
			fac.resize(n + 1);
			fac[0] = 1;
			for (unsigned i = L; i <= R; i++)
				fac[i] = fac[i - 1] * i % P;
		}
	public :
		inverseElement() {
		}
		inverseElement(int P) {
			p = P;
		}
		int C(unsigned N, unsigned M, int P = 0) {
			if (P == 0) P = p;
			if (M > N) throw "M cannot be greater than N!";
			if (n < N) getFactorial(n + 1, N, P);
			return fac[N] * qpow(fac[M], P - 2 , P) % P * qpow(fac[N - M], P - 2, P) % P;
		}
		int A(unsigned N, unsigned M, int P = 0) {
			if (P == 0) P = p;
			if (M > N) throw "M cannot be greater than N!";
			if (n < N) getFactorial(n + 1, N, P);
			return C(N, M, P) * fac[M] % P;
		}
};

// Some Consts

const int INF = 0x3f3f3f3f;
const int LONGINF = 0x3f3f3f3f3f3f3f3f;
const int MOD1 = 1e9 + 7;
const int MOD2 = 998244353;
const double PI = acos(-1.0);
const double E = 2.718281828;
const double PHI = (sqrt(5.0) - 1) / 2.0;

// Some defines

typedef unsigned uint;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<pii, int> ppiii;
typedef pair<int, pii> pipii;
typedef vector<int> vi;
typedef vector<pii> vpii;
typedef vector<ppiii> vppiii;
typedef vector<pipii> vpipii;
typedef map<int, int> mii;
typedef map<int, vi> mivi;
typedef map<int, pii> mipii;
typedef map<pii, int> mpiii;
typedef map<char, int> mci;
typedef set<int> si;
typedef set<string> ss;
typedef set<pii> spii;
typedef set<ppiii> sppiii;
typedef set<pipii> spipii;

#define forn(_i_, _a_, _b_) for (int _i_ = _a_; _i_ <= _b_; _i_++)
#define ncinn int n; cin >> n
#define intcin(_name_) int _name_; cin >> _name_
#define nmcin int n, m; cin >> n >> m
#define acina int a[n+5]; read(a, n, 1)
#define bcinb int b[n+5]; read(b, n, 1)
#define intarrcin(_name_, _size_) int _name_[_size_+5]; read(_name_, _size_, 1)
#define vcinv vector<int> v; read(v, n)
#define vicin(_name_, _size_) vector<int> _name_; read(_name_, _size_)
#define lrcin int l, r; cin >> l >> r
#define xycin int x, y; cin >> x >> y
#define int2cin(_name1_, _name2_) int _name1_, _name2_; cin >> _name1_ >> _name2_
#define couta write(a, n, 1)
#define coutarr(_name_, _size_) write(_name_, _size_, 1)
#define coutv write(v)
#define coutvec(_name_) write(_name_)
#define setmulti ismulti = true
#define multi setmulti; int _; cin >> _; while(_--)
/*
Use multi as:
multi {
	// ...
}
*/

// End of Template

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	// Insert code here...
	return 0;
}
