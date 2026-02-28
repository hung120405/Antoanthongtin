#include <array>
#include <cctype>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

static const string ALPHABET_25 = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; // no J

static string normalize_letters(const string& text) {
    // Keep A-Z only, uppercase, replace J -> I
    string out;
    out.reserve(text.size());

    for (unsigned char ch : text) {
        if (isalpha(ch)) {
            char up = static_cast<char>(toupper(ch));
            if (up == 'J') up = 'I';
            out.push_back(up);
        }
    }
    return out;
}

struct KeySquare {
    array<array<char, 5>, 5> square{};
    unordered_map<char, pair<int, int>> pos; // char -> (row, col)
};

static KeySquare build_key_square(const string& key) {
    string k = normalize_letters(key);

    bool seen[26] = {false};
    vector<char> stream;
    stream.reserve(25);

    auto push_if_ok = [&](char ch) {
        if (ch < 'A' || ch > 'Z') return;
        if (ch == 'J') return;
        int idx = ch - 'A';
        if (!seen[idx] && ALPHABET_25.find(ch) != string::npos) {
            seen[idx] = true;
            stream.push_back(ch);
        }
    };

    for (char ch : k) push_if_ok(ch);
    for (char ch : ALPHABET_25) push_if_ok(ch);

    KeySquare ks;
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            char ch = stream[r * 5 + c];
            ks.square[r][c] = ch;
            ks.pos[ch] = {r, c};
        }
    }
    return ks;
}

static vector<pair<char, char>> prepare_plaintext(const string& plaintext) {
    // Rules:
    // - normalize (A-Z only, uppercase, J->I)
    // - if a == b: pair (a, 'X') and advance by 1
    // - if odd length: pad last with 'X'
    string text = normalize_letters(plaintext);

    vector<pair<char, char>> pairs;
    pairs.reserve((text.size() + 1) / 2);

    for (size_t i = 0; i < text.size();) {
        char a = text[i];
        char b = (i + 1 < text.size()) ? text[i + 1] : 'X';

        if (a == b) {
            pairs.push_back({a, 'X'});
            i += 1;
        } else {
            pairs.push_back({a, b});
            i += 2;
        }
    }

    return pairs;
}

static string encrypt_playfair(const string& plaintext, const string& key) {
    KeySquare ks = build_key_square(key);
    auto pairs = prepare_plaintext(plaintext);

    string out;
    out.reserve(pairs.size() * 2);

    for (auto [a, b] : pairs) {
        auto ita = ks.pos.find(a);
        auto itb = ks.pos.find(b);
        if (ita == ks.pos.end() || itb == ks.pos.end()) {
            continue;
        }

        int ra = ita->second.first, ca = ita->second.second;
        int rb = itb->second.first, cb = itb->second.second;

        if (ra == rb) {
            // same row: shift right
            out.push_back(ks.square[ra][(ca + 1) % 5]);
            out.push_back(ks.square[rb][(cb + 1) % 5]);
        } else if (ca == cb) {
            // same column: shift down
            out.push_back(ks.square[(ra + 1) % 5][ca]);
            out.push_back(ks.square[(rb + 1) % 5][cb]);
        } else {
            // rectangle: swap columns
            out.push_back(ks.square[ra][cb]);
            out.push_back(ks.square[rb][ca]);
        }
    }

    return out;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string key;
    string plaintext;

    cout << "Nhap khoa (key): " << flush;
    getline(cin, key);

    cout << "Nhap ban ro (plaintext): " << flush;
    getline(cin, plaintext);

    cout << "Ciphertext: " << encrypt_playfair(plaintext, key) << "\n";
    return 0;
}
