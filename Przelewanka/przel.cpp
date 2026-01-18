#include <bits/stdc++.h>
using namespace std;

//depends which of this is more optimal
map <vector <int>, int> visited_beg, visited_end;

int opt_check (vector <int> &var, bool type, queue <vector <int>> &q){ //we are checking if v is visited from end (case type = false) or if is visited from begin (case: type =true)
       if (type == false){
               if (!visited_end.contains (var)){
                       visited_end [var] = visited_end [q.front()] + 1;
                       q.push (var);
                       if (visited_beg.contains (var)) return visited_beg [var] + visited_end [var];
               }
       }
       else {
               if (!visited_beg.contains (var)){
                       visited_beg [var] = visited_beg [q.front()] + 1;
                       q.push (var);
                       if (visited_end.contains (var)) return visited_beg [var] + visited_end [var];
               }
       }
       return -1;
}

//next three functions makes reverse pouring, 
int rev_pour_from_to (vector <int> &x, queue<vector <int>> &q){
        vector <int> var = q.front();
        int n = (int) x.size(), output;
        for (int i = 0; i < n; ++i){
                if (var[i] == 0){ //we poured water from i to j and nothing left
                for (int j = 0; j < n; ++j){
                        if (i!=j){
                                for (int h = 1; h <= min (q.front()[j], x[i]); ++h){
                                        var[i] ++;
                                        var[j] --;
                                        output = opt_check (var, false, q);
                                        if (output >= 0) return output;
                                }
                        }
                var[i] = q.front()[i];
                var[j] = q.front()[j];
                } }
                else if (var[i] == x[i]){ //we poured water from j to i and we filled up this entirely
                        for (int j = 0; j < n; ++j){
                                if (i != j){
                                for (int h = 1; h <= min (x[j] - q.front()[j], x[i]); ++h){
                                        var[j] ++;
                                        var[i] --;
                                        output = opt_check (var, false, q);
                                        if (output >= 0) return output;
                                }}
                                var[i] = q.front()[i];
                                var[j] = q.front()[j];
                        }
                }
        }
        var.clear();
        return -1;
}

int rev_pour_in (vector <int> &x, queue <vector <int>> &q){
        vector <int> var = q.front();
        int n = (int) x.size(), output;
        for (int i = 0; i < n; ++i){
                if (var[i] == x[i]){
                        for (int j = 0; j < x[i]; ++j){
                                var[i]--;
                                output = opt_check (var, false, q);
                                if (output >= 0) return output;
                        }
                        var[i] = q.front()[i];
                }
        }
        var.clear();
        return -1;
}

int rev_pour_out (vector <int> &x, queue <vector <int>> &q){
        vector <int> var = q.front();
        int n = (int) x.size(), output;
        for (int i = 0; i < n; ++i){
                if (var[i] == 0){
                        for (int j = 1; j <= x[i]; ++j){
                                var[i]++;
                                output = opt_check (var, false, q);
                                if (output >= 0) return output;
                        }
                        var[i] = q.front()[i];
                }
        }
        var.clear();
        return -1;
}

//bfs from expected state
int bfs_end(vector <int> &x, queue <vector <int>> &from_end){
        int m = (int) from_end.size(), output;
        for (int i = 0; i < m; ++i){
                //cout << from_end.front()[1] <<" " << from_end.front()[2] << "end\n";
                output = rev_pour_in (x, from_end);
                if (output >= 0) return output;
                output = rev_pour_out (x, from_end);
                if (output >= 0) return output;
                output = rev_pour_from_to (x, from_end);
                if (output >= 0) return output;
                from_end.pop();
        }
        return -1;
}

//these three functions pour water in standard way
int pour_from_to (vector <int> &x, queue<vector <int>> &q){
        vector <int> var = q.front();
        int n = (int) x.size(), output;
        for (int i = 0; i < n; ++i){
                for (int j = 0; j < n; ++j){
                        if (i!=j){
                                //we pour water from i to j
                                output = min (q.front()[i] + q.front()[j], x[j]);
                                var[j] = output;
                                var[i] = q.front()[i] + q.front()[j] - output;
                                output = opt_check (var, true, q);
                                if (output >= 0) return output;
                                var[i] = q.front()[i];
                                var[j] = q.front()[j];
                        }
                }
        }
        var.clear();
        return -1;
}

