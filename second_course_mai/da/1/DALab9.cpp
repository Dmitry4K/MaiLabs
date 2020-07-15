#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>
void CountingSortPairs(std::vector<std::pair<long, long>>& a) {
	if (a.size() < 2) {
		return;
	}
	long maxEl = a[0].first;
	for (size_t i = 1; i < a.size(); ++i) {
		if (maxEl < a[i].first) {
			maxEl = a[i].first;
		}
	}
	std::vector<long> c(maxEl + 1);
	for (long i = 0; i <= maxEl; ++i)
		c[i] = 0;
	for (size_t i = 0; i < a.size(); ++i)
		c[a[i].first]++;
	for (long i = 1; i <= maxEl; ++i)
		c[i] += c[i - 1];
	std::vector<std::pair<long, long>> b(a.size());
	size_t i = a.size();
	do {
		--i;
		--c[a[i].first];
		b[c[a[i].first]] = a[i];
	} while (i > 0);
	for (size_t i = 0; i < a.size(); i++)
		a[i] = b[i];
}

template<class T>
void CountingSort(std::vector<T>& a) {
	if (a.size() < 2) {
		return;
	}
	T maxEl = a[0];
	for (size_t i = 1; i < a.size(); ++i) {
		if (maxEl < a[i]) {
			maxEl = a[i];
		}
	}
	std::vector<T> c(maxEl + 1);
	for (T i = 0; i <= maxEl; ++i)
		c[i] = 0;
	for (size_t i = 0; i < a.size(); ++i)
		c[a[i]]++;
	for (T i = 1; i <= maxEl; ++i)
		c[i] += c[i - 1];
	std::vector<T> b(a.size());
	size_t i = a.size();
	do {
		--i;
		--c[a[i]];
		b[c[a[i]]] = a[i];
	} while (i > 0);
	for (size_t i = 0; i < a.size(); i++)
		a[i] = b[i];
}

std::vector<char> Part(std::vector < std::vector<long>>g) {
	std::vector<char> part(g.size(), -1);
	std::vector<int> q(g.size()); 
	for (int st = 0; st < g.size(); ++st)
		if (part[st] == -1) {
			int h = 0, t = 0;
			q[t++] = st;
			part[st] = 0;
			while (h < t) {
				int v = q[h++];
				for (size_t i = 0; i < g[v].size(); ++i) {
					int to = g[v][i];
					if (part[to] == -1)
						part[to] = !part[v], q[t++] = to;
				}
			}
		}
	return part;
}


bool DFS(long v, std::vector<std::vector<long>>& g, std::vector<bool>& used, std::vector<long>& matching) {
	if (used[v]) {
		return false;
	}
	used[v] = true;
	for (size_t i = 0; i < g[v].size(); ++i) {
		if (matching[g[v][i]] == -1 || DFS(matching[g[v][i]], g, used, matching)) {
			matching[g[v][i]] = v;
			return true;
		}
	}
	return false;
}

std::vector <std::pair<long,long>> KunAlgorithm(std::vector<std::vector<long>>& g) {
	std::vector<long> matching;
	matching.assign(g.size(), -1);
	std::vector<bool> used;
	for (size_t i = 0; i < g.size(); ++i) {
		used.assign(g.size(), false);
		DFS(i, g, used, matching);
	}
	used.assign(g.size(), false);
	std::vector <std::pair<long, long>> Pairs;
	for (size_t i = 0; i < matching.size(); ++i) {
		if (matching[i] != -1 ) {
			Pairs.push_back(std::make_pair(i, matching[i]));
		}
	}
	for (size_t i = 0; i < Pairs.size(); ++i) {
		if (Pairs[i].first > Pairs[i].second) {
			std::swap(Pairs[i].first, Pairs[i].second);
		}
	}
	CountingSortPairs(Pairs);
	return Pairs;
}

int main() {
	std::ios::sync_with_stdio(false);
	long n, m;
	std::cin >> n >> m;
	std::vector<std::vector<long>> g(n);
	std::vector<bool> left_side, right_side;
	left_side.assign(n, false);
	right_side.assign(n, false);
	long from, to;
	for (long i = 0; i < m; ++i) {
		std::cin >> from >> to;
		g[from - 1].push_back(to - 1);
		g[to - 1].push_back(from - 1);
	}
	std::vector<char> part = Part(g);
	for (size_t i = 0; i < g.size(); ++i) {
		if (g[i].size() != 0)
			CountingSort(g[i]);
	}
	std::vector<std::vector<long>> g1(g.size());
	for (int i = 0; i < g.size(); ++i) {
		if (!part[i]) {
			g1[i] = g[i];
		}
	}
	std::vector<std::pair<long, long>>Pairs = KunAlgorithm(g1);
	std::cout << Pairs.size() << std::endl;
	for (size_t i = 0; i < Pairs.size(); ++i) {
		std::cout << Pairs[i].first+1 << ' ' << Pairs[i].second+1 << std::endl;
	}
	return 0;
}
