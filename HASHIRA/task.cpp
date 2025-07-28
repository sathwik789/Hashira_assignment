#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <iomanip>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

typedef long long ll;
typedef pair<ll, ll> Point;

// Euclidean algorithm for GCD
ll gcd(ll a, ll b) {
    while (b) {
        ll t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Least Common Multiple
ll lcm(ll a, ll b) {
    return a / gcd(a, b) * b;
}

// Fraction class for precision
struct Fraction {
    ll num, den;

    Fraction(ll n = 0, ll d = 1) {
        ll g = gcd(abs(n), abs(d));
        num = n / g;
        den = d / g;
        if (den < 0) {
            num = -num;
            den = -den;
        }
    }

    Fraction operator+(const Fraction& other) const {
        ll lcm_den = lcm(den, other.den);
        ll sum_num = num * (lcm_den / den) + other.num * (lcm_den / other.den);
        return Fraction(sum_num, lcm_den);
    }

    Fraction operator*(const Fraction& other) const {
        return Fraction(num * other.num, den * other.den);
    }

    Fraction operator/(const Fraction& other) const {
        return Fraction(num * other.den, den * other.num);
    }

    ll toInt() const {
        return llround((long double)num / den);
    }
};

// Converts string from given base to decimal
ll convertToDecimal(const string& value, int base) {
    ll result = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c))
            digit = c - '0';
        else if (isalpha(c))
            digit = tolower(c) - 'a' + 10;
        else
            continue;

        if (digit >= base) return -1;
        result = result * base + digit;
    }
    return result;
}

// Lagrange interpolation to get constant term (x=0)
Fraction lagrangeInterpolation(const vector<Point>& points) {
    Fraction result(0, 1);
    int k = points.size();

    for (int i = 0; i < k; ++i) {
        ll xi = points[i].first;
        ll yi = points[i].second;
        Fraction term(yi, 1);

        for (int j = 0; j < k; ++j) {
            if (i != j) {
                ll xj = points[j].first;
                term = term * Fraction(-xj, xi - xj);
            }
        }
        result = result + term;
    }

    return result;
}

// Generate all combinations of size k
void generateCombinations(const vector<Point>& points, int k, int idx, vector<Point>& curr, vector<vector<Point>>& allComb) {
    if (curr.size() == k) {
        allComb.push_back(curr);
        return;
    }
    for (int i = idx; i < points.size(); ++i) {
        curr.push_back(points[i]);
        generateCombinations(points, k, i + 1, curr, allComb);
        curr.pop_back();
    }
}

// Solve one test case
ll solveSecretFromJson(const json& j) {
    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<Point> points;
    for (auto& el : j.items()) {
        if (el.key() == "keys") continue;

        ll x = stoll(el.key());
        int base = stoi(el.value()["base"].get<string>());
        string valueStr = el.value()["value"];
        ll y = convertToDecimal(valueStr, base);
        points.push_back({x, y});
    }

    vector<vector<Point>> allComb;
    vector<Point> temp;
    generateCombinations(points, k, 0, temp, allComb);

    unordered_map<ll, int> frequency;
    for (auto& comb : allComb) {
        Fraction c = lagrangeInterpolation(comb);
        ll rounded = c.toInt();
        frequency[rounded]++;
    }

    ll secret = 0;
    int maxFreq = 0;
    for (auto& p : frequency) {
        if (p.second > maxFreq) {
            maxFreq = p.second;
            secret = p.first;
        }
    }

    return secret;
}

int main() {
    ifstream file("input.JSON");
    if (!file.is_open()) {
        cerr << "Failed to open input.JSON\n";
        return 1;
    }

    json inputJson;
    file >> inputJson;

    int testCaseNum = 1;
    for (auto& test : inputJson.items()) {
        ll secret = solveSecretFromJson(test.value());
        cout << "Secret from Test Case " << testCaseNum++ << ": " << secret << endl;
    }

    return 0;
}
