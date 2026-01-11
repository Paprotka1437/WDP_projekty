#include <iostream>
#include <vector>
#include <deque>
#include <cassert>
using namespace std;

/* function compares the quality of two intervals, by squaring it, and returns true if the quality of A
is strictly greater than the quality of B */
 bool comp_quality (pair <int, int> A, pair <int, int> B, vector <pair <int, int>> &points){
                  __int128_t C = points[A.second].first - points[A.first].first,
                             D = points[B.second].first - points[B.first].first,
                             E = A.second - A.first + 1, F = B.second - B.first + 1;
                  return C*C*F > D*D*E;
 }

//function decides wchich interval contains point i have the greatest quality
void answer (int n, vector <pair <int, int>> &points, vector <pair <int, int>> &intervals){
        deque <int> maksi;
        int end = 0; //points to first interval that we haven't used yet
        for (int i = 0; i < n; ++i){
                if (intervals[end].first == i){
                        /*loop removes from back of deque every interval that have quality worse than
                        new interval, because every of this interval ends up earlier than new one so it 
                        cant be answer for any point i */
                        while (!maksi.empty() && comp_quality (intervals[end], intervals[maksi.back()], points)){
                                maksi.pop_back();
                        }
                        maksi.push_back (end);
                        ++end;
                }
                //if intervalse ends up, we simply remove it
                if (intervals[maksi.front()].second < i) maksi.pop_front();
                cout << intervals[maksi.front()].first + 1 << " " << intervals[maksi.front()].second + 1 << "\n";
        }
        maksi.clear();
        return;
}

//function determines every U-strict maximal interval
void solve (int n, int U, vector <pair <int, int>> &points){
        deque <int> maksi, mini;
        vector <pair <int, int>> intervals;
        int beg = 0;// beg and end are the limits of interval
        for (int end = 0; end < n; ++end){
                /* using deque to efficiently determine the next maximum second coordinate on 
                current interval, first we remove the points that won't be maximum never */
                while (!maksi.empty() && points[maksi.back()].second < points[end].second){
                        maksi.pop_back();
                }
                //the same for mini
                while (!mini.empty() && points[mini.back()].second > points[end].second){
                        mini.pop_back();
                }
                maksi.push_back (end); mini.push_back (end);
                /* if on maksi is only one element it means that maximum is point end, so we must
                check if max-min exceeds U, and if it happens we know that we find new U-strict
                maximal interval. Next we moving beg to the moment when max-min on interval is below
                U again */
                if (maksi.size() == 1){
                        if (points[end].second - points[mini.front()].second > U){
                                intervals.push_back ({beg, end - 1});
                        }
                        while (points[end].second - points[mini.front()].second > U){
                                if (mini.front() == beg) mini.pop_front();
                                ++beg;
                        }
                }
                //doing the same for minimun on interval
                if (mini.size() == 1){
                        if (points[maksi.front()].second - points[end].second > U){
                                intervals.push_back ({beg, end - 1});
                        }
                        while (points[maksi.front()].second - points[end].second > U){
                                if (maksi.front() == beg) maksi.pop_front();
                                ++beg;
                        }
                }
        }
        intervals.push_back ({beg, n-1});
        intervals.push_back ({n, n}); //to avoid calling outside of vector in function answer
        answer (n, points, intervals);
        maksi.clear(); mini.clear(); intervals.clear();
        return;
}

//functions scans input
bool scanning (int n, vector <pair <int, int>> &points){
        points.resize (n);
        for (int i = 0; i < n; ++i){
                if (!(cin >> points[i].first >> points[i].second)){
                        cerr << "insufficient input \n";
                        return false;
                }
                if (i >= 1 && points[i].first <= points[i - 1].first){
                        perror ("x is not strictly increasing \n");
                        return false;
                }
        }
        return true;
}

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0); cout.tie(0);
        int n, U;
        if (!(cin >> n >> U)) cerr << "insufficient input \n";
        if (n < 1 || U < 0){
                perror ("wrong input \n");
                return 1;
        }
        vector <pair <int, int>> points;
        if (!scanning (n, points)) return 1;
        solve (n, U, points);
        points.clear();
	return 0;
}