int pour_in (vector <int> &x, queue <vector <int>> &q){
        vector <int> var = q.front();
        int n = (int) x.size(), output;
        for (int i = 0; i < n; ++i){
                var[i] = x[i];
                output = opt_check (var, true, q);
                if (output >= 0) return output;
                var[i] = q.front()[i];
        }
        var.clear();
        return -1;
}

int pour_out (vector <int> &x, queue <vector <int>> &q){
        vector <int> var = q.front();
        int n = (int) x.size(), output;
        for (int i = 0; i < n; ++i){
                var[i] = 0;
                output = opt_check (var, true, q);
                if (output >= 0) return output;
                var[i] = q.front()[i];
        }
        var.clear();
        return -1;
}

int bfs_beg (vector <int> &x, queue <vector <int>> &from_beg){
        int m = (int) from_beg.size(), output;
        for (int i = 0; i < m; ++i){
                output = pour_in (x, from_beg);
                if (output >= 0) return output;
                output = pour_out (x, from_beg);
                if (output >= 0) return output;
                output = pour_from_to (x, from_beg);
                if (output >= 0) return output;
                from_beg.pop();
        }
        return -1;
}

//cases when y[i] are all zeros or equal to x[i]
int edgecases (vector <int> &x, vector <int> &y, int n){
        int output = 0, count = 0;
        for (int i = 0; i < n; ++i){
                if (y[i] == x[i] || y[i] ==0){
                        ++count;
                        if (y[i] == x[i] && y[i] != 0) ++output;
                }
        }
        if (count == n) return output;
        return -1;
}

int answer (vector <int> &x, vector <int> &y, int n){
        int output = 0;
        //some edge cases
        output = edgecases (x, y, n);
        if (output >= 0) return output;

        queue <vector <int>> from_beg, from_end;
        vector <int> current;
        for (int i = 0; i < n; ++i) current.push_back (0);
        from_beg.push (current);       visited_beg [current] = 0;
        from_end.push (y);             visited_end [y] = 0;
        //we take the smaller queue to optimize our backtrack
        while (!from_beg.empty() || !from_end.empty()){
                if ((from_beg.size() <= from_end.size() && !from_beg.empty()) || from_end.empty()){
                        output = bfs_beg (x, from_beg);
                        if (output >= 0) break;
                }
                else {
                        output = bfs_end (x, from_end);
                        if (output >= 0) break;
                }
        }
        while (!from_beg.empty()) from_beg.pop();
        while (!from_end.empty()) from_end.pop();
        return output;
}

bool check (vector <int> &x, vector <int> &y, int n){
        //checks if exist one glass that is empty or full
        int count = 0;
        for (int i = 0; i < n; ++i) if (y[i] == 0 || y[i] == x[i]) count++;
        if (count == 0) return false;
        //checks if gcd of sequence x divide every y[i]
        int nwd = 0;
        for (int i = 0; i < n; ++i) nwd = gcd (nwd, x[i]);
        if (nwd == 0) nwd = 1;
        for (int i = 0; i < n; ++i) x[i] /= nwd;
        for (int i = 0; i < n; ++i){
                if (y[i] % nwd) return false;
                y[i] /= nwd;
        }
        return true;
}

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0); cout.tie(0);
        int n;
        if (!(cin >> n)) cerr << "not sufficient input";
        vector <int> x, y;
        x.resize (n); y.resize (n);
        for (int i = 0; i < n; ++i) if (!(cin >> x[i] >> y[i])) cerr << "not suffcient input";
        //edge cases
        if (n == 0) { cout << "0\n"; return 0; }
        if (n == 1) {
                if (x[0] == y[0]) cout << "1\n";
                else if (y[0] == 0) cout << "0\n";
                else cout<< "-1\n";
                return 0;
        }
        //checking if its possible to satisfy the condition
        if (!check (x, y, n)){
                cout << "-1\n";
                return 0;
        }
        //cout << pref[0] << " " <<pref[1] << " " <<pref [2] << "\n";
        cout << answer (x, y, n) << "\n";
        visited_beg.clear(); visited_end.clear();
        x.clear(); y.clear();
	return 0;
}

