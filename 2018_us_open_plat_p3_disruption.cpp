#include<bits/stdc++.h>
using namespace std;


using ll = long long;

const int INF = 1e9+9;
const int N = 1e5+5, LG = 20;

int n;
vector<int> adj[N];

int tt, tin[N], tout[N], sz[N], par[N][LG], hvc[N];

void dfs(int u, int p) {
  tin[u] = tt++, sz[u] = 1, par[u][0] = p;
  for (int j = 1; j < LG; ++j) {
    par[u][j] = par[par[u][j - 1]][j - 1];
  }
  int mx = 0;
  for (auto v: adj[u]) {
    if (v != p) {
      dfs(v, u);
      sz[u] += sz[v];
      if (sz[v] > mx) {
        mx = sz[v];
        hvc[u] = v;
      }
    }
  }
  tout[u] = tt - 1;
}

int ch_cnt, idx_cnt, chno[N], chd[N], idx[N];

void hld(int u, int p) {
  if (chd[ch_cnt] == -1) {
    chd[ch_cnt] = u;
  }
  chno[u] = ch_cnt, idx[u] = idx_cnt++;
  if (hvc[u] != -1) {
    hld(hvc[u], u);
  }
  for (auto v: adj[u]) {
    if (v != p and v != hvc[u]) {
      ch_cnt++;
      hld(v, u);
    }
  }
}

bool is_anc(int u, int v) {
  return (tin[u] <= tin[v] and tout[v] <= tout[u]);
}

int get_lca(int u, int v) {
  for (int j = LG - 1; j >= 0; --j) {
    if (!is_anc(par[u][j], v)) {
      u = par[u][j];
    }
  }
  return u;
}

vector<int> st(3 * N, INF);

void make_less(int l, int r, int x, int u=1, int s=0, int e=n-1){
  if(l>e or r<s) return ;
  if(l<=s and e<=r){
    st[u] = min(st[u], x);
    return ;
  }
  int v=2*u, w=2*u+1, m=(s+e)/2;
  if(st[u]!=INF){
    st[v] = min(st[v], st[u]);
    st[w] = min(st[w], st[u]);
    st[u]=INF;
  }
  make_less(l,r,x,v,s,m);
  make_less(l,r,x,w,m+1,e);
}

int at(int i, int u=1, int s=0, int e=n-1){
  if(s==e)  return st[u];
  int v=2*u, w=2*u+1, m=(s+e)/2;
  if(st[u]!=INF){
    st[v] = min(st[v], st[u]);
    st[w] = min(st[w], st[u]);
    st[u]=INF;
  }
  if(i<=m)  return at(i,v,s,m);
  else  return at(i,w,m+1,e);
}

void pupdate_up(int u, int anc, int x) {
  if (chno[u] == chno[anc]) {
    make_less(idx[anc], idx[u], x);
    return ;
  }
  make_less(idx[chd[chno[u]]], idx[u], x);
  pupdate_up(par[chd[chno[u]]][0], anc, x);
}

void pupdate(int u, int v, int x) {
  if (is_anc(u, v)) {
    int l1 = get_lca(v, u);
    pupdate_up(v, l1, x);
  }
  else if (is_anc(v, u)) {
    int l2 = get_lca(u, v);
    pupdate_up(u, l2, x);
  }
  else {
    int l1 = get_lca(u, v);
    pupdate_up(u, l1, x);

    int l2 = get_lca(v, u);
    pupdate_up(v, l2, x);

  }
}

void solve() {

  freopen("disrupt.in", "r", stdin);
  freopen("disrupt.out", "w", stdout);

  int q;  cin >> n >> q;

  vector<array<int, 2>> edges(n - 1);
  for (int e = 0; e < n - 1; ++e) {
    auto &[u, v] = edges[e];
    cin >> u >> v;  u--, v--;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  for (int u = 0; u < n; ++u) {
    hvc[u] = -1;
  }
  tt = 0;
  dfs(0, 0);

  for (int u = 0; u < n; ++u) {
    chd[u] = -1;
  }
  ch_cnt = 0, idx_cnt = 0;
  hld(0, 0);

  while (q--) {
    int u, v, c;  cin >> u >> v >> c;  u--, v--;
    pupdate(u, v, c);
  }

  for (int e = 0; e < n - 1; ++e) {
    auto [u, v] = edges[e];
    if (par[v][0] != u)  swap(u, v);
    int ans = at(idx[v]);
    if (ans == INF)  ans = -1;
    cout << ans << "\n";
  }

}

int main(){
  ios::sync_with_stdio(0), cin.tie(0);

  int tc = 1;
  for (int t = 1; t <= tc; ++t) {
    solve();
  }
}
