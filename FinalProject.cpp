#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <limits>

using namespace std;

// ============================================================
//  CONSTANTS
// ============================================================
const int MAX_PHRASES  = 30;
const int MAX_PROVERBS = 30;
const int MAX_TABOOS   = 30;
const int MAX_TERMS    = 30;
const int MAX_ASPECTS  = 30;
const int MAX_FOODS    = 30;
const int MAX_PEOPLE   = 30;

// ============================================================
//  DATA STRUCTURES
// ============================================================
struct Phrase       { string english; string translation; };
struct Proverb      { string text;    string meaning;  string context; };
struct CulturalTaboo{ string rule;    string reason;   string alternative; };
struct DomainTerm   { string english; string translation; string domain; };
struct CulturalAspect { string title; string description; };
struct Food         { string name;    string description; };
struct Person       { string role;    string description; };

// ============================================================
//  HELPER FUNCTIONS
// ============================================================
int readInt() {
    int n = 0;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return n;
}
char readChar() {
    char c = ' ';
    cin >> c;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return c;
}
string readLine() {
    string s;
    getline(cin, s);
    return s;
}
void line(int width = 50, char ch = '-') {
    cout << string(width, ch) << "\n";
}
void pause() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

// ============================================================
//  BADGE STRUCTURE
// ============================================================
struct Badge {
    string name;
    string description;
    bool   earned;
    Badge(string n, string d) : name(n), description(d), earned(false) {}
};

// ============================================================
//  QUIZ & VOCAB STRUCTURES
// ============================================================
struct QuizQuestion {
    string question;
    string options[4];
    int    correctIndex;
    string explanation;


    QuizQuestion() : correctIndex(0) {}
    QuizQuestion(string q, string o[4], int ci, string exp)
        : question(q), correctIndex(ci), explanation(exp) {
        for (int i = 0; i < 4; i++) options[i] = o[i];
    }
};

struct VocabWord {
    string english, punjabi, balochi, sindhi, urdu, pashto, kashmiri, gilgiti, example;
    VocabWord() {}
    VocabWord(string e,string p,string b,string s,string u,
              string pa,string k,string g,string ex)
        : english(e),punjabi(p),balochi(b),sindhi(s),urdu(u),
          pashto(pa),kashmiri(k),gilgiti(g),example(ex) {}
};

struct LeaderboardEntry {
    string username; int score; int level; int streak;
    LeaderboardEntry(string u,int s,int l,int st)
        : username(u),score(s),level(l),streak(st) {}
};

// ============================================================
//  USER CLASS
// ============================================================
class User {
private:
    string        username;
    string        password;
    string        email;           
    string        lastLoginDate;
    int           level;
    int           totalScore;
    int           streak;
    vector<Badge> badges;
    set<string>   masteredWords;
    set<string>   weakAreas;

    void checkLevelUp() {
        if (totalScore >= 500 && level == 1) {
            level = 2;
            cout << "\n*** LEVEL UP! You are now Intermediate! ***\n";
            awardBadge("Level Up");
        }
        if (totalScore >= 1500 && level == 2) {
            level = 3;
            cout << "\n*** LEVEL UP! You are now Advanced! ***\n";
            awardBadge("Punjabi Pro");
        }
    }

public:
    User() : level(1), totalScore(0), streak(0) { initBadges(); }

    User(string u, string p, string em = "")
        : username(u), password(p), email(em), level(1), totalScore(0), streak(0) {
        initBadges();
        lastLoginDate = "";  // will be set on first login with the help of updateStreak
    }

    void initBadges() {
        badges.clear();
        badges.push_back(Badge("First Steps",      "Complete your first lesson"));
        badges.push_back(Badge("Quiz Warrior",      "Score 100% on any quiz"));
        badges.push_back(Badge("Streak 10",         "Maintain a 10-day streak"));
        badges.push_back(Badge("Streak 30",         "Maintain a 30-day (1 month) streak"));
        badges.push_back(Badge("Streak 60",         "Maintain a 60-day (2 month) streak"));
        badges.push_back(Badge("Streak 365",        "Maintain a 365-day (1 year) streak"));
        badges.push_back(Badge("Vocabulary King",   "Master 50 words"));
        badges.push_back(Badge("Level Up",          "Reach Intermediate level"));
        badges.push_back(Badge("Punjabi Pro",       "Reach Advanced level"));
        badges.push_back(Badge("Culture Explorer",  "Add a custom culture aspect"));
    }

    // ---- Getters ----
    string        getUsername()      const { return username; }
    string        getPassword()      const { return password; }
    string        getEmail()         const { return email; }
    int           getLevel()         const { return level; }
    int           getTotalScore()    const { return totalScore; }
    int           getStreak()        const { return streak; }
    string        getLastLogin()     const { return lastLoginDate; }
    set<string>   getWeakAreas()     const { return weakAreas; }
    set<string>   getMasteredWords() const { return masteredWords; }
    vector<Badge> getBadges()        const { return badges; }

    string getLevelName() const {
        if (level == 1) return "Beginner";
        if (level == 2) return "Intermediate";
        return "Advanced";
    }

    // ---- Setters ----
    void setUsername(string u)  { username = u; }
    void setPassword(string p)  { password = p; }
    void setEmail(string e)     { email = e; }
    void setLevel(int l)        { level = l; }
    void setTotalScore(int s)   { totalScore = s; }
    void setStreak(int s)       { streak = s; }
    void setLastLogin(string d) { lastLoginDate = d; }

    // ---- Behaviour ----
    void addScore(int pts) { totalScore += pts; checkLevelUp(); }

    void addMasteredWord(const string& w) {
        masteredWords.insert(w);
        if ((int)masteredWords.size() >= 50) awardBadge("Vocabulary King");
    }

    void addWeakArea(const string& t)    { weakAreas.insert(t); }
    void removeWeakArea(const string& t) { weakAreas.erase(t); }

    void awardBadge(const string& name) {
        for (auto& b : badges) {
            if (b.name == name && !b.earned) {
                b.earned = true;
                cout << "\n[BADGE UNLOCKED] " << b.name
                     << " -- " << b.description << "\n";
            }
        }
    }

    // Check streak-based badges
    void checkStreakBadges() {
        if (streak >= 10)  awardBadge("Streak 10");
        if (streak >= 30)  awardBadge("Streak 30");
        if (streak >= 60)  awardBadge("Streak 60");
        if (streak >= 365) awardBadge("Streak 365");
    }

    void updateStreak() {
        string today = getCurrentDate();
        if (lastLoginDate.empty() || lastLoginDate == "0000-00-00") {
            // First-ever login: start streak at 1
            streak = 1;
            lastLoginDate = today;
            checkStreakBadges();
        } else if (today != lastLoginDate) {
            // Only increment if it's actually a new calendar day
            streak++;
            lastLoginDate = today;
            checkStreakBadges();
        }
        // Same day login: do nothing (no double-count)
    }

    // Display streak plan status
    void displayStreakPlan() const {
        cout << "\n--- STREAK PLAN STATUS ---\n";
        cout << "  Current streak : " << streak << " days\n";

        struct Plan { int days; string label; };
        Plan plans[] = {
            {10,  "10-Day  Streak Badge"},
            {30,  "1-Month Streak Badge"},
            {60,  "2-Month Streak Badge"},
            {365, "1-Year  Streak Badge"}
        };
        for (auto& pl : plans) {
            int remaining = pl.days - streak;
            if (remaining <= 0)
                cout << "  [EARNED] " << pl.label << "\n";
            else
                cout << "  [" << remaining << " days left] " << pl.label << "\n";
        }
    }

    void displayProfile() const {
        int learned = (int)masteredWords.size();
        int pct     = min(100, learned * 2);

        cout << "\n";
        line(46, '=');
        cout << "              USER PROFILE\n";
        line(46, '=');
        cout << "  Username : " << username       << "\n";
        cout << "  Email    : " << (email.empty() ? "(not set)" : email) << "\n";
        cout << "  Level    : " << getLevelName() << "\n";
        cout << "  Score    : " << totalScore     << "\n";
        cout << "  Streak   : " << streak << " days\n";
        line(46, '-');
        cout << "  Words Mastered : " << learned << " / 50\n";
        int barLen = 20, filled = (learned * barLen) / 50;
        cout << "  Progress  : [";
        for (int i = 0; i < barLen; i++) cout << (i < filled ? '#' : ' ');
        cout << "] " << pct << "%\n";
        line(46, '-');

        // Streak plan
        cout << "  STREAK PLAN:\n";
        struct Plan { int d; string lbl; };
        Plan plans[] = {{10,"10-Day"},{30,"1-Month"},{60,"2-Month"},{365,"1-Year"}};
        for (auto& pl : plans) {
            int rem = pl.d - streak;
            if (rem <= 0)
                cout << "    [EARNED] " << pl.lbl << " badge\n";
            else
                cout << "    [" << rem << " days to go] " << pl.lbl << " badge\n";
        }
        line(46, '-');

        cout << "  Badges Earned:\n";
        bool any = false;
        for (const auto& b : badges)
            if (b.earned) { cout << "    [*] " << b.name << "\n"; any = true; }
        if (!any) cout << "    (none yet - start learning!)\n";
        line(46, '=');
    }

    // Serialise to string (5 lines per user)
    string serialize() const {
        // Line 1: core fields including email
        string s = username + "|" + password + "|" + email + "|"
                 + to_string(level)      + "|"
                 + to_string(totalScore) + "|"
                 + to_string(streak)     + "|"
                 + lastLoginDate + "\n";

        s += "MASTERED:";
        for (const auto& w : masteredWords) s += w + ",";
        s += "\n";

        s += "WEAK:";
        for (const auto& wa : weakAreas) s += wa + ",";
        s += "\n";

        s += "BADGES:";
        for (const auto& b : badges)
            s += b.name + "=" + (b.earned ? "1" : "0") + ",";
        s += "\n";

        return s;
    }

    static string getCurrentDate() {
        time_t now = time(0);
        tm* t = localtime(&now);
        char buf[20];
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
                 1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday);
        return string(buf);
    }
};

// ============================================================
//  FILE HANDLER
// ============================================================
class FileHandler {
private:
    static const string USERS_FILE;
    static const string LEADERBOARD_FILE;
    static const string CUSTOM_CULTURE_FILE;

public:
    // Save (insert or update) a user in users.dat
    static bool saveUser(const User& user) {
        vector<string> kept;
        ifstream fin(USERS_FILE);
        if (fin.is_open()) {
            string ln;
            int skip = 0;
            while (getline(fin, ln)) {
                if (skip > 0) { skip--; continue; }
                if (!ln.empty() && ln.find(user.getUsername() + "|") == 0)
                    skip = 3;   // skip the 3 metadata lines (MASTERED, WEAK, BADGES)
                else
                    kept.push_back(ln);
            }
            fin.close();
        }
        ofstream fout(USERS_FILE, ios::trunc);
        if (!fout.is_open()) return false;
        for (const auto& l : kept)
            if (!l.empty()) fout << l << "\n";
        fout << user.serialize();
        fout.close();
        return true;
    }

    static bool userExists(const string& username) {
        ifstream fin(USERS_FILE);
        if (!fin.is_open()) return false;
        string ln;
        while (getline(fin, ln))
            if (ln.find(username + "|") == 0) return true;
        return false;
    }

    static bool validateLogin(const string& username, const string& password) {
        ifstream fin(USERS_FILE);
        if (!fin.is_open()) return false;
        string ln;
        while (getline(fin, ln)) {
            if (ln.find(username + "|") != 0) continue;
            stringstream ss(ln);
            string u, p;
            getline(ss, u, '|');
            getline(ss, p, '|');
            return (p == password);
        }
        return false;
    }

    static User loadUser(const string& username) {
        User user;
        ifstream fin(USERS_FILE);
        if (!fin.is_open()) return user;
        string ln;
        while (getline(fin, ln)) {
            if (ln.find(username + "|") != 0) continue;

            // Parse: username|password|email|level|score|streak|date
            stringstream ss(ln);
            string u, p, em, lv, sc, st, dt;
            getline(ss, u,  '|');
            getline(ss, p,  '|');
            getline(ss, em, '|');
            getline(ss, lv, '|');
            getline(ss, sc, '|');
            getline(ss, st, '|');
            getline(ss, dt);
            while (!dt.empty() && (dt.back()=='\r' || dt.back()==' ')) dt.pop_back();

            try {
                user.setUsername(u);
                user.setPassword(p);
                user.setEmail(em);
                user.setLevel(stoi(lv));
                user.setTotalScore(stoi(sc));
                user.setStreak(stoi(st));
                user.setLastLogin(dt);
            } catch (...) {}

            // Skip MASTERED, WEAK, BADGES lines
            string tmp;
            getline(fin, tmp);
            getline(fin, tmp);
            getline(fin, tmp);
            break;
        }
        fin.close();
        return user;
    }

    static void updateLeaderboard(const User& user) {
        auto entries = loadLeaderboard();
        bool found = false;
        for (auto& e : entries) {
            if (e.username == user.getUsername()) {
                e.score  = user.getTotalScore();
                e.level  = user.getLevel();
                e.streak = user.getStreak();
                found = true; break;
            }
        }
        if (!found)
            entries.push_back(LeaderboardEntry(user.getUsername(),
                              user.getTotalScore(), user.getLevel(), user.getStreak()));
        sort(entries.begin(), entries.end(),
             [](const LeaderboardEntry& a, const LeaderboardEntry& b){
                 return a.score > b.score; });
        ofstream fout(LEADERBOARD_FILE, ios::trunc);
        for (const auto& e : entries)
            fout << e.username << "|" << e.score << "|"
                 << e.level    << "|" << e.streak << "\n";
        fout.close();
    }

    static vector<LeaderboardEntry> loadLeaderboard() {
        vector<LeaderboardEntry> entries;
        ifstream fin(LEADERBOARD_FILE);
        if (!fin.is_open()) return entries;
        string ln;
        while (getline(fin, ln)) {
            if (ln.empty()) continue;
            stringstream ss(ln);
            string u, s, l, st;
            getline(ss, u, '|'); getline(ss, s, '|');
            getline(ss, l, '|'); getline(ss, st);
            try { entries.push_back(LeaderboardEntry(u, stoi(s), stoi(l), stoi(st))); }
            catch (...) {}
        }
        fin.close();
        return entries;
    }

    // Save a custom culture aspect added by a user
    static void saveCustomAspect(const string& cultureName, const string& username,
                                  const string& title, const string& description) {
        ofstream fout(CUSTOM_CULTURE_FILE, ios::app);
        if (!fout.is_open()) return;
        fout << cultureName << "|" << username << "|" << title << "|" << description << "\n";
        fout.close();
    }

    // Load & display all custom culture aspects
    static void showCustomAspects() {
        ifstream fin(CUSTOM_CULTURE_FILE);
        if (!fin.is_open()) {
            cout << "  (No custom aspects added yet.)\n";
            return;
        }
        string ln;
        bool any = false;
        while (getline(fin, ln)) {
            if (ln.empty()) continue;
            stringstream ss(ln);
            string culture, user, title, desc;
            getline(ss, culture, '|');
            getline(ss, user,    '|');
            getline(ss, title,   '|');
            getline(ss, desc);
            cout << "  [" << culture << " | by " << user << "]\n"
                 << "  " << title << ": " << desc << "\n\n";
            any = true;
        }
        if (!any) cout << "  (No custom aspects added yet.)\n";
        fin.close();
    }
};

const string FileHandler::USERS_FILE         = "users.dat";
const string FileHandler::LEADERBOARD_FILE   = "leaderboard.dat";
const string FileHandler::CUSTOM_CULTURE_FILE= "custom_culture.dat";

// ============================================================
//  GAME BASE CLASS (abstract)
// ============================================================
class Game {
protected:
    string gameName;
    int    score;
public:
    Game(string name) : gameName(name), score(0) {}
    virtual ~Game() {}
    virtual void play()             = 0;
    virtual void showInstructions() = 0;
    int    getScore() const { return score; }
    string getName()  const { return gameName; }
};

// ---- Hangman ----
class HangmanGame : public Game {
private:
    vector<pair<string,string>> wordBank;

    void drawHangman(int w) const {
string stages[7] = {
    "  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========", 
    "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========", 
    "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========" 
};

        cout << stages[min(w, 6)] << "\n";
    }
public:
    HangmanGame() : Game("Hangman") {
        wordBank = {{"paani","water"},{"ghar","home"},{"kitab","book"},
                    {"roti","bread"},{"aam","mango"},{"nadi","river"},
                    {"tara","star"},{"badal","cloud"},{"phul","flower"},
                    {"yaar","friend"},{"chand","moon"},{"sooraj","sun"}};
    }
    void showInstructions() override {
        cout << "\n--- HANGMAN INSTRUCTIONS ---\n";
        cout << "Guess the romanized Punjabi word, one letter at a time.\n";
        cout << "You have 6 wrong guesses before the game ends.\n\n";
    }
    void play() override {
        showInstructions();
        score = 0;
        srand((unsigned int)time(0));
        int idx = rand() % (int)wordBank.size();
        string target = wordBank[idx].first, hint = wordBank[idx].second;
        string display(target.size(), '_');
        set<char> tried;
        int wrong = 0, maxWrong = 6;
        cout << "Hint: " << hint << "  | Word length: " << target.size() << "\n\n";
        while (wrong < maxWrong && display != target) {
            drawHangman(wrong);
            cout << "Word  : ";
            for (char c : display) cout << c << " ";
            cout << "\nTried : ";
            for (char c : tried) cout << c << " ";
            cout << "\nGuess a letter: ";
            char g = tolower(readChar());
            if (tried.count(g)) { cout << "Already tried!\n\n"; continue; }
            tried.insert(g);
            bool hit = false;
            for (int i = 0; i < (int)target.size(); i++)
                if (target[i] == g) { display[i] = g; hit = true; }
            if (hit) cout << "Good guess!\n\n";
            else     { wrong++; cout << "Wrong! (" << wrong << "/" << maxWrong << ")\n\n"; }
        }
        drawHangman(wrong);
        if (display == target) {
            cout << "CORRECT! Word: " << target << "\n";
            score = (maxWrong - wrong) * 10;
            cout << "Points: " << score << "\n";
        } else {
            cout << "Game over! Word was: " << target << "\n";
        }
    }
};

// ---- Word Match ----
class WordMatchGame : public Game {
private:
    vector<pair<string,string>> pairs;
public:
    WordMatchGame() : Game("Word Match") {
        pairs = {{"hello","Sat Sri Akal"},{"water","Paani"},{"bread","Roti"},
                 {"book","Kitaab"},{"house","Ghar"},{"sun","Sooraj"},
                 {"moon","Chand"},{"friend","Yaar"},{"love","Pyaar"},{"teacher","Adhiyaapak"}};
    }
    void showInstructions() override {
        cout << "\n--- WORD MATCH INSTRUCTIONS ---\n";
        cout << "Match English words to their Punjabi meaning.\n";
        cout << "Type 1-4 for your answer. Each correct = 10 pts.\n\n";
    }
    void play() override {
        showInstructions();
        score = 0;
        srand((unsigned int)time(0));
        vector<pair<string,string>> sh = pairs;
        for (int i = (int)sh.size()-1; i > 0; i--) swap(sh[i], sh[rand()%(i+1)]);
        int numQ = min(5, (int)sh.size());
        for (int i = 0; i < numQ; i++) {
            string correct = sh[i].second;
            vector<string> opts; opts.push_back(correct);
            vector<int> used; used.push_back(i);
            while ((int)opts.size() < 4) {
                int r = rand() % (int)sh.size();
                bool dup = false;
                for (int u : used) if (u == r) { dup = true; break; }
                if (!dup) { opts.push_back(sh[r].second); used.push_back(r); }
            }
            for (int k=(int)opts.size()-1;k>0;k--) swap(opts[k],opts[rand()%(k+1)]);
            int ci = 0;
            for (int k=0;k<4;k++) if (opts[k]==correct) ci=k;
            cout << "Q" << (i+1) << "/" << numQ << " - English: \"" << sh[i].first << "\"\n";
            for (int k=0;k<4;k++) cout << "  " << (k+1) << ". " << opts[k] << "\n";
            cout << "Answer (1-4): ";
            int ch = readInt();
            if (ch-1 == ci) { cout << "Correct!\n\n"; score += 10; }
            else             cout << "Wrong! Answer: " << correct << "\n\n";
        }
        cout << "Word Match done! Score: " << score << "/" << (numQ*10) << "\n";
    }
};

// ============================================================
//  ABSTRACT CULTURE CLASS
// ============================================================
class Culture {
private:
    string cultureName, cultureRegion;
    int    populationSize;

    Phrase        phrases[MAX_PHRASES];
    Proverb       proverbs[MAX_PROVERBS];
    CulturalTaboo taboos[MAX_TABOOS];
    DomainTerm    terms[MAX_TERMS];
    CulturalAspect aspects[MAX_ASPECTS];
    Food          foods[MAX_FOODS];
    Person        people[MAX_PEOPLE];

    int phraseCount, proverbCount, tabooCount, termCount,
        aspectCount, foodCount, peopleCount;

public:
    Culture(string name, string region, int population)
        : cultureName(name), cultureRegion(region), populationSize(population),
          phraseCount(0), proverbCount(0), tabooCount(0), termCount(0),
          aspectCount(0), foodCount(0), peopleCount(0) {}

    virtual ~Culture() {}

    virtual string        translate(string e)                       = 0;
    virtual CulturalTaboo getCulturalTaboo(string topic)           = 0;
    virtual Proverb       getProverb(string topic)                 = 0;
    virtual string        getGreeting(int formalityLevel)          = 0;
    virtual string        getDomainTerm(string term, string domain)= 0;

    void addPhrase(Phrase p)       { if (phraseCount  < MAX_PHRASES)  phrases[phraseCount++]  = p; }
    void addProverb(Proverb pr)    { if (proverbCount < MAX_PROVERBS) proverbs[proverbCount++]= pr;}
    void addTaboo(CulturalTaboo t) { if (tabooCount   < MAX_TABOOS)   taboos[tabooCount++]    = t; }
    void addTerm(DomainTerm dt)    { if (termCount    < MAX_TERMS)    terms[termCount++]      = dt;}
    void addAspect(CulturalAspect a){ if (aspectCount < MAX_ASPECTS)  aspects[aspectCount++]  = a; }
    void addFood(Food f)           { if (foodCount    < MAX_FOODS)    foods[foodCount++]      = f; }
    void addPerson(Person p)       { if (peopleCount  < MAX_PEOPLE)   people[peopleCount++]   = p; }

    string getName()        { return cultureName; }
    string getRegion()      { return cultureRegion; }
    int    getPopulation()  { return populationSize; }

    int           getPhraseCount()      { return phraseCount; }
    Phrase        getPhraseAt(int i)    { return phrases[i]; }
    int           getProverbCount()     { return proverbCount; }
    Proverb       getProverbAt(int i)   { return proverbs[i]; }
    int           getTabooCount()       { return tabooCount; }
    CulturalTaboo getTabooAt(int i)     { return taboos[i]; }
    int           getTermCount()        { return termCount; }
    DomainTerm    getTermAt(int i)      { return terms[i]; }
    int           getAspectCount()      { return aspectCount; }
    CulturalAspect getAspectAt(int i)  { return aspects[i]; }
    int           getFoodCount()        { return foodCount; }
    Food          getFoodAt(int i)      { return foods[i]; }
    int           getPersonCount()      { return peopleCount; }
    Person        getPersonAt(int i)    { return people[i]; }

    // Display all culture information
    void displayAll() {
        line(52, '=');
        cout << "  CULTURE: " << cultureName << "  |  Region: " << cultureRegion << "\n";
        line(52, '=');

        cout << "\n[COMMON PHRASES]\n";
        for (int i = 0; i < phraseCount; i++)
            cout << "  " << setw(30) << left << phrases[i].english
                 << " -> " << phrases[i].translation << "\n";

        cout << "\n[PROVERBS & THEIR MEANINGS]\n";
        for (int i = 0; i < proverbCount; i++)
            cout << "  \"" << proverbs[i].text << "\"\n"
                 << "   Meaning : " << proverbs[i].meaning << "\n"
                 << "   Context : " << proverbs[i].context << "\n\n";

        cout << "[CULTURAL TABOOS]\n";
        for (int i = 0; i < tabooCount; i++)
            cout << "  Rule       : " << taboos[i].rule << "\n"
                 << "  Reason     : " << taboos[i].reason << "\n"
                 << "  Alternative: " << taboos[i].alternative << "\n\n";

        cout << "[DOMAIN TERMS]\n";
        for (int i = 0; i < termCount; i++)
            cout << "  [" << terms[i].domain << "] "
                 << terms[i].english << " -> " << terms[i].translation << "\n";

        cout << "\n[CULTURAL ASPECTS]\n";
        for (int i = 0; i < aspectCount; i++)
            cout << "  " << aspects[i].title << ": " << aspects[i].description << "\n";

        cout << "\n[TRADITIONAL FOODS]\n";
        for (int i = 0; i < foodCount; i++)
            cout << "  " << foods[i].name << ": " << foods[i].description << "\n";

        cout << "\n[NOTABLE PEOPLE / ROLES]\n";
        for (int i = 0; i < peopleCount; i++)
            cout << "  " << people[i].role << " - " << people[i].description << "\n";

        line(52, '=');
    }
};

// ============================================================
//  PUNJABI CULTURE
// ============================================================
class PunjabiCulture : public Culture {
public:
    PunjabiCulture() : Culture("Punjabi", "Punjab", 90) {

        // ===== Phrases (20) =====
        addPhrase({"Sat Sri Akal",       "Hello (traditional greeting)"});
        addPhrase({"Tusi kiven ho?",      "How are you?"});
        addPhrase({"Shukriya",            "Thank you"});
        addPhrase({"Subah Bakhair",       "Good Morning"});
        addPhrase({"Shab Bakhair",        "Good Night"});
        addPhrase({"Meharbani",           "Please"});
        addPhrase({"Maafi",               "Sorry"});
        addPhrase({"Haan",                "Yes"});
        addPhrase({"Nahi",                "No"});
        addPhrase({"Dost",                "Friend"});
        addPhrase({"Khandan",             "Family"});
        addPhrase({"Khana",               "Food"});
        addPhrase({"Paani",               "Water"});
        addPhrase({"School",              "School"});
        addPhrase({"Ustad",               "Teacher"});
        addPhrase({"Doctor Sahib",        "Doctor"});
        addPhrase({"Haspatal",            "Hospital"});
        addPhrase({"Adalat",              "Court"});
        addPhrase({"Wakeel",              "Lawyer"});
        addPhrase({"Tera naa ki ae?",     "What is your name?"});

        // ===== Proverbs (20) =====
        addProverb({"Jithay daana, uthay khaana",
                    "You eat where fate takes you", "Destiny"});
        addProverb({"Jis tan lagay so tan jane",
                    "Only the one suffering knows the pain", "Empathy"});
        addProverb({"Neki kar dariya mein daal",
                    "Do good and forget it", "Selfless kindness"});
        addProverb({"Jithay sharam, uthay dharam",
                    "Where there is modesty, there is faith", "Morality"});
        addProverb({"Sach da raah lamba hunda hai",
                    "The path of truth is long", "Honesty"});
        addProverb({"Jindy kaar daany, oo kanwaly wi sayany",
                    "He with grain is wise even if a fool", "Effort"});
        addProverb({"Ilm roshni hai",
                    "Knowledge is light", "Education"});
        addProverb({"Sabar da phal meetha hunda hai",
                    "Patience brings reward", "Perseverance"});
        addProverb({"Dost da sahara, zindagi da sahara",
                    "Friends are life's support", "Friendship"});
        addProverb({"Pyare ghar nu swarg banaunde ne",
                    "Love makes a home heaven", "Family"});
        addProverb({"Insaf bina mulk barbaad hunda hai",
                    "Without justice, nations collapse", "Law"});
        addProverb({"Dil saaf, raah saaf",
                    "A pure heart clears the path", "Purity"});
        addProverb({"Jis da iman, usi da zindagi",
                    "Faith guides life", "Religion"});
        addProverb({"Zuban da zakham sab ton vadda hunda hai",
                    "Words hurt more than wounds", "Speech"});
        addProverb({"Khet da malik fasal da malik",
                    "The farmer owns his crop", "Ownership"});
        addProverb({"Samay da mol samajh",
                    "Value time", "Wisdom"});
        addProverb({"Bachpan da sanskar zindagi bhar chalda hai",
                    "Childhood values last forever", "Upbringing"});
        addProverb({"Jis da sach, usi da raaj",
                    "Truth rules", "Honesty"});
        addProverb({"Jis da sabar, usi da jeet",
                    "Patience wins", "Perseverance"});
        addProverb({"Jis da ilm, usi da roshan",
                    "Knowledge brings light", "Education"});

        // ===== Cultural Taboos (20) =====
        addTaboo({"Don't eat with left hand",            "Considered disrespectful", "Use right hand"});
        addTaboo({"Avoid pointing feet at elders",       "Seen as rude",             "Sit respectfully"});
        addTaboo({"Don't interrupt elders while speaking","Shows lack of respect",   "Wait for your turn"});
        addTaboo({"Avoid public displays of affection",  "Considered inappropriate", "Show respect privately"});
        addTaboo({"Don't waste food",                    "Food is sacred",           "Eat responsibly"});
        addTaboo({"Avoid speaking loudly in holy places","Shows disrespect",         "Speak softly"});
        addTaboo({"Don't mock traditional attire",       "Seen as dishonor",         "Respect clothing"});
        addTaboo({"Avoid criticizing Punjabi food",      "Food is pride",            "Appreciate cuisine"});
        addTaboo({"Don't insult elders",                 "Respect is core value",    "Show humility"});
        addTaboo({"Avoid ignoring guests",               "Hospitality is sacred",    "Serve warmly"});
        addTaboo({"Don't disrespect turban",             "Symbol of honor",          "Handle respectfully"});
        addTaboo({"Avoid gossiping about family",        "Seen as betrayal",         "Protect family honor"});
        addTaboo({"Don't waste water",                   "Water is precious",        "Use carefully"});
        addTaboo({"Avoid disrespecting farmers",         "Farmers are backbone",     "Show gratitude"});
        addTaboo({"Don't argue in Gurdwara",             "Sacred place",             "Stay peaceful"});
        addTaboo({"Avoid eating before elders",          "Shows greed",              "Wait your turn"});
        addTaboo({"Don't disrespect women",              "Equality is valued",       "Show respect"});
        addTaboo({"Avoid ignoring traditions",           "Culture is identity",      "Participate actively"});
        addTaboo({"Don't insult Punjabi music",          "Music is heritage",        "Celebrate it"});
        addTaboo({"Avoid arrogance",                     "Humility is valued",       "Stay grounded"});

        // ===== Domain Terms (20) =====
        addTerm({"Fever",    "Bukhar",         "MEDICAL"});
        addTerm({"Cough",    "Khansi",         "MEDICAL"});
        addTerm({"Medicine", "Dawa",           "MEDICAL"});
        addTerm({"Hospital", "Haspatal",       "MEDICAL"});
        addTerm({"Doctor",   "Doctor Sahib",   "MEDICAL"});
        addTerm({"Court",    "Adalat",         "LEGAL"});
        addTerm({"Lawyer",   "Wakeel",         "LEGAL"});
        addTerm({"Judge",    "Mohtaram Jaj",   "LEGAL"});
        addTerm({"Justice",  "Insaf",          "LEGAL"});
        addTerm({"Police",   "Police",         "LEGAL"});
        addTerm({"School",   "School",         "EDUCATIONAL"});
        addTerm({"Teacher",  "Ustad",          "EDUCATIONAL"});
        addTerm({"Student",  "Talib-e-ilm",    "EDUCATIONAL"});
        addTerm({"Book",     "Kitaab",         "EDUCATIONAL"});
        addTerm({"University","Jamia",         "EDUCATIONAL"});
        addTerm({"Knowledge","Ilm",            "EDUCATIONAL"});
        addTerm({"Law",      "Qanoon",         "LEGAL"});
        addTerm({"Health",   "Sehat",          "MEDICAL"});
        addTerm({"Education","Taleem",         "EDUCATIONAL"});
        addTerm({"Crime",    "Juram",          "LEGAL"});

        // ===== Cultural Aspects (20) =====
        addAspect({"Bhangra Dance",    "A lively folk dance performed during harvest and celebrations."});
        addAspect({"Lassi",            "A traditional yogurt-based drink, often sweet or salty."});
        addAspect({"Phulkari",         "Colorful embroidery art used in shawls and dupattas."});
        addAspect({"Punjabi Weddings", "Known for vibrant rituals, dhol beats, and joyful celebrations."});
        addAspect({"Lohri Festival",   "Celebrated in January with bonfires and harvest songs."});
        addAspect({"Punjabi Cuisine",  "Famous for Makki di Roti, Sarson da Saag, Butter Chicken."});
        addAspect({"Dhol",             "Traditional drum, central to Punjabi celebrations."});
        addAspect({"Turban (Pagri)",   "Symbol of respect and honor worn by Punjabi men."});
        addAspect({"Gurdwara",         "Sikh place of worship, central to Punjabi spiritual life."});
        addAspect({"Hospitality",      "Punjabis are known for warmth and generosity to guests."});
        addAspect({"Basant Festival",  "Spring kite-flying festival celebrated with joy and colors."});
        addAspect({"Punjabi Poetry",   "Rich tradition with Bulleh Shah and Waris Shah."});
        addAspect({"Heer Ranjha",      "Classic Punjabi love story symbolizing eternal romance."});
        addAspect({"Punjabi Music",    "Folk songs, qawwalis, and modern bhangra beats."});
        addAspect({"Harvest Season",   "Celebrated with dances, songs, and communal feasts."});
        addAspect({"Punjabi Attire",   "Salwar kameez, kurta pajama, phulkari dupattas."});
        addAspect({"Punjabi Villages", "Known for green fields, tractors, and simple living."});
        addAspect({"Punjabi Literature","Sufi poetry, folk tales, and modern novels."});
        addAspect({"Punjabi Values",   "Respect for elders, hospitality, and community spirit."});
        addAspect({"Punjabi Festivals","Vaisakhi, Lohri, Basant, and Gurpurab celebrations."});

        // ===== Food Items (20) =====
        addFood({"Makki di Roti & Sarson da Saag","Corn bread with mustard greens, a Punjabi classic."});
        addFood({"Butter Chicken",  "Rich creamy chicken curry loved worldwide."});
        addFood({"Lassi",           "Refreshing yogurt drink, sweet or salty."});
        addFood({"Chole Bhature",   "Spicy chickpeas with fried bread."});
        addFood({"Paneer Tikka",    "Grilled cottage cheese cubes with spices."});
        addFood({"Amritsari Fish",  "Spiced fried fish from Amritsar."});
        addFood({"Tandoori Chicken","Chicken marinated in yogurt and spices, cooked in a tandoor."});
        addFood({"Rajma Chawal",    "Red kidney beans curry served with rice."});
        addFood({"Aloo Paratha",    "Stuffed flatbread with spiced potatoes."});
        addFood({"Dal Makhani",     "Creamy black lentils slow cooked overnight."});
        addFood({"Pakoras",         "Deep-fried fritters made with vegetables."});
        addFood({"Kheer",           "Rice pudding cooked with milk and sugar."});
        addFood({"Gajar ka Halwa",  "Carrot dessert cooked with milk and ghee."});
        addFood({"Pinni",           "Traditional Punjabi sweet made with flour and ghee."});
        addFood({"Achaar",          "Spicy pickles made from mango, lemon, or chili."});
        addFood({"Bhindi Masala",   "Okra cooked with onions and spices."});
        addFood({"Baingan Bharta",  "Smoked eggplant mashed with spices."});
        addFood({"Shahi Paneer",    "Paneer cooked in rich creamy gravy."});
        addFood({"Kadhi Pakora",    "Gram flour curry with fried pakoras."});
        addFood({"Phirni",          "Ground rice dessert served chilled in clay pots."});

        // ===== Cultural / Historical People (20) =====
        addPerson({"Kisaan",        "Farmer - cultivates wheat, rice, and other crops."});
        addPerson({"Ustad",         "Teacher - guides education and learning."});
        addPerson({"Gayak",         "Singer - performs Punjabi folk and bhangra songs."});
        addPerson({"Shaayar",       "Poet - writes Punjabi poetry, like Bulleh Shah."});
        addPerson({"Dholi",         "Drummer - plays the dhol to energize dances."});
        addPerson({"Bawarchi",      "Chef - expert in Punjabi cuisine."});
        addPerson({"Talib-e-ilm",   "Student - learns in schools and universities."});
        addPerson({"Dukaan Daar",   "Shopkeeper - runs local bazaars and shops."});
        addPerson({"Bunai Kar",     "Weaver - makes phulkari embroidery and textiles."});
        addPerson({"Kahani Kar",    "Storyteller - keeps folk tales alive."});
        addPerson({"Granthi",       "Priest - serves in the Gurdwara."});
        addPerson({"Sipahi",        "Soldier - defends the land and people."});
        addPerson({"Vyapari",       "Trader - engages in business and commerce."});
        addPerson({"Doctor Sahib",  "Doctor - provides healthcare."});
        addPerson({"Wakeel",        "Lawyer - represents justice in courts."});
        addPerson({"Abhineta",      "Actor - performs in Punjabi theatre and cinema."});
        addPerson({"Khidari",       "Athlete - plays kabaddi, hockey, and other sports."});
        addPerson({"Karigar",       "Craftsman - makes traditional tools and ornaments."});
        addPerson({"Siyasatdaan",   "Politician - leads communities and governance."});
        addPerson({"Pind da Vasi",  "Villager - represents simple rural life."});
    }

    string translate(string e) override {
        for (int i = 0; i < getPhraseCount(); i++) {
            Phrase p = getPhraseAt(i);
            if (p.english == e) return p.translation;
        }
        return "Translation not found";
    }
    CulturalTaboo getCulturalTaboo(string topic) override {
        for (int i = 0; i < getTabooCount(); i++) {
            CulturalTaboo t = getTabooAt(i);
            if (t.rule.find(topic) != string::npos) return t;
        }
        return {"Not found","N/A","N/A"};
    }
    Proverb getProverb(string topic) override {
        for (int i = 0; i < getProverbCount(); i++) {
            Proverb p = getProverbAt(i);
            if (p.context.find(topic) != string::npos) return p;
        }
        return {"Not found","N/A","N/A"};
    }
    string getGreeting(int f) override {
        switch(f) {
            case 0: return "Oye! Ki haal hai?";
            case 1: return "Sat Sri Akal";
            case 2: return "Namaste Ji";
            case 3: return "Tuhada swagat hai";
            default: return "Sat Sri Akal";
        }
    }
    string getDomainTerm(string e, string d) override {
        for (int i = 0; i < getTermCount(); i++) {
            DomainTerm dt = getTermAt(i);
            if (dt.english == e && dt.domain == d) return dt.translation;
        }
        return "Term not found";
    }
};

// ============================================================
//  ABSTRACT LANGUAGE CLASS
// ============================================================
class Language {
protected:
    User*  currentUser;
    string languageName;

    bool requireLogin() const {
        if (!currentUser) { cout << "Please log in first.\n"; return false; }
        return true;
    }

public:
    Language(string name) : currentUser(nullptr), languageName(name) {}
    virtual ~Language() {}

    virtual void translate(const string& word)         = 0;
    virtual void runQuiz(int level)                    = 0;
    virtual void startGame(int gameChoice)             = 0;
    virtual void showProgress()                        = 0;
    virtual void showLeaderboard()                     = 0;
    virtual void loadModule(int level)                 = 0;
    virtual void dailyChallenge()                      = 0;
    virtual void pronunciationGuide(const string& w)   = 0;
    virtual void revisionQuiz()                        = 0;
    virtual void showVocabulary(const string& cat)     = 0;

    void   setCurrentUser(User* u) { currentUser = u; }
    User*  getCurrentUser()        { return currentUser; }
    string getLanguageName()       { return languageName; }
};

// ============================================================
//  PUNJABI LANGUAGE CLASS
// ============================================================
class Punjabi : public Language {
private:
    map<string, pair<string,string>>  dictionary;
    map<string, vector<VocabWord>>    vocabulary;
    map<int, vector<QuizQuestion>>    quizBank;
    map<string, string>               pronunciationMap;

    void toLower(string& s) const {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
    }

    void runQuizSession(const vector<QuizQuestion>& questions, const string& label) {
        int correct = 0, total = (int)questions.size();
        if (total == 0) { cout << "No questions available.\n"; return; }

        cout << "\n"; line(50,'=');
        cout << "  " << label << " (" << total << " questions)\n";
        line(50,'=');

        for (int i = 0; i < total; i++) {
            const QuizQuestion& q = questions[i];
            cout << "\nQ" << (i+1) << ": " << q.question << "\n";
            cout << "  A. " << q.options[0] << "\n";
            cout << "  B. " << q.options[1] << "\n";
            cout << "  C. " << q.options[2] << "\n";
            cout << "  D. " << q.options[3] << "\n";
            cout << "Answer (A/B/C/D): ";
            char ans = toupper(readChar());
            int idx = ans - 'A';
            if (idx >= 0 && idx < 4 && idx == q.correctIndex) {
                cout << "CORRECT!\n";
                if (!q.explanation.empty()) cout << "  Tip: " << q.explanation << "\n";
                correct++;
            } else {
                cout << "WRONG! Correct: " << (char)('A'+q.correctIndex) << "\n";
                if (!q.explanation.empty()) cout << "  Tip: " << q.explanation << "\n";
            }
        }

        int pct    = (correct*100)/total;
        int points = correct*20;
        cout << "\n"; line(40,'-');
        cout << "Result : " << correct << "/" << total << " (" << pct << "%)\n";
        cout << "Points : +" << points << "\n"; line(40,'-');

        if (currentUser) {
            currentUser->addScore(points);
            if (pct == 100) currentUser->awardBadge("Quiz Warrior");
            if (pct < 60)   currentUser->addWeakArea(label);
            else            currentUser->removeWeakArea(label);
            FileHandler::saveUser(*currentUser);
            FileHandler::updateLeaderboard(*currentUser);
        }
    }

    void initDictionary() {
        dictionary["hello"]       = {"Sat Sri Akal",     "Traditional greeting"};
        dictionary["thank you"]   = {"Dhanyavaad",       "Formal thank you"};
        dictionary["yes"]         = {"Haan",             "Informal yes"};
        dictionary["no"]          = {"Nahin",            "No"};
        dictionary["water"]       = {"Paani",            "Essential word"};
        dictionary["bread"]       = {"Roti",             "Staple food"};
        dictionary["house"]       = {"Ghar",             "Home"};
        dictionary["book"]        = {"Kitaab",           "Book"};
        dictionary["friend"]      = {"Yaar",             "Close friend"};
        dictionary["mother"]      = {"Maa",              "Mother"};
        dictionary["father"]      = {"Pita",             "Father"};
        dictionary["love"]        = {"Pyaar",            "Love"};
        dictionary["sun"]         = {"Sooraj",           "Sun"};
        dictionary["moon"]        = {"Chand",            "Moon"};
        dictionary["star"]        = {"Tara",             "Star"};
        dictionary["teacher"]     = {"Adhiyaapak",       "Teacher"};
        dictionary["student"]     = {"Vidiyaarthi",      "Student"};
        dictionary["good"]        = {"Changa",           "Good / well"};
        dictionary["bad"]         = {"Maada",            "Bad"};
        dictionary["beautiful"]   = {"Sundar",           "Beautiful"};
        dictionary["village"]     = {"Pind",             "Village"};
        dictionary["river"]       = {"Nadi",             "River"};
        dictionary["eat"]         = {"Khaana",           "To eat"};
        dictionary["go"]          = {"Jaana",            "To go"};
        dictionary["come"]        = {"Aona",             "To come"};
        dictionary["speak"]       = {"Bolna",            "To speak"};
        dictionary["write"]       = {"Likhna",           "To write"};
        dictionary["read"]        = {"Parhna",           "To read"};
        dictionary["big"]         = {"Vadda",            "Big"};
        dictionary["small"]       = {"Chhota",           "Small"};
        dictionary["brother"]     = {"Bhara",            "Brother"};
        dictionary["sister"]      = {"Bhain",            "Sister"};
        dictionary["market"]      = {"Bazaar",           "Market"};
        dictionary["school"]      = {"Skool",            "School"};
        dictionary["how are you"] = {"Tusi Kivein Ho?",  "How are you?"};
        dictionary["my name is"]  = {"Mera Naam ... Hai","My name is ..."};
    }

    void initVocabulary() {
        vocabulary["greetings"] = {
            VocabWord("Hello",     "Sat Sri Akal","Salam Dost","Adaab","Assalam o Alaikum",
                      "Staray Mashay","Adaab","Khush Aamdeed","Sat Sri Akal ji!"),
            VocabWord("Goodbye",   "Alwida","Khuda Hafiz","Allah Wahi","Allah Hafiz",
                      "Da Khoday Pa Aman","Khuda Hafiz","Phir Milam","Chalo Alwida!"),
            VocabWord("Thank you", "Dhanyavaad","Mehrbani","Meharbani","Shukriya",
                      "Manana","Shukria","Bohat Mehrbani","Bahut Dhanyavaad.")
        };
        vocabulary["numbers"] = {
            VocabWord("One",  "Ikk",   "Yak", "Hiku",   "Ek",   "Yaw",  "Akh",   "Ek",   "Ikk = 1"),
            VocabWord("Two",  "Do",    "Do",  "Bya",    "Do",   "Dwa",  "Zih",   "Do",   "Do = 2"),
            VocabWord("Three","Teen",  "Say", "Trey",   "Teen", "Dre",  "Trey",  "Teen", "Teen = 3"),
            VocabWord("Five", "Panj",  "Panj","Panj",   "Paanch","Pinja","Panch","Panj", "Panj = 5 rivers of Punjab!")
        };
        vocabulary["colors"] = {
            VocabWord("Red",   "Laal",   "Sur",   "Laal",   "Surkh",  "Sur",   "Lool",  "Qizil", "Laal rang"),
            VocabWord("Blue",  "Neela",  "Kabood","Neelo",  "Neela",  "Sheen", "Neel",  "Gok",   "Neela rang"),
            VocabWord("Green", "Hara",   "Sabz",  "Sabho",  "Hara",   "Shne",  "Poz",   "Yashil","Hara rang")
        };
        vocabulary["food"] = {
            VocabWord("Bread", "Roti",   "Naan",  "Maani",  "Roti",  "Dodai", "Tchot", "Roti",  "Roti khao"),
            VocabWord("Water", "Paani",  "Aab",   "Pani",   "Paani", "Obuh",  "Paan",  "Paani", "Paani piyo"),
            VocabWord("Rice",  "Chaawal","Birinj","Chawaru","Chawal","Wruje", "Hom",   "Chawal","Chaawal khao")
        };
        vocabulary["family"] = {
            VocabWord("Mother","Maa",    "Maan",  "Maau",   "Ammi",  "Mor",   "Muj",   "Amma",  "Maa pyaari hai"),
            VocabWord("Father","Pita",   "Baap",  "Pita",   "Abbu",  "Plaar", "Moul",  "Abu",   "Pita taakat hai"),
            VocabWord("Brother","Bhara", "Bira",  "Bhenya", "Bhai",  "Wror",  "Bav",   "Bhai",  "Bhara saath hai")
        };
    }

    void initQuizBank() {
        // ---- Beginner (Level 1) -- 5 questions ----
        { string o[4]={"Sat Sri Akal","Alwida","Dhanyavaad","Shubh Raat"};
          quizBank[1].push_back(QuizQuestion("How do you say Hello in Punjabi?",o,0,"Sat Sri Akal is the classic greeting.")); }
        { string o[4]={"Paani","Roti","Ghar","Kitaab"};
          quizBank[1].push_back(QuizQuestion("Punjabi word for water?",o,0,"Paani = water. Punjab has 5 great rivers!")); }
        { string o[4]={"Haan","Nahin","Changa","Theek"};
          quizBank[1].push_back(QuizQuestion("Which word means Yes?",o,0,"Haan = yes; Nahin = no.")); }
        { string o[4]={"Star","Moon","Sun","Cloud"};
          quizBank[1].push_back(QuizQuestion("What does Chand mean?",o,1,"Chand = Moon.")); }
        { string o[4]={"Three","Four","Five","Six"};
          quizBank[1].push_back(QuizQuestion("Panj in English is?",o,2,"Panj = Five. Punjab = Land of Five Rivers!")); }

        // ---- Intermediate (Level 2) -- 8 questions ----
        { string o[4]={"I am going home","I am eating","I am reading","I am speaking"};
          quizBank[2].push_back(QuizQuestion("Translate: Main ghar ja reha haan",o,0,"Main=I, Ghar=Home.")); }
        { string o[4]={"Adhiyaapak","Vidiyaarthi","Bazaar","Pita"};
          quizBank[2].push_back(QuizQuestion("Punjabi word for teacher?",o,0,"Adhiyaapak = teacher.")); }
        { string o[4]={"Listen","Speak","Write","Read"};
          quizBank[2].push_back(QuizQuestion("Likhna means?",o,2,"Likhna = to write.")); }
        { string o[4]={"Village","City","School","River"};
          quizBank[2].push_back(QuizQuestion("Pind means?",o,0,"Pind = Village.")); }
        { string o[4]={"Beautiful girl","Brave man","Kind woman","Tall tree"};
          quizBank[2].push_back(QuizQuestion("Sundar Kudi means?",o,0,"Sundar=Beautiful, Kudi=Girl.")); }
        { string o[4]={"I love you","How are you","Come here","Go away"};
          quizBank[2].push_back(QuizQuestion("Tusi Kivein Ho means?",o,1,"Tusi Kivein Ho = How are you?")); }
        { string o[4]={"Brother","Sister","Father","Mother"};
          quizBank[2].push_back(QuizQuestion("Bhain means?",o,1,"Bhain = Sister.")); }
        { string o[4]={"Market","School","River","Village"};
          quizBank[2].push_back(QuizQuestion("Bazaar means?",o,0,"Bazaar = Market.")); }

        // ---- Advanced (Level 3) -- 8 questions ----
        { string o[4]={"Past perfect","Past continuous","Future simple","Present simple"};
          quizBank[3].push_back(QuizQuestion("Tense: Uh khaana kha reha si",o,1,"Reha si = was doing: past continuous.")); }
        { string o[4]={"Subjunctive","Imperative","Indicative","Conditional"};
          quizBank[3].push_back(QuizQuestion("Tusi aao (You, come!) - which mood?",o,1,"Imperative mood = commands.")); }
        { string o[4]={"Waris Shah","Bulleh Shah","Guru Nanak","Farid"};
          quizBank[3].push_back(QuizQuestion("Who wrote the epic poem Heer Ranjha?",o,0,"Waris Shah wrote Heer in 1766 CE.")); }
        { string o[4]={"16th century","14th century","18th century","12th century"};
          quizBank[3].push_back(QuizQuestion("Gurmukhi script was standardized in the?",o,0,"Guru Angad Dev Ji standardized it in 16th century.")); }
        { string o[4]={"Greet a guest","Bless someone","Show gratitude","Begin a meal"};
          quizBank[3].push_back(QuizQuestion("Sat Sri Akal is used to?",o,0,"Literally: The Timeless Lord is Truth.")); }
        { string o[4]={"Kha lao","Pi lao","Ja sadda","Uth jao"};
          quizBank[3].push_back(QuizQuestion("Which phrase means 'Wake up!'?",o,3,"Uth jao = Wake up / get up.")); }
        { string o[4]={"Verb","Noun","Adjective","Adverb"};
          quizBank[3].push_back(QuizQuestion("Sundar (beautiful) is a?",o,2,"Sundar is an adjective describing a quality.")); }
        { string o[4]={"Verb-Subject-Object","Subject-Verb-Object","Subject-Object-Verb","Object-Subject-Verb"};
          quizBank[3].push_back(QuizQuestion("Punjabi sentence order (SOV means)?",o,2,"Punjabi follows Subject-Object-Verb order.")); }
    }

    void initPronunciation() {
        pronunciationMap["Sat Sri Akal"] =
            "SAT  : like 'shut'\nSRI  : roll the R lightly\nAKAL : uh-KAAL\nTip: say as one smooth phrase.";
        pronunciationMap["Paani"] =
            "PAA-nee\nHold the AA long, like 'father'.\nTip: Think 'pasta' ending in 'nee'.";
        pronunciationMap["Dhanyavaad"] =
            "DHUN-yuh-VAAD\nDh = aspirated D sound.\nTip: This sound doesn't exist in English.";
        pronunciationMap["Khaana"] =
            "KHAA-na\nKh = back of throat, like Scottish 'loch'.\nTip: Soft throat-clear + 'aana'.";
        pronunciationMap["Panj"] =
            "PUNJ\nRhymes with 'lunch' without L.\nTip: A sounds like U in 'fun'.";
        pronunciationMap["Bolna"] =
            "BOWL-na\nFirst syllable like 'bowl'.\nTip: Stress usually on FIRST syllable in Punjabi.";
    }

public:
    Punjabi() : Language("Punjabi") {
        initDictionary(); initVocabulary(); initQuizBank(); initPronunciation();
    }

    void translate(const string& word) override {
        if (!requireLogin()) return;
        string key = word; toLower(key);
        cout << "\n--- TRANSLATION ---\n";
        auto it = dictionary.find(key);
        if (it != dictionary.end()) {
            cout << "  English : " << word              << "\n";
            cout << "  Punjabi : " << it->second.first  << "\n";
            cout << "  Note    : " << it->second.second << "\n";
        } else {
            cout << "  Word not found.\n";
            cout << "  Try: hello, water, bread, house, friend, love, teacher...\n";
        }
    }

    void runQuiz(int level) override {
        if (!requireLogin()) return;
        string names[] = {"","Beginner","Intermediate","Advanced"};
        if (level < 1 || level > 3) { cout << "Invalid level.\n"; return; }
        if (currentUser->getLevel() < level) {
            cout << "Reach " << names[level] << " first. Current: "
                 << currentUser->getLevelName() << "\n"; return;
        }
        auto it = quizBank.find(level);
        if (it == quizBank.end() || it->second.empty()) { cout << "No questions.\n"; return; }
        runQuizSession(it->second, names[level] + " Punjabi Quiz");
    }

    void startGame(int choice) override {
        if (!requireLogin()) return;
        Game* game = nullptr;
        if      (choice == 1) game = new HangmanGame();
        else if (choice == 2) game = new WordMatchGame();
        else { cout << "Invalid game choice.\n"; return; }
        game->play();
        int earned = game->getScore();
        if (earned > 0 && currentUser) {
            currentUser->addScore(earned);
            cout << "+" << earned << " points added!\n";
            FileHandler::saveUser(*currentUser);
            FileHandler::updateLeaderboard(*currentUser);
        }
        delete game;
    }

    void showProgress() override {
        if (!requireLogin()) return;
        currentUser->displayProfile();
        auto weak = currentUser->getWeakAreas();
        if (!weak.empty()) {
            cout << "\nAreas needing revision:\n";
            for (const auto& w : weak) cout << "  - " << w << "\n";
        } else {
            cout << "\nNo weak areas detected. Excellent!\n";
        }
    }

    void showLeaderboard() override {
        auto entries = FileHandler::loadLeaderboard();
        cout << "\n"; line(50,'=');
        cout << "             GLOBAL LEADERBOARD\n"; line(50,'=');
        cout << left << setw(5)<<"Rank" << setw(18)<<"Username"
             << setw(10)<<"Score" << setw(14)<<"Level" << "Streak\n";
        line(50,'-');
        if (entries.empty()) cout << "  No entries yet. Be the first!\n";
        for (int i = 0; i < (int)entries.size() && i < 10; i++) {
            string rank;
            if      (i==0) rank="1st";
            else if (i==1) rank="2nd";
            else if (i==2) rank="3rd";
            else           rank=to_string(i+1)+"th";
            string lvl;
            if      (entries[i].level==1) lvl="Beginner";
            else if (entries[i].level==2) lvl="Intermediate";
            else                          lvl="Advanced";
            cout << left << setw(5)<<rank << setw(18)<<entries[i].username
                 << setw(10)<<entries[i].score << setw(14)<<lvl
                 << entries[i].streak << " days\n";
        }
        line(50,'=');
    }

    void loadModule(int level) override {
        if (!requireLogin()) return;
        map<int, vector<string>> cats;
        cats[1]={"greetings","numbers"}; cats[2]={"colors","food"}; cats[3]={"family"};
        if (!cats.count(level)) { cout << "Invalid level.\n"; return; }
        string names[]={"","Beginner","Intermediate","Advanced"};
        cout << "\n"; line(50,'=');
        cout << "  VOCABULARY MODULE -- " << names[level] << "\n"; line(50,'=');
        for (const string& cat : cats[level]) {
            cout << "\n  [" << cat << "]\n";
            cout << left << setw(14)<<"English" << setw(18)<<"Punjabi (Roman)" << "Example\n";
            line(70,'-');
            if (!vocabulary.count(cat)) continue;
            for (const auto& w : vocabulary.at(cat)) {
                cout << left << setw(14)<<w.english << setw(18)<<w.punjabi << w.example << "\n";
                currentUser->addMasteredWord(w.english);
            }
        }
        currentUser->awardBadge("First Steps");
        FileHandler::saveUser(*currentUser);
    }

    void dailyChallenge() override {
        if (!requireLogin()) return;
        cout << "\n=== DAILY CHALLENGE -- " << User::getCurrentDate() << " ===\n";
        srand((unsigned int)time(0));
        auto& pool = quizBank[1];
        if (pool.empty()) { cout << "No questions.\n"; return; }
        vector<QuizQuestion> daily;
        set<int> picked;
        for (int att=0; (int)daily.size()<3 && att<100; att++) {
            int r = rand()%(int)pool.size();
            if (!picked.count(r)) { daily.push_back(pool[r]); picked.insert(r); }
        }
        runQuizSession(daily, "Daily Challenge");
        currentUser->updateStreak();
        FileHandler::saveUser(*currentUser);
        cout << "Streak: " << currentUser->getStreak() << " days\n";
    }

    void pronunciationGuide(const string& word) override {
        if (!requireLogin()) return;
        cout << "\n--- PRONUNCIATION: " << word << " ---\n";
        auto it = pronunciationMap.find(word);
        if (it != pronunciationMap.end()) {
            cout << it->second << "\n";
            cout << "\n--- General Punjabi Rules ---\n";
            cout << "  1. Aspirated (kh, gh, dh): say with a breath out\n";
            cout << "  2. Long vowels (aa, ee, oo): hold twice as long\n";
            cout << "  3. Stress usually on FIRST syllable\n";
        } else {
            cout << "No guide for '" << word << "'.\n";
            cout << "Available: Sat Sri Akal, Paani, Dhanyavaad, Khaana, Panj, Bolna\n";
        }
    }

    void revisionQuiz() override {
        if (!requireLogin()) return;
        auto weak = currentUser->getWeakAreas();
        cout << "\n=== REVISION QUIZ ===\n";
        if (weak.empty()) {
            cout << "No weak areas! Running a general refresher.\n\n";
            runQuizSession(quizBank[1], "General Revision"); return;
        }
        cout << "Weak areas:\n";
        for (const auto& w : weak) cout << "  - " << w << "\n";
        vector<QuizQuestion> rev;
        for (auto& kv : quizBank)
            for (const auto& q : kv.second) rev.push_back(q);
        if ((int)rev.size() > 6) rev.resize(6);
        runQuizSession(rev, "Revision Quiz");
    }

    void showVocabulary(const string& cat) override {
        if (!requireLogin()) return;
        auto it = vocabulary.find(cat);
        if (it == vocabulary.end()) {
            cout << "Category not found.\nAvailable: greetings, numbers, colors, food, family\n";
            return;
        }
        cout << "\n--- VOCABULARY: " << cat << " ---\n";
        cout << left
             << setw(12)<<"English" << setw(15)<<"Punjabi" << setw(15)<<"Balochi"
             << setw(12)<<"Sindhi"  << setw(12)<<"Urdu"    << setw(12)<<"Pashto"
             << setw(12)<<"Kashmiri"<< setw(12)<<"Gilgiti" << "Example\n";
        line(120,'-');
        for (const auto& w : it->second)
            cout << left
                 << setw(12)<<w.english << setw(15)<<w.punjabi << setw(15)<<w.balochi
                 << setw(12)<<w.sindhi  << setw(12)<<w.urdu    << setw(12)<<w.pashto
                 << setw(12)<<w.kashmiri<< setw(12)<<w.gilgiti << w.example << "\n";
    }

    void showGameMenu() const {
        cout << "\n--- MINI GAMES ---\n";
        cout << "  1. Hangman    (Guess the Punjabi word)\n";
        cout << "  2. Word Match (Pick the correct Punjabi)\n";
        cout << "Choice: ";
    }

    void showLevelMenu() const {
        cout << "\n--- SELECT LEVEL ---\n";
        cout << "  1. Beginner\n  2. Intermediate\n  3. Advanced\n";
        cout << "Choice: ";
    }

    void showLanguageMenu() const {
        cout << "\n"; line(50,'=');
        cout << "   PUNJABI LANGUAGE LEARNING MENU\n";
        if (currentUser)
            cout << "   User: " << currentUser->getUsername()
                 << "  |  Level: " << currentUser->getLevelName()
                 << "  |  Score: " << currentUser->getTotalScore() << "\n";
        line(50,'=');
        cout << "   1.  Translate a Word\n";
        cout << "   2.  Vocabulary Module\n";
        cout << "   3.  Take a Quiz (Beginner)\n";
        cout << "   4.  Take a Quiz (Intermediate)\n";
        cout << "   5.  Take a Quiz (Advanced)\n";
        cout << "   6.  Mini Games\n";
        cout << "   7.  Daily Challenge\n";
        cout << "   8.  Revision Quiz (Weak Areas)\n";
        cout << "   9.  Pronunciation Guide\n";
        cout << "   0.  Back to Main Menu\n";
        line(50,'=');
        cout << "Choice: ";
    }
};

// ============================================================
//  URDU CULTURE CLASS  (all overrides INSIDE the class)
// ============================================================
class UrduCulture : public Culture {
public:
    UrduCulture() : Culture("Urdu", "Pakistan", 90) {
        addPhrase({"Assalam o Alaikum",   "Hello / Peace be upon you"});
        addPhrase({"Aap kaise hain?",     "How are you?"});
        addPhrase({"Shukriya",            "Thank you"});
        addPhrase({"Khuda Hafiz",         "Goodbye"});
        addPhrase({"Meharbani",           "Please / Kindness"});
        addPhrase({"Maaf kijiye",         "Sorry"});
        addPhrase({"Haan",                "Yes"});
        addPhrase({"Nahi",                "No"});
        addPhrase({"Aap ka naam kya hai?","What is your name?"});
        addPhrase({"Subah Bakhair",       "Good Morning"});
        addPhrase({"Sham Bakhair",        "Good Evening"});
        addPhrase({"Khush Amdeed",        "Welcome"});
        addPhrase({"Khana",               "Food"});
        addPhrase({"Pani",                "Water"});
        addPhrase({"Dost",                "Friend"});
        addPhrase({"Ustaad",              "Teacher"});
        addPhrase({"Doctor",              "Doctor"});
        addPhrase({"School",              "School"});
        addPhrase({"Ghar",                "House"});
        addPhrase({"Dil se shukriya",     "Heartfelt thanks"});

        addProverb({"Sabar ka phal meetha hota hai","Patience brings reward","Patience"});
        addProverb({"Jaisa karoge waisa bharoge","As you do, so you receive","Karma"});
        addProverb({"Ilm roshni hai","Knowledge is light","Education"});
        addProverb({"Mehnat kamyabi ki kunji hai","Hard work is key to success","Hard Work"});
        addProverb({"Sach hamesha jeetta hai","Truth always wins","Truth"});
        addProverb({"Dost woh jo mushkil mein kaam aaye","True friend helps in need","Friendship"});
        addProverb({"Waqt kisi ka intezar nahi karta","Time waits for none","Time"});
        addProverb({"Achai kabhi zaya nahi jati","Good deeds never wasted","Goodness"});
        addProverb({"Zaban ka zakham gehra hota hai","Words hurt deeply","Speech"});
        addProverb({"Mohabbat sab kuch jeet leti hai","Love conquers all","Love"});
        addProverb({"Ghar ki murgi daal barabar","Familiarity breeds contempt","Appreciation"});
        addProverb({"Neend aaye to sar na ho","Opportunities are often missed","Timing"});
        addProverb({"Aankhon ki sharm se andha nahi hote","Respect should not blind judgment","Wisdom"});
        addProverb({"Jab tak dant hai tab tak mashghalat hai","Problems never cease","Life"});
        addProverb({"Khoob teri baat suni aur meri nahi suni","Bias leads to injustice","Fairness"});
        addProverb({"Aqalmand ko ishara kafi hai","A hint is enough for the wise","Intelligence"});
        addProverb({"Har qadar momin nahi","Not every believer is devout","Faith"});
        addProverb({"Insaan galti ka putla hai","Humans are prone to error","Humility"});
        addProverb({"Woh din bhi aayenge","Better days will come","Hope"});
        addProverb({"Sachcha dost mushkil ka saathi","A true friend stays in hardship","Loyalty"});

        addTaboo({"Buzurgon ki be-izzati na karein","Respect elders","Show respect"});
        addTaboo({"Mehmaan ko ignore na karein","Hospitality is important","Welcome guests"});
        addTaboo({"Jhoot bolna bura hai","Lying is bad","Speak truth"});
        addTaboo({"Zyada gussa na karein","Anger is harmful","Stay calm"});
        addTaboo({"Masjid mein shor na karein","Respect religious places","Be silent"});
        addTaboo({"Ghar ki baatein bahar na le jayein","Privacy matters","Keep secrets"});
        addTaboo({"Bachon par zulm na karein","Protect children","Be kind"});
        addTaboo({"Khane ki na-shukri na karein","Respect food","Be grateful"});
        addTaboo({"Zyada takabbur na karein","Avoid arrogance","Be humble"});
        addTaboo({"Bad words use karna mana hai","Disrespectful language","Use polite words"});

        addTerm({"Doctor","Doctor","MEDICAL"});
        addTerm({"Hospital","Hospital","MEDICAL"});
        addTerm({"Fever","Bukhar","MEDICAL"});
        addTerm({"Court","Adalat","LEGAL"});
        addTerm({"Law","Qanoon","LEGAL"});
        addTerm({"Judge","Jaj","LEGAL"});
        addTerm({"School","School","EDUCATIONAL"});
        addTerm({"Teacher","Ustaad","EDUCATIONAL"});
        addTerm({"Student","Taalib-e-Ilm","EDUCATIONAL"});
        addTerm({"Book","Kitab","EDUCATIONAL"});

        addAspect({"Urdu Poetry",      "Rich poetic tradition of Ghazals and Nazms"});
        addAspect({"Mushaira",         "Poetry gathering event"});
        addAspect({"Pakistani Cuisine","Biryani, Nihari, Haleem and more"});
        addAspect({"Eid Celebrations", "Religious festivals with joy and food"});
        addAspect({"Wedding Culture",  "Dhol, Mehndi and Barat traditions"});
        addAspect({"Respect Culture",  "Strong emphasis on elders respect"});
        addAspect({"Hospitality",      "Guests are treated with honor"});
        addAspect({"Islamic Influence","Strong cultural and moral influence"});
        addAspect({"Shalwar Kameez",   "Traditional clothing widely worn"});
        addAspect({"Joint Family System","Strong family unit tradition"});

        addFood({"Biryani",   "Spiced rice with meat"});
        addFood({"Nihari",    "Slow cooked beef stew"});
        addFood({"Haleem",    "Wheat and meat porridge"});
        addFood({"Korma",     "Creamy meat curry"});
        addFood({"Karahi",    "Spicy tomato-based curry"});
        addFood({"Seekh Kabab","Grilled minced meat sticks"});
        addFood({"Samosa",    "Fried stuffed pastry"});
        addFood({"Pakora",    "Fried vegetable fritters"});
        addFood({"Halwa",     "Sweet semolina dessert"});
        addFood({"Chai",      "Tea with milk and spices"});
    }

    string translate(string e) override {
        if (e=="hello")       return "Assalam o Alaikum";
        if (e=="thank you")   return "Shukriya";
        if (e=="love")        return "Mohabbat";
        if (e=="book")        return "Kitaab";
        if (e=="teacher")     return "Ustaad";
        return "Translation not found";
    }
    CulturalTaboo getCulturalTaboo(string t) override {
        if (t=="elders")      return {"Respect elders","Avoid disrespect","Strong cultural value"};
        if (t=="hospitality") return {"Never refuse tea","Hospitality rule","Common etiquette"};
        if (t=="religion")    return {"Avoid irreverence","Faith respect","Deeply rooted taboo"};
        return {"Not found","N/A","N/A"};
    }
    Proverb getProverb(string t) override {
        if (t=="patience")    return {"Sabar ka phal meetha hota hai","Patience","Encourages endurance"};
        if (t=="knowledge")   return {"Ilm roshni hai","Knowledge","Knowledge is light"};
        if (t=="truth")       return {"Sachai hamesha jeetti hai","Truth","Truth always wins"};
        return {"Not found","N/A","N/A"};
    }
    string getGreeting(int f) override {
        switch(f) {
            case 0: return "Aray! Kya haal hai?";
            case 1: return "Assalam o Alaikum";
            case 2: return "Adaab";
            case 3: return "Khush Aamdeed";
            default: return "Assalam o Alaikum";
        }
    }
    string getDomainTerm(string e, string d) override {
        if (d=="literature") { if (e=="poetry") return "Shaairi"; if (e=="novel") return "Afsana"; }
        if (d=="religion")   { if (e=="prayer") return "Dua";     if (e=="mosque") return "Masjid"; }
        if (d=="education")  { if (e=="student") return "Talib-e-Ilm"; if (e=="teacher") return "Ustaad"; }
        return "Term not found";
    }
};

// ============================================================
//  URDU LANGUAGE CLASS
// ============================================================
class Urdu : public Language {
private:
    map<string, string>              dictionary;
    map<string, vector<VocabWord>>   vocabulary;
    map<int, vector<QuizQuestion>>   quizBank;
    map<string, string>              pronunciationMap;

    void toLower(string& s) const { transform(s.begin(),s.end(),s.begin(),::tolower); }

    void runQuizSession(const vector<QuizQuestion>& questions, const string& label) {
        int correct = 0, total = (int)questions.size();
        if (total == 0) { cout << "No questions.\n"; return; }
        cout << "\n===== " << label << " =====\n";
        for (int i = 0; i < total; i++) {
            const QuizQuestion& q = questions[i];
            cout << "\nQ" << i+1 << ": " << q.question << "\n";
            cout << "A. "<<q.options[0]<<"\nB. "<<q.options[1]<<"\nC. "<<q.options[2]<<"\nD. "<<q.options[3]<<"\n";
            cout << "Answer: ";
            char ans = toupper(readChar());
            int idx = ans - 'A';
            if (idx == q.correctIndex) { cout << "Correct!\n"; correct++; }
            else cout << "Wrong! Correct: " << (char)('A'+q.correctIndex) << "\n";
        }
        int pts = correct * 20;
        cout << "\nScore: " << correct << "/" << total << "  Points: +" << pts << "\n";
        if (currentUser) {
            currentUser->addScore(pts);
            FileHandler::saveUser(*currentUser);
            FileHandler::updateLeaderboard(*currentUser);
        }
    }

    void initDictionary() {
        dictionary["hello"]="Assalam o Alaikum"; dictionary["thank you"]="Shukriya";
        dictionary["yes"]="Haan";  dictionary["no"]="Nahi"; dictionary["water"]="Pani";
        dictionary["food"]="Khana"; dictionary["book"]="Kitab"; dictionary["friend"]="Dost";
        dictionary["teacher"]="Ustaad"; dictionary["school"]="School"; dictionary["love"]="Pyar";
        dictionary["house"]="Ghar"; dictionary["mother"]="Maa"; dictionary["father"]="Baap";
        dictionary["beautiful"]="Khubsurat"; dictionary["good"]="Acha"; dictionary["bad"]="Bura";
        dictionary["go"]="Jana"; dictionary["come"]="Aana"; dictionary["write"]="Likhna";
    }

    void initVocabulary() {
        vocabulary["greetings"] = {
            VocabWord("Hello",       "Assalam o Alaikum","","","","","","","Greeting in formal setting"),
            VocabWord("Good Morning","Subah Bakhair","","","","","","","Morning greeting"),
            VocabWord("Good Night",  "Shab Bakhair","","","","","","","Night farewell"),
            VocabWord("Welcome",     "Khush Aamdeed","","","","","","","Welcoming guests"),
            VocabWord("Goodbye",     "Allah Hafiz","","","","","","","Farewell blessing")
        };
        vocabulary["numbers"] = {
            VocabWord("One",  "Ek",   "","","","","","","Ek adad"),
            VocabWord("Two",  "Do",   "","","","","","","Do adad"),
            VocabWord("Three","Teen", "","","","","","","Teen adad"),
            VocabWord("Four", "Chaar","","","","","","","Chaar adad"),
            VocabWord("Five", "Paanch","","","","","","","Paanch adad")
        };
        vocabulary["colors"] = {
            VocabWord("Red",    "Surkh",  "","","","","","","Surkh rang"),
            VocabWord("Blue",   "Neela",  "","","","","","","Neela rang"),
            VocabWord("Green",  "Hara",   "","","","","","","Hara rang"),
            VocabWord("White",  "Safaid", "","","","","","","Safaid rang"),
            VocabWord("Black",  "Kaala",  "","","","","","","Kaala rang")
        };
        vocabulary["food"] = {
            VocabWord("Bread",  "Roti",  "","","","","","","Roti khao"),
            VocabWord("Water",  "Pani",  "","","","","","","Pani piyo"),
            VocabWord("Rice",   "Chawal","","","","","","","Chawal pakao"),
            VocabWord("Meat",   "Gosht", "","","","","","","Gosht khaana"),
            VocabWord("Tea",    "Chai",  "","","","","","","Chai peena")
        };
        vocabulary["family"] = {
            VocabWord("Mother", "Ammi", "","","","","","","Ammi pyaari hain"),
            VocabWord("Father", "Abbu", "","","","","","","Abbu taaqatwar hain"),
            VocabWord("Brother","Bhai", "","","","","","","Bhai saath hai"),
            VocabWord("Sister", "Behn", "","","","","","","Behn meherbaan hai"),
            VocabWord("Child",  "Bacha","","","","","","","Bacha masoom hota hai")
        };
    }

    void initQuizBank() {
        // ---- Beginner (5) ----
        { string o[4]={"Shukriya","Khuda Hafiz","Assalam","Kitab"};
          quizBank[1].push_back(QuizQuestion("How do you say Thank You in Urdu?",o,0,"Shukriya means thanks")); }
        { string o[4]={"Pani","Aag","Dhoop","Hawa"};
          quizBank[1].push_back(QuizQuestion("Urdu word for Water?",o,0,"Pani = water")); }
        { string o[4]={"Dost","Dushman","Ghar","Shehar"};
          quizBank[1].push_back(QuizQuestion("Friend in Urdu?",o,0,"Dost means friend")); }
        { string o[4]={"Subah Bakhair","Shab Bakhair","Khuda Hafiz","Adaab"};
          quizBank[1].push_back(QuizQuestion("Good Morning in Urdu?",o,0,"Subah Bakhair = Good Morning")); }
        { string o[4]={"Ammi","Abbu","Bhai","Behn"};
          quizBank[1].push_back(QuizQuestion("Mother in Urdu?",o,0,"Ammi = Mother")); }
        // ---- Intermediate (5) ----
        { string o[4]={"Khubsurat","Badsoorat","Berang","Sada"};
          quizBank[2].push_back(QuizQuestion("Beautiful in Urdu?",o,0,"Khubsurat = Beautiful")); }
        { string o[4]={"Ustaad","Shagird","Kitab","Qalam"};
          quizBank[2].push_back(QuizQuestion("Teacher in Urdu?",o,0,"Ustaad = Teacher")); }
        { string o[4]={"Main ghar ja raha hoon","Main khana kha raha hoon","Main parh raha hoon","Main so raha hoon"};
          quizBank[2].push_back(QuizQuestion("I am going home in Urdu?",o,0,"Main ghar ja raha hoon")); }
        { string o[4]={"Adalat","Masjid","School","Hospital"};
          quizBank[2].push_back(QuizQuestion("Court in Urdu?",o,0,"Adalat = Court")); }
        { string o[4]={"Mohabbat","Nafrat","Khushi","Gham"};
          quizBank[2].push_back(QuizQuestion("Love in Urdu?",o,0,"Mohabbat = Love")); }
        // ---- Advanced (5) ----
        { string o[4]={"Mirza Ghalib","Allama Iqbal","Faiz Ahmed Faiz","Ahmad Faraz"};
          quizBank[3].push_back(QuizQuestion("Who wrote 'Lab pe aati hai dua'?",o,1,"Allama Iqbal wrote this famous poem.")); }
        { string o[4]={"Ghazal","Nazm","Rubai","Qasida"};
          quizBank[3].push_back(QuizQuestion("Couplet-based love poem in Urdu is called?",o,0,"Ghazal is the couplet love poem form.")); }
        { string o[4]={"Persian","Arabic","Sanskrit","Turkish"};
          quizBank[3].push_back(QuizQuestion("Urdu script is based on which script?",o,0,"Nastaliq style derived from Persian script.")); }
        { string o[4]={"Progressive Writers","Mushaira Guild","Urdu Academy","Cultural Forum"};
          quizBank[3].push_back(QuizQuestion("Famous Urdu literary movement founded in 1936?",o,0,"Progressive Writers' Movement.")); }
        { string o[4]={"Subject-Object-Verb","Subject-Verb-Object","Verb-Subject-Object","Object-Verb-Subject"};
          quizBank[3].push_back(QuizQuestion("Urdu sentence word order (SOV) means?",o,0,"Like Punjabi, Urdu follows Subject-Object-Verb order.")); }
    }

    void initPronunciation() {
        pronunciationMap["Assalam o Alaikum"] = "A-ssa-lam o A-lai-kum\nSlow and respectful. Linger on 'lai'. Soft kum at end.";
        pronunciationMap["Shukriya"]    = "Shuk-ri-ya\nStress on FIRST syllable. Short i in middle.";
        pronunciationMap["Khubsurat"]   = "Khoob-soo-rat\nKhoob = very beautiful. 3 even syllables.";
        pronunciationMap["Mohabbat"]    = "Mo-hab-bat\nDouble b is held slightly. Stress on middle syllable.";
        pronunciationMap["Zindagi"]     = "Zin-da-gi\nRolling sound, equal stress on all 3 syllables.";
        pronunciationMap["Pani"]        = "Paa-nee\nLong aa. Think 'father' + nee.";
        pronunciationMap["Biryani"]     = "Bir-ya-ni\nStress on YA. Quick bir at start.";
        pronunciationMap["Khuda Hafiz"] = "Khu-da Ha-fiz\nKhu = throaty from back of mouth. God be your guardian.";
        pronunciationMap["Adalat"]      = "A-da-lat\nAll syllables equal. Final t is crisp.";
        pronunciationMap["Kitab"]       = "Ki-taab\nHold the long aa in middle. Short ki at start.";
    }

public:
    Urdu() : Language("Urdu") {
        initDictionary(); initVocabulary(); initQuizBank(); initPronunciation();
    }

    void translate(const string& word) override {
        if (!requireLogin()) return;
        string key = word; toLower(key);
        cout << "\n--- Urdu Translation ---\n";
        if (dictionary.count(key)) cout << word << " = " << dictionary[key] << "\n";
        else cout << "Word not found.\n";
    }
    void runQuiz(int level) override {
        if (!requireLogin()) return;
        string names[]={"","Beginner","Intermediate","Advanced"};
        if (level < 1 || level > 3){ cout << "Invalid level.\n"; return; }
        if (!quizBank.count(level) || quizBank[level].empty()) { cout << "No quiz for this level.\n"; return; }
        runQuizSession(quizBank[level], "Urdu " + names[level] + " Quiz");
    }
    void startGame(int choice) override {
        if (!requireLogin()) return;
        // Build Urdu-specific word banks
        vector<pair<string,string>> hangmanWords = {
            {"pani","water"},{"ghar","home"},{"kitab","book"},
            {"dost","friend"},{"chai","tea"},{"roti","bread"},
            {"ammi","mother"},{"bhai","brother"},{"ustaad","teacher"},{"sheher","city"}};
        vector<pair<string,string>> matchPairs = {
            {"hello","Assalam o Alaikum"},{"water","Pani"},{"bread","Roti"},
            {"friend","Dost"},{"love","Mohabbat"},{"teacher","Ustaad"},
            {"book","Kitab"},{"house","Ghar"},{"beautiful","Khubsurat"},{"thank you","Shukriya"}};
        if (choice == 1) {
            // Hangman with Urdu words
            srand((unsigned int)time(0));
            int idx = rand() % (int)hangmanWords.size();
            string target = hangmanWords[idx].first, hint = hangmanWords[idx].second;
            string display(target.size(), '_');
            set<char> tried; int wrong = 0, maxWrong = 6;
            cout << "\n--- HANGMAN (Urdu) ---\nHint: " << hint << "  | Length: " << target.size() << "\n\n";
            while (wrong < maxWrong && display != target) {
                cout << "Word  : "; for (char c:display) cout<<c<<" ";
                cout << "\nTried : "; for (char c:tried) cout<<c<<" ";
                cout << "\nGuess a letter: ";
                char g = tolower(readChar());
                if (tried.count(g)){ cout<<"Already tried!\n\n"; continue; }
                tried.insert(g);
                bool hit=false;
                for (int i=0;i<(int)target.size();i++) if (target[i]==g){display[i]=g;hit=true;}
                if (hit) cout<<"Good guess!\n\n";
                else { wrong++; cout<<"Wrong! ("<<wrong<<"/"<<maxWrong<<")\n\n"; }
            }
            if (display==target){ cout<<"CORRECT! Word: "<<target<<"\n";
                int earned=(maxWrong-wrong)*10;
                cout<<"Points: "<<earned<<"\n";
                if (currentUser){ currentUser->addScore(earned); FileHandler::saveUser(*currentUser); }
            } else { cout<<"Game over! Word was: "<<target<<"\n"; }
        } else if (choice == 2) {
            // Word Match with Urdu words
            srand((unsigned int)time(0));
            vector<pair<string,string>> sh = matchPairs;
            for (int i=(int)sh.size()-1;i>0;i--) swap(sh[i],sh[rand()%(i+1)]);
            int numQ=min(5,(int)sh.size()); int score=0;
            cout << "\n--- WORD MATCH (Urdu) ---\n";
            for (int i=0;i<numQ;i++){
                string correct=sh[i].second;
                vector<string> opts; opts.push_back(correct);
                vector<int> used; used.push_back(i);
                while ((int)opts.size()<4){
                    int r=rand()%(int)sh.size();
                    bool dup=false; for (int u:used) if(u==r){dup=true;break;}
                    if (!dup){opts.push_back(sh[r].second);used.push_back(r);}
                }
                for (int k=(int)opts.size()-1;k>0;k--) swap(opts[k],opts[rand()%(k+1)]);
                int ci=0; for (int k=0;k<4;k++) if (opts[k]==correct) ci=k;
                cout<<"Q"<<i+1<<" - English: \""<<sh[i].first<<"\"\n";
                for (int k=0;k<4;k++) cout<<"  "<<k+1<<". "<<opts[k]<<"\n";
                cout<<"Answer (1-4): "; int ch=readInt();
                if (ch-1==ci){cout<<"Correct!\n\n";score+=10;}
                else cout<<"Wrong! Answer: "<<correct<<"\n\n";
            }
            cout<<"Word Match done! Score: "<<score<<"/"<<numQ*10<<"\n";
            if (currentUser){ currentUser->addScore(score); FileHandler::saveUser(*currentUser); }
        } else { cout << "Invalid game choice.\n"; }
    }
    void showProgress() override {
        if (!requireLogin()) return;
        currentUser->displayProfile();
    }
    void showLeaderboard() override {
        auto entries = FileHandler::loadLeaderboard();
        cout << "\n"; line(50,'=');
        cout << "             GLOBAL LEADERBOARD\n"; line(50,'=');
        cout << left << setw(5)<<"Rank" << setw(18)<<"Username" << setw(10)<<"Score" << "Streak\n";
        line(50,'-');
        if (entries.empty()) cout << "  No entries yet.\n";
        for (int i=0; i<(int)entries.size()&&i<10; i++)
            cout << left << setw(5)<<i+1 << setw(18)<<entries[i].username
                 << setw(10)<<entries[i].score << entries[i].streak << " days\n";
        line(50,'=');
    }
    void loadModule(int level) override {
        if (!requireLogin()) return;
        map<int,vector<string>> cats;
        cats[1]={"greetings","numbers"}; cats[2]={"colors","food"}; cats[3]={"family"};
        if (!cats.count(level)){ cout << "Invalid level.\n"; return; }
        string names[]={"","Beginner","Intermediate","Advanced"};
        cout << "\n"; line(50,'=');
        cout << "  URDU VOCABULARY MODULE -- " << names[level] << "\n"; line(50,'=');
        for (const string& cat : cats[level]) {
            cout << "\n  [" << cat << "]\n";
            cout << left << setw(14)<<"English" << setw(20)<<"Urdu (Roman)" << "Example\n";
            line(60,'-');
            if (!vocabulary.count(cat)) continue;
            for (const auto& w : vocabulary.at(cat)) {
                cout << left << setw(14)<<w.english << setw(20)<<w.punjabi << w.example << "\n";
                currentUser->addMasteredWord(w.english);
            }
        }
        currentUser->awardBadge("First Steps");
        FileHandler::saveUser(*currentUser);
    }
    void dailyChallenge() override {
        if (!requireLogin()) return;
        cout << "\n=== DAILY URDU CHALLENGE -- " << User::getCurrentDate() << " ===\n";
        srand((unsigned int)time(0));
        auto& pool = quizBank[1];
        if (pool.empty()){ cout << "No questions.\n"; return; }
        vector<QuizQuestion> daily;
        set<int> picked;
        for (int att=0; (int)daily.size()<3 && att<100; att++){
            int r=rand()%(int)pool.size();
            if (!picked.count(r)){daily.push_back(pool[r]);picked.insert(r);}
        }
        runQuizSession(daily,"Daily Urdu Challenge");
        currentUser->updateStreak();
        FileHandler::saveUser(*currentUser);
        cout << "Streak: " << currentUser->getStreak() << " days\n";
    }
    void pronunciationGuide(const string& word) override {
        if (!requireLogin()) return;
        cout << "\n--- URDU PRONUNCIATION: " << word << " ---\n";
        auto it = pronunciationMap.find(word);
        if (it != pronunciationMap.end()) {
            cout << it->second << "\n";
            cout << "\n--- General Urdu Rules ---\n";
            cout << "  1. Aspirated (kh, gh): from back of throat\n";
            cout << "  2. Long vowels (aa, ee): held twice as long\n";
            cout << "  3. Urdu is written right-to-left in Nastaliq script\n";
        } else {
            cout << "No guide for '" << word << "'.\n";
            cout << "Available: Assalam o Alaikum, Shukriya, Khubsurat, Mohabbat, Zindagi, Pani, Biryani, Khuda Hafiz, Adalat, Kitab\n";
        }
    }
    void revisionQuiz() override {
        if (!requireLogin()) return;
        auto weak = currentUser->getWeakAreas();
        cout << "\n=== URDU REVISION QUIZ ===\n";
        if (weak.empty()){ cout << "No weak areas! Running general refresher.\n\n"; runQuizSession(quizBank[1],"General Urdu Revision"); return; }
        vector<QuizQuestion> rev;
        for (auto& kv:quizBank) for (const auto& q:kv.second) rev.push_back(q);
        if ((int)rev.size()>6) rev.resize(6);
        runQuizSession(rev,"Urdu Revision Quiz");
    }
    void showVocabulary(const string& cat) override {
        if (!requireLogin()) return;
        if (!vocabulary.count(cat)){
            cout << "Category not found.\nAvailable: greetings, numbers, colors, food, family\n"; return;
        }
        cout << "\n--- URDU VOCABULARY: " << cat << " ---\n";
        cout << left << setw(14)<<"English" << setw(20)<<"Urdu (Roman)" << "Example\n";
        line(60,'-');
        for (const auto& w : vocabulary.at(cat))
            cout << left << setw(14)<<w.english << setw(20)<<w.punjabi << w.example << "\n";
    }
};

// ============================================================
//  SINDHI CULTURE  (overrides INSIDE the class)
// ============================================================
class SindhiCulture : public Culture {
public:
    SindhiCulture() : Culture("Sindhi", "Sindh", 90) {
        addPhrase({"Assalam o Alaikum",     "Hello / Peace be upon you"});
        addPhrase({"Tuhanje haal cha aahin?","How are you?"});
        addPhrase({"Mehrbani",              "Thank you / Please"});
        addPhrase({"Khuda Hafiz",           "Goodbye"});
        addPhrase({"Maaf Kajo",             "Sorry"});
        addPhrase({"Haan",                  "Yes"});
        addPhrase({"Naa",                   "No"});
        addPhrase({"Tuharo naalo chha aahe?","What is your name?"});
        addPhrase({"Subh Bakhair",          "Good Morning"});
        addPhrase({"Sham Bakhair",          "Good Evening"});
        addPhrase({"Khana",                 "Food"});
        addPhrase({"Pani",                  "Water"});
        addPhrase({"Dost",                  "Friend"});
        addPhrase({"Ustad",                 "Teacher"});
        addPhrase({"Ghar",                  "House"});
        addPhrase({"Kitab",                 "Book"});
        addPhrase({"Hospital",              "Hospital"});
        addPhrase({"School",                "School"});
        addPhrase({"Dil khush aahi",        "I am happy"});
        addPhrase({"Tuhanje shukriya",      "Thank you very much"});

        addProverb({"Jeko kare so bhare","As you sow, so shall you reap","Karma"});
        addProverb({"Sabar jo phal mitho aahe","Patience brings sweet reward","Patience"});
        addProverb({"Ilm roshni aahe","Knowledge is light","Education"});
        addProverb({"Mehnat sa kamyabi milande aahe","Hard work leads to success","Hard Work"});
        addProverb({"Sach hamesha jeetando aahe","Truth always wins","Truth"});
        addProverb({"Dost mushkil mein pehchanje wenda aahe","Friend known in trouble","Friendship"});
        addProverb({"Waqt kise ji intezar na kare","Time waits for none","Time"});
        addProverb({"Achai kabhi zaya na thiye","Good deeds never go waste","Goodness"});
        addProverb({"Zaban jo zakhm gehro aahe","Words hurt deeply","Speech"});
        addProverb({"Mohabbat sab kuch jeetendi aahe","Love conquers all","Love"});
        addProverb({"Sach jo suraj kabhi na dhaley","Truth never fades","Honesty"});
        addProverb({"Ajrak joon rangan mein ekta","Colors of Ajrak symbolize harmony","Unity"});
        addProverb({"Pani waro pani jo phand aahe","Every river has its own path","Destiny"});
        addProverb({"Ghar ji izzat ghar waalan san aahe","Family honor depends on its members","Family"});
        addProverb({"Khalo na dhalo taan kaho","Think before you speak","Wisdom"});
        addProverb({"Mitti jo ghar hi ghar aahio","The home of clay is the real home","Simplicity"});
        addProverb({"Haar maan wali jo taqat aahe","There is strength in accepting defeat","Humility"});
        addProverb({"Masoom dil jo khuda dost aahio","God loves the innocent heart","Purity"});
        addProverb({"Sach bolyan saan dil khush rehande aahe","Truthfulness brings peace","Truth"});
        addProverb({"Ilm waro ilm bant saghando aahe","Knowledge multiplies when shared","Sharing"});

        addTaboo({"Buzurgan ji be-izzati na karo","Respect elders","Show respect"});
        addTaboo({"Jhoot na bolo","Do not lie","Truthfulness"});
        addTaboo({"Mehmaan ignore na karo","Respect guests","Hospitality"});
        addTaboo({"Zyada gussa na karo","Avoid anger","Calmness"});
        addTaboo({"Masjid mein shor na karo","No noise in mosque","Respect"});
        addTaboo({"Ghar ji baat bahar na kadh","Family matters are private","Privacy"});
        addTaboo({"Khane ji na-shukri na karo","Don't waste food","Gratitude"});
        addTaboo({"Takabbur na karo","Avoid arrogance","Humility"});
        addTaboo({"Zulm na karo","Do not oppress","Justice"});
        addTaboo({"Bachan san narmi karo","Be kind to children","Kindness"});

        addTerm({"Doctor","Doctor","MEDICAL"}); addTerm({"Hospital","Hospital","MEDICAL"});
        addTerm({"Fever","Bukhar","MEDICAL"}); addTerm({"Court","Adalat","LEGAL"});
        addTerm({"Law","Qanoon","LEGAL"}); addTerm({"Judge","Jaj","LEGAL"});
        addTerm({"School","School","EDUCATIONAL"}); addTerm({"Teacher","Ustad","EDUCATIONAL"});
        addTerm({"Student","Taalib Ilm","EDUCATIONAL"}); addTerm({"Book","Kitab","EDUCATIONAL"});

        addAspect({"Ajrak",             "Traditional Sindhi block-printed shawl and symbol of culture"});
        addAspect({"Sindhi Topi",       "Traditional cap representing identity"});
        addAspect({"Sufi Culture",      "Deep spiritual and poetic tradition"});
        addAspect({"Lal Shahbaz Qalandar","Famous Sufi saint of Sehwan Sharif"});
        addAspect({"Sindhi Cuisine",    "Sai bhaji, Sindhi biryani, koki"});
        addAspect({"Sindhi Weddings",   "Colorful and traditional ceremonies"});
        addAspect({"Ralli Craft",       "Traditional handmade patchwork textiles"});
        addAspect({"River Indus",       "Civilization along the Indus River"});
        addAspect({"Mela Festivals",    "Cultural fairs and gatherings"});
        addAspect({"Folk Music",        "Traditional Sindhi music and instruments"});

        addFood({"Sindhi Biryani","Spicy rice dish with meat and potatoes"});
        addFood({"Sai Bhaji","Spinach and lentil curry"});
        addFood({"Koki","Thick flatbread with spices"});
        addFood({"Dal Pakwan","Crispy bread with lentils"});
        addFood({"Seviyan","Sweet vermicelli dessert"});
        addFood({"Besan Tikki","Chickpea flour snack"});
        addFood({"Bhugal Bhee","Lotus stem curry"});
        addFood({"Sindhi Kadhi","Tangy gram flour curry"});
        addFood({"Lassi","Yogurt drink"});
        addFood({"Halwa","Sweet dessert"});
    }

    string translate(string e) override {
        if (e=="hello")     return "Salaam";
        if (e=="thank you") return "Mehrbani";
        if (e=="water")     return "Paani";
        if (e=="bread")     return "Roti";
        if (e=="friend")    return "Dost";
        return "Translation not found";
    }
    CulturalTaboo getCulturalTaboo(string t) override {
        if (t=="ajrak")       return {"Never disrespect Ajrak","Cultural symbol","Deep heritage"};
        if (t=="hospitality") return {"Guests are sacred","Hospitality","Strong Sindhi tradition"};
        if (t=="elders")      return {"Respect elders","Avoid disrespect","Core cultural value"};
        return {"Not found","N/A","N/A"};
    }
    Proverb getProverb(string t) override {
        if (t=="truth") return {"Sach jo suraj kabhi na dhaley","Truth","Truth never fades"};
        if (t=="unity") return {"Ajrak joon rangan mein ekta","Unity","Colors symbolize harmony"};
        return {"Not found","N/A","N/A"};
    }
    string getGreeting(int f) override {
        switch(f) {
            case 0: return "Hala! Kheir aahe?";
            case 1: return "Salaam";
            case 2: return "Mehrbani";
            case 3: return "Khush Aamdeed";
            default: return "Salaam";
        }
    }
    string getDomainTerm(string e, string d) override {
        if (d=="crafts") { if (e=="shawl") return "Ajrak"; if (e=="cap") return "Sindhi Topi"; }
        if (d=="music")  { if (e=="song") return "Geet"; if (e=="instrument") return "Yaktaro"; }
        return "Term not found";
    }
};

// ============================================================
//  SINDHI LANGUAGE CLASS
// ============================================================
class Sindhi : public Language {
private:
    map<string,string>             dictionary;
    map<string,vector<VocabWord>>  vocabulary;
    map<int,vector<QuizQuestion>>  quizBank;
    map<string,string>             pronunciationMap;

    void toLower(string& s) const { transform(s.begin(),s.end(),s.begin(),::tolower); }

    void runQuizSession(const vector<QuizQuestion>& questions, const string& label) {
        int correct = 0, total = (int)questions.size();
        if (total == 0) { cout << "No questions.\n"; return; }
        cout << "\n===== " << label << " =====\n";
        for (int i=0; i<total; i++) {
            const QuizQuestion& q = questions[i];
            cout << "\nQ"<<i+1<<": "<<q.question<<"\nA. "<<q.options[0]
                 <<"\nB. "<<q.options[1]<<"\nC. "<<q.options[2]<<"\nD. "<<q.options[3]<<"\nAnswer: ";
            char ans = toupper(readChar());
            int idx = ans - 'A';
            if (idx == q.correctIndex) { cout << "Correct!\n"; correct++; }
            else cout << "Wrong! Correct: " << (char)('A'+q.correctIndex) << "\n";
        }
        int pts = correct * 20;
        cout << "\nScore: " << correct << "/" << total << "  Points: +" << pts << "\n";
        if (currentUser) { currentUser->addScore(pts); FileHandler::saveUser(*currentUser); }
    }

    void initDictionary() {
        dictionary["hello"]="Assalam o Alaikum"; dictionary["thank you"]="Mehrbani";
        dictionary["yes"]="Haan"; dictionary["no"]="Na"; dictionary["water"]="Pani";
        dictionary["food"]="Khana"; dictionary["friend"]="Dost"; dictionary["teacher"]="Ustad";
        dictionary["book"]="Kitab"; dictionary["house"]="Ghar"; dictionary["love"]="Muhabbat";
        dictionary["good"]="Changa"; dictionary["bad"]="Bura"; dictionary["go"]="Wanja";
        dictionary["come"]="Ach"; dictionary["write"]="Likh"; dictionary["read"]="Parh";
        dictionary["big"]="Vado"; dictionary["small"]="Chhoto"; dictionary["school"]="School";
    }
    void initVocabulary() {
        vocabulary["greetings"] = {
            VocabWord("Hello",       "Assalam o Alaikum","","","","","","","Greeting"),
            VocabWord("Good Morning","Subh Bakhair","","","","","","","Morning greeting"),
            VocabWord("Good Evening","Sham Bakhair","","","","","","","Evening greeting"),
            VocabWord("Welcome",     "Aaio Ji","","","","","","","Welcoming guests"),
            VocabWord("Goodbye",     "Khuda Hafiz","","","","","","","Farewell")
        };
        vocabulary["numbers"] = {
            VocabWord("One",  "Hiku",  "","","","","","","Hiku adad"),
            VocabWord("Two",  "Bya",   "","","","","","","Bya adad"),
            VocabWord("Three","Trey",  "","","","","","","Trey adad"),
            VocabWord("Four", "Chaar", "","","","","","","Chaar adad"),
            VocabWord("Five", "Panj",  "","","","","","","Panj adad")
        };
        vocabulary["colors"] = {
            VocabWord("Red",   "Laal",  "","","","","","","Laal rang"),
            VocabWord("Blue",  "Neelo", "","","","","","","Neelo rang"),
            VocabWord("Green", "Sabho", "","","","","","","Sabho rang"),
            VocabWord("White", "Chito", "","","","","","","Chito rang"),
            VocabWord("Black", "Kaaro", "","","","","","","Kaaro rang")
        };
        vocabulary["food"] = {
            VocabWord("Bread", "Maani","","","","","","","Maani khao"),
            VocabWord("Water", "Pani", "","","","","","","Pani piyo"),
            VocabWord("Rice",  "Bhaat","","","","","","","Bhaat pakao"),
            VocabWord("Tea",   "Chai", "","","","","","","Chai piyo"),
            VocabWord("Fish",  "Machhri","","","","","","","Sindhi machhri")
        };
        vocabulary["family"] = {
            VocabWord("Mother","Ami",  "","","","","","","Ami pyaari aahe"),
            VocabWord("Father","Baba", "","","","","","","Baba taaqatwar"),
            VocabWord("Brother","Bhenu","","","","","","","Bhenu saath"),
            VocabWord("Sister","Bhaiṇ","","","","","","","Bhaiṇ meherbaan"),
            VocabWord("Child", "ٻار / Baar","","","","","","","Baar masoom")
        };
    }
    void initQuizBank() {
        // ---- Beginner (5) ----
        { string o[4]={"Mehrbani","Shukriya","Salam","Kitaab"};
          quizBank[1].push_back(QuizQuestion("Thank you in Sindhi?",o,0,"Mehrbani means thank you")); }
        { string o[4]={"Pani","Aag","Hawa","Dhoop"};
          quizBank[1].push_back(QuizQuestion("Water in Sindhi?",o,0,"Pani = water")); }
        { string o[4]={"Dost","Dushman","Ghar","Kitab"};
          quizBank[1].push_back(QuizQuestion("Friend in Sindhi?",o,0,"Dost = friend")); }
        { string o[4]={"Subh Bakhair","Shab Bakhair","Khuda Hafiz","Adaab"};
          quizBank[1].push_back(QuizQuestion("Good Morning in Sindhi?",o,0,"Subh Bakhair = Good Morning")); }
        { string o[4]={"Ami","Baba","Bhenu","Baar"};
          quizBank[1].push_back(QuizQuestion("Mother in Sindhi?",o,0,"Ami = Mother in Sindhi")); }
        // ---- Intermediate (5) ----
        { string o[4]={"Ajrak","Topi","Ralli","Dupatta"};
          quizBank[2].push_back(QuizQuestion("Symbol of Sindhi culture?",o,0,"Ajrak is the iconic Sindhi shawl.")); }
        { string o[4]={"Lal Shahbaz","Bulleh Shah","Waris Shah","Rumi"};
          quizBank[2].push_back(QuizQuestion("Famous Sufi saint of Sehwan Sharif?",o,0,"Lal Shahbaz Qalandar.")); }
        { string o[4]={"Sai Bhaji","Koki","Biryani","Haleem"};
          quizBank[2].push_back(QuizQuestion("Traditional Sindhi spinach and lentil curry?",o,0,"Sai Bhaji = spinach curry.")); }
        { string o[4]={"Indus","Ravi","Chenab","Jhelum"};
          quizBank[2].push_back(QuizQuestion("Major river flowing through Sindh?",o,0,"The Indus River = Darya-e-Sindh.")); }
        { string o[4]={"Ralli","Ajrak","Phulkari","Pashmina"};
          quizBank[2].push_back(QuizQuestion("Traditional Sindhi handmade patchwork textile?",o,0,"Ralli craft is traditional patchwork.")); }
        // ---- Advanced (5) ----
        { string o[4]={"Sindhi Biryani","Koki","Sai Bhaji","Dal Pakwan"};
          quizBank[3].push_back(QuizQuestion("Which Sindhi dish is thick flatbread with spices?",o,1,"Koki = thick spiced flatbread.")); }
        { string o[4]={"Mohenjo-daro","Taxila","Harappa","Mehrgarh"};
          quizBank[3].push_back(QuizQuestion("Famous ancient Indus Valley Civilization site in Sindh?",o,0,"Mohenjo-daro is in Sindh.")); }
        { string o[4]={"Shah Abdul Latif","Bulleh Shah","Waris Shah","Sachal Sarmast"};
          quizBank[3].push_back(QuizQuestion("Author of Shah Jo Risalo, famous Sindhi poetry?",o,0,"Shah Abdul Latif Bhittai.")); }
        { string o[4]={"Sindhi","Balochi","Pashto","Punjabi"};
          quizBank[3].push_back(QuizQuestion("Which language uses the Perso-Arabic script in Sindh?",o,0,"Sindhi uses extended Perso-Arabic.")); }
        { string o[4]={"Hyderabad","Karachi","Sukkur","Larkana"};
          quizBank[3].push_back(QuizQuestion("Ancient capital of Talpur Mirs in Sindh?",o,0,"Hyderabad was Talpur capital.")); }
    }
    void initPronunciation() {
        pronunciationMap["Assalam o Alaikum"]= "A-ssa-lam o a-lai-kum (soft and respectful)";
        pronunciationMap["Mehrbani"]= "Mehr-baa-ni (stress on middle syllable)";
        pronunciationMap["Ajrak"]   = "Aj-rak (hard j, short crisp a at start)";
        pronunciationMap["Pani"]    = "Paa-ni (long aa, like 'father')";
        pronunciationMap["Subh Bakhair"]= "Subh Bakh-air (Subh=morning, hold the ai in Bakhair)";
        pronunciationMap["Dost"]    = "Dost (rhymes with 'most', soft d)";
        pronunciationMap["Koki"]    = "Ko-ki (two equal short syllables)";
        pronunciationMap["Ghar"]    = "Ghur (gh from back of throat + ur)";
        pronunciationMap["Kitab"]   = "Ki-taab (long aa in middle, crisp final b)";
        pronunciationMap["Maani"]   = "Maa-ni (long aa, like 'father' + nee)";
    }

public:
    Sindhi() : Language("Sindhi") {
        initDictionary(); initVocabulary(); initQuizBank(); initPronunciation();
    }
    void translate(const string& word) override {
        if (!requireLogin()) return;
        string key=word; toLower(key);
        cout << "\n--- Sindhi Translation ---\n";
        if (dictionary.count(key)) cout << word << " = " << dictionary[key] << "\n";
        else cout << "Word not found.\nTry: hello, water, bread, friend, teacher, love...\n";
    }
    void runQuiz(int level) override {
        if (!requireLogin()) return;
        string names[]={"","Beginner","Intermediate","Advanced"};
        if (level<1||level>3){cout<<"Invalid level.\n";return;}
        if (!quizBank.count(level)||quizBank[level].empty()){cout<<"No quiz available.\n";return;}
        runQuizSession(quizBank[level],"Sindhi "+names[level]+" Quiz");
    }
    void startGame(int choice) override {
        if (!requireLogin()) return;
        vector<pair<string,string>> hangmanWords={
            {"pani","water"},{"ghar","home"},{"kitab","book"},
            {"dost","friend"},{"maani","bread"},{"ajrak","Sindhi shawl"},
            {"roti","flatbread"},{"bhaat","rice"},{"ustad","teacher"},{"chai","tea"}};
        vector<pair<string,string>> matchPairs={
            {"hello","Assalam o Alaikum"},{"water","Pani"},{"bread","Maani"},
            {"friend","Dost"},{"teacher","Ustad"},{"book","Kitab"},
            {"thank you","Mehrbani"},{"house","Ghar"},{"morning","Subh Bakhair"},{"mother","Ami"}};
        if (choice==1){
            srand((unsigned int)time(0));
            int idx=rand()%(int)hangmanWords.size();
            string target=hangmanWords[idx].first,hint=hangmanWords[idx].second;
            string display(target.size(),'_'); set<char> tried; int wrong=0,maxWrong=6;
            cout<<"\n--- HANGMAN (Sindhi) ---\nHint: "<<hint<<"  | Length: "<<target.size()<<"\n\n";
            while (wrong<maxWrong&&display!=target){
                cout<<"Word  : "; for(char c:display)cout<<c<<" ";
                cout<<"\nTried : "; for(char c:tried)cout<<c<<" ";
                cout<<"\nGuess a letter: "; char g=tolower(readChar());
                if (tried.count(g)){cout<<"Already tried!\n\n";continue;}
                tried.insert(g); bool hit=false;
                for(int i=0;i<(int)target.size();i++) if(target[i]==g){display[i]=g;hit=true;}
                if(hit)cout<<"Good guess!\n\n";
                else{wrong++;cout<<"Wrong! ("<<wrong<<"/"<<maxWrong<<")\n\n";}
            }
            if(display==target){cout<<"CORRECT! Word: "<<target<<"\n";
                int earned=(maxWrong-wrong)*10;cout<<"Points: "<<earned<<"\n";
                if(currentUser){currentUser->addScore(earned);FileHandler::saveUser(*currentUser);}}
            else cout<<"Game over! Word was: "<<target<<"\n";
        } else if (choice==2){
            srand((unsigned int)time(0));
            vector<pair<string,string>> sh=matchPairs;
            for(int i=(int)sh.size()-1;i>0;i--) swap(sh[i],sh[rand()%(i+1)]);
            int numQ=min(5,(int)sh.size()),score=0;
            cout<<"\n--- WORD MATCH (Sindhi) ---\n";
            for(int i=0;i<numQ;i++){
                string correct=sh[i].second; vector<string> opts; opts.push_back(correct);
                vector<int> used; used.push_back(i);
                while((int)opts.size()<4){int r=rand()%(int)sh.size();bool dup=false;
                    for(int u:used) if(u==r){dup=true;break;}
                    if(!dup){opts.push_back(sh[r].second);used.push_back(r);}}
                for(int k=(int)opts.size()-1;k>0;k--) swap(opts[k],opts[rand()%(k+1)]);
                int ci=0; for(int k=0;k<4;k++) if(opts[k]==correct)ci=k;
                cout<<"Q"<<i+1<<" - \""<<sh[i].first<<"\"\n";
                for(int k=0;k<4;k++) cout<<"  "<<k+1<<". "<<opts[k]<<"\n";
                cout<<"Answer (1-4): "; int ch=readInt();
                if(ch-1==ci){cout<<"Correct!\n\n";score+=10;}
                else cout<<"Wrong! Answer: "<<correct<<"\n\n";
            }
            cout<<"Score: "<<score<<"/"<<numQ*10<<"\n";
            if(currentUser){currentUser->addScore(score);FileHandler::saveUser(*currentUser);}
        } else cout<<"Invalid game choice.\n";
    }
    void showProgress() override { if (!requireLogin()) return; currentUser->displayProfile(); }
    void showLeaderboard() override {
        auto e=FileHandler::loadLeaderboard();
        cout<<"\n"; line(50,'='); cout<<"             GLOBAL LEADERBOARD\n"; line(50,'=');
        cout<<left<<setw(5)<<"Rank"<<setw(18)<<"Username"<<setw(10)<<"Score"<<"Streak\n"; line(50,'-');
        if(e.empty())cout<<"  No entries yet.\n";
        for(int i=0;i<(int)e.size()&&i<10;i++)
            cout<<left<<setw(5)<<i+1<<setw(18)<<e[i].username<<setw(10)<<e[i].score<<e[i].streak<<" days\n";
        line(50,'=');
    }
    void loadModule(int level) override {
        if (!requireLogin()) return;
        map<int,vector<string>> cats;
        cats[1]={"greetings","numbers"}; cats[2]={"colors","food"}; cats[3]={"family"};
        if (!cats.count(level)){cout<<"Invalid level.\n";return;}
        string names[]={"","Beginner","Intermediate","Advanced"};
        cout<<"\n"; line(50,'='); cout<<"  SINDHI VOCABULARY -- "<<names[level]<<"\n"; line(50,'=');
        for (const string& cat:cats[level]){
            cout<<"\n  ["<<cat<<"]\n";
            cout<<left<<setw(14)<<"English"<<setw(20)<<"Sindhi (Roman)"<<"Example\n"; line(60,'-');
            if (!vocabulary.count(cat)) continue;
            for (const auto& w:vocabulary.at(cat)){
                cout<<left<<setw(14)<<w.english<<setw(20)<<w.punjabi<<w.example<<"\n";
                currentUser->addMasteredWord(w.english);
            }
        }
        currentUser->awardBadge("First Steps"); FileHandler::saveUser(*currentUser);
    }
    void dailyChallenge() override {
        if (!requireLogin()) return;
        cout<<"\n=== DAILY SINDHI CHALLENGE -- "<<User::getCurrentDate()<<" ===\n";
        srand((unsigned int)time(0)); auto& pool=quizBank[1];
        if(pool.empty()){cout<<"No questions.\n";return;}
        vector<QuizQuestion> daily; set<int> picked;
        for(int att=0;(int)daily.size()<3&&att<100;att++){int r=rand()%(int)pool.size();
            if(!picked.count(r)){daily.push_back(pool[r]);picked.insert(r);}}
        runQuizSession(daily,"Daily Sindhi Challenge");
        currentUser->updateStreak(); FileHandler::saveUser(*currentUser);
        cout<<"Streak: "<<currentUser->getStreak()<<" days\n";
    }
    void pronunciationGuide(const string& word) override {
        if (!requireLogin()) return;
        cout<<"\n--- SINDHI PRONUNCIATION: "<<word<<" ---\n";
        if (pronunciationMap.count(word)){
            cout<<pronunciationMap.at(word)<<"\n";
            cout<<"\n--- General Sindhi Rules ---\n";
            cout<<"  1. Sindhi has unique retroflex sounds (like ṭ, ḍ)\n";
            cout<<"  2. Breathy consonants are common\n";
            cout<<"  3. Long vowels aa, ee, oo are held twice as long\n";
        } else {
            cout<<"No guide for '"<<word<<"'.\n";
            cout<<"Available: Assalam o Alaikum, Mehrbani, Ajrak, Pani, Subh Bakhair, Dost, Koki, Ghar, Kitab, Maani\n";
        }
    }
    void revisionQuiz() override {
        if (!requireLogin()) return;
        auto weak=currentUser->getWeakAreas();
        cout<<"\n=== SINDHI REVISION QUIZ ===\n";
        if(weak.empty()){cout<<"No weak areas! General refresher.\n\n";runQuizSession(quizBank[1],"Sindhi Revision");return;}
        vector<QuizQuestion> rev;
        for(auto& kv:quizBank) for(const auto& q:kv.second) rev.push_back(q);
        if((int)rev.size()>6) rev.resize(6);
        runQuizSession(rev,"Sindhi Revision Quiz");
    }
    void showVocabulary(const string& cat) override {
        if (!requireLogin()) return;
        if (!vocabulary.count(cat)){cout<<"Category not found.\nAvailable: greetings, numbers, colors, food, family\n";return;}
        cout<<"\n--- SINDHI VOCABULARY: "<<cat<<" ---\n";
        cout<<left<<setw(14)<<"English"<<setw(20)<<"Sindhi (Roman)"<<"Example\n"; line(60,'-');
        for (const auto& w:vocabulary.at(cat))
            cout<<left<<setw(14)<<w.english<<setw(20)<<w.punjabi<<w.example<<"\n";
    }
};

// ============================================================
//  BALOCHI CULTURE  (overrides INSIDE the class)
// ============================================================
class BalochiCulture : public Culture {
public:
    BalochiCulture() : Culture("Balochi", "Balochistan", 90) {
        addPhrase({"Assalam o Alaikum","Hello / Peace greeting"});
        addPhrase({"Tao cha ae?","How are you?"});
        addPhrase({"Man shukria","Thank you"});
        addPhrase({"Khuda Hafiz","Goodbye"});
        addPhrase({"Maaf kan","Sorry"});
        addPhrase({"Ha","Yes"});
        addPhrase({"Na","No"});
        addPhrase({"Tora naam cha aa?","What is your name?"});
        addPhrase({"Subah bakhair","Good Morning"});
        addPhrase({"Sham bakhair","Good Evening"});
        addPhrase({"Pani","Water"});
        addPhrase({"Khana","Food"});
        addPhrase({"Dost","Friend"});
        addPhrase({"Ustad","Teacher"});
        addPhrase({"Ghar","House"});
        addPhrase({"Kitab","Book"});
        addPhrase({"School","School"});
        addPhrase({"Hospital","Hospital"});
        addPhrase({"Dil khush","Happy"});
        addPhrase({"Bohot shukria","Thank you very much"});

        addProverb({"Sabar har mushkil ka hal aahe","Patience solves every problem","Patience"});
        addProverb({"Mehnat kamyabi ji kunji aahe","Hard work is key to success","Hard Work"});
        addProverb({"Sach hamesha jeet aahe","Truth always wins","Truth"});
        addProverb({"Dost mushkil mein pehchaan wende","Friend known in hardship","Friendship"});
        addProverb({"Waqt kisi ka intezar na karda","Time waits for none","Time"});
        addProverb({"Ilm roshni aahe","Knowledge is light","Education"});
        addProverb({"Zaban ji izzat zaroori aahe","Respect speech","Speech"});
        addProverb({"Achai kabhi zaya na thiye","Goodness never wasted","Goodness"});
        addProverb({"Mohabbat sab kuch jeet aahe","Love conquers all","Love"});
        addProverb({"Takabbur zawaal aahe","Pride leads to downfall","Humility"});
        addProverb({"Bahaduri se zindagi jeeyo","Live life with courage","Bravery"});
        addProverb({"Izzat sab se barh kar hai","Honor above all","Honor"});
        addProverb({"Jirga ka faisla sab ka faisla","The council's decision is everyone's","Community"});
        addProverb({"Pani ka bahao rasta banana ata hai","Water carves its own path","Perseverance"});
        addProverb({"Pahad ki tarah mazboot raho","Be strong like the mountains","Strength"});
        addProverb({"Sardar ki izzat qaum ki izzat hai","Chief's honor is the tribe's honor","Leadership"});
        addProverb({"Dushman se bhi seekhna chahiye","Learn even from the enemy","Wisdom"});
        addProverb({"Mehman khuda ka bheja hota hai","Guest is sent by God","Hospitality"});
        addProverb({"Sehat daulat se badi hai","Health is greater than wealth","Health"});
        addProverb({"Ukhuwwat hi zindagi hai","Brotherhood is life","Unity"});

        addTaboo({"Buzurgan ji beizzati na karo","Respect elders","Respect"});
        addTaboo({"Jhoot na bolo","Do not lie","Truth"});
        addTaboo({"Mehmaan ignore na karo","Respect guests","Hospitality"});
        addTaboo({"Zyada gussa na karo","Avoid anger","Calmness"});
        addTaboo({"Masjid mein shor na karo","No noise in mosque","Respect"});
        addTaboo({"Ghar ji baat bahar na le jao","Keep family matters private","Privacy"});
        addTaboo({"Zulm na karo","Do not oppress","Justice"});
        addTaboo({"Takabbur na karo","Avoid arrogance","Humility"});
        addTaboo({"Khane ki na-shukri na karo","Respect food","Gratitude"});
        addTaboo({"Bachon pe zulm na karo","Protect children","Kindness"});

        addTerm({"Doctor","Doctor","MEDICAL"}); addTerm({"Hospital","Hospital","MEDICAL"});
        addTerm({"Fever","Bukhar","MEDICAL"}); addTerm({"Court","Adalat","LEGAL"});
        addTerm({"Law","Qanoon","LEGAL"}); addTerm({"Judge","Jaj","LEGAL"});
        addTerm({"School","School","EDUCATIONAL"}); addTerm({"Teacher","Ustad","EDUCATIONAL"});
        addTerm({"Student","Talib e Ilm","EDUCATIONAL"}); addTerm({"Book","Kitab","EDUCATIONAL"});

        addAspect({"Balochi Culture","Rich tribal traditions and hospitality"});
        addAspect({"Balochi Dress","Traditional embroidered dresses and turbans"});
        addAspect({"Attan Dance","Traditional group dance"});
        addAspect({"Sibi Festival","Cultural festival of Balochistan"});
        addAspect({"Camel Culture","Camels used in transport and traditions"});
        addAspect({"Balochi Music","Folk music with soroz and rhythms"});
        addAspect({"Nomadic Lifestyle","Traditional migratory lifestyle"});
        addAspect({"Handicrafts","Carpets, embroidery and handmade goods"});
        addAspect({"Hospitality","Very strong guest respect culture"});
        addAspect({"Desert Life","Life in arid and desert regions"});

        addFood({"Sajji","Roasted whole chicken or lamb"});
        addFood({"Kaak","Traditional hard bread"});
        addFood({"Landhi","Sun-dried meat"});
        addFood({"Dumpukht","Slow cooked meat dish"});
        addFood({"Khaddi Kebab","Whole lamb barbecue style"});
        addFood({"Balochi Tikka","Spiced grilled meat"});
        addFood({"Rosh","Boiled meat dish"});
        addFood({"Chai","Tea"});
        addFood({"Sajji Rice","Rice served with sajji meat"});
        addFood({"Date sweets","Sweet dishes made from dates"});
    }

    string translate(string e) override {
        if (e=="hello")     return "Salaam";
        if (e=="thank you") return "Manana";
        if (e=="friend")    return "Dost";
        if (e=="love")      return "Meena";
        return "Translation not found";
    }
    CulturalTaboo getCulturalTaboo(string t) override {
        if (t=="tribal loyalty") return {"Never betray tribe","Loyalty","Tribal code"};
        if (t=="elders")         return {"Disrespecting elders forbidden","Respect","Strong tribal value"};
        if (t=="honor")          return {"Honor above all","Izzat","Core Balochi value"};
        return {"Not found","N/A","N/A"};
    }
    Proverb getProverb(string t) override {
        if (t=="bravery") return {"Bahaduri se zindagi jeeyo","Bravery","Courage is valued"};
        if (t=="honor")   return {"Izzat sab se barh kar hai","Honor","Honor above all"};
        return {"Not found","N/A","N/A"};
    }
    string getGreeting(int f) override {
        switch(f) {
            case 0: return "Salam dost!";
            case 1: return "Salaam";
            case 2: return "Manana";
            case 3: return "Khush Aamdeed";
            default: return "Salaam";
        }
    }
    string getDomainTerm(string e, string d) override {
        if (d=="tribal law") { if (e=="council") return "Jirga"; if (e=="chief") return "Sardar"; }
        if (d=="music")      { if (e=="dance") return "Chaap"; }
        return "Term not found";
    }
};

// ============================================================
//  BALOCHI LANGUAGE CLASS
// ============================================================
class Balochi : public Language {
private:
    map<string,string>             dictionary;
    map<string,vector<VocabWord>>  vocabulary;
    map<int,vector<QuizQuestion>>  quizBank;
    map<string,string>             pronunciationMap;

    void toLower(string& s) const { transform(s.begin(),s.end(),s.begin(),::tolower); }

    void runQuizSession(const vector<QuizQuestion>& questions, const string& label) {
        int correct=0, total=(int)questions.size();
        if (!total) { cout << "No questions.\n"; return; }
        cout << "\n===== " << label << " =====\n";
        for (int i=0;i<total;i++) {
            const QuizQuestion& q=questions[i];
            cout << "\nQ"<<i+1<<": "<<q.question<<"\nA. "<<q.options[0]
                 <<"\nB. "<<q.options[1]<<"\nC. "<<q.options[2]<<"\nD. "<<q.options[3]<<"\nAnswer: ";
            char ans=toupper(readChar()); int idx=ans-'A';
            if (idx==q.correctIndex){ cout<<"Correct!\n"; correct++; }
            else cout<<"Wrong! Correct: "<<(char)('A'+q.correctIndex)<<"\n";
        }
        int pts=correct*20;
        cout<<"\nScore: "<<correct<<"/"<<total<<"  Points: +"<<pts<<"\n";
        if (currentUser){ currentUser->addScore(pts); FileHandler::saveUser(*currentUser); }
    }

    void initDictionary() {
        dictionary["hello"]="Assalam o Alaikum"; dictionary["thank you"]="Shukria";
        dictionary["yes"]="Ha"; dictionary["no"]="Na"; dictionary["water"]="Pani";
        dictionary["food"]="Khana"; dictionary["friend"]="Dost"; dictionary["teacher"]="Ustad";
        dictionary["book"]="Kitab"; dictionary["house"]="Ghar"; dictionary["love"]="Mohabbat";
        dictionary["good"]="Acha"; dictionary["bad"]="Bura"; dictionary["go"]="Rawan";
        dictionary["come"]="Aao"; dictionary["write"]="Likhan"; dictionary["read"]="Parhan";
        dictionary["big"]="Wado"; dictionary["small"]="Choto"; dictionary["school"]="School";
    }
    void initVocabulary() {
        vocabulary["greetings"]={
            VocabWord("Hello",       "Salam","","","","","","","Greeting"),
            VocabWord("Good Morning","Subah bakhair","","","","","","","Morning"),
            VocabWord("Welcome",     "Khush aamdeed","","","","","","","Welcoming"),
            VocabWord("Goodbye",     "Khuda Hafiz","","","","","","","Farewell"),
            VocabWord("Thank you",   "Shukria","","","","","","","Gratitude")
        };
        vocabulary["numbers"]={
            VocabWord("One",  "Yak","","","","","","","Yak = 1"),
            VocabWord("Two",  "Do", "","","","","","","Do = 2"),
            VocabWord("Three","Say","","","","","","","Say = 3"),
            VocabWord("Four", "Char","","","","","","","Char = 4"),
            VocabWord("Five", "Panj","","","","","","","Panj = 5")
        };
        vocabulary["colors"]={
            VocabWord("Red",  "Sur",  "","","","","","","Sur rang"),
            VocabWord("Blue", "Kabood","","","","","","","Kabood rang"),
            VocabWord("Green","Sabz", "","","","","","","Sabz rang"),
            VocabWord("White","Spet", "","","","","","","Spet rang"),
            VocabWord("Black","Syah", "","","","","","","Syah rang")
        };
        vocabulary["food"]={
            VocabWord("Bread","Naan", "","","","","","","Naan khao"),
            VocabWord("Water","Aab",  "","","","","","","Aab piyo"),
            VocabWord("Meat", "Gosht","","","","","","","Gosht khaana"),
            VocabWord("Rice", "Birinj","","","","","","","Birinj pakao"),
            VocabWord("Tea",  "Chai", "","","","","","","Chai piyo")
        };
        vocabulary["family"]={
            VocabWord("Mother","Maan","","","","","","","Maan pyaari"),
            VocabWord("Father","Baap","","","","","","","Baap taaqatwar"),
            VocabWord("Brother","Bira","","","","","","","Bira saath"),
            VocabWord("Sister","Khuhar","","","","","","","Khuhar meherbaan"),
            VocabWord("Child","Bach","","","","","","","Bach masoom")
        };
    }
    void initQuizBank() {
        // ---- Beginner (5) ----
        { string o[4]={"Shukria","Salam","Kitab","Ghar"};
          quizBank[1].push_back(QuizQuestion("Thank you in Balochi?",o,0,"Shukria means thanks")); }
        { string o[4]={"Aab","Aag","Hawa","Zameen"};
          quizBank[1].push_back(QuizQuestion("Water in Balochi?",o,0,"Aab = water in Balochi")); }
        { string o[4]={"Dost","Dushman","Ghar","Kitab"};
          quizBank[1].push_back(QuizQuestion("Friend in Balochi?",o,0,"Dost = friend")); }
        { string o[4]={"Subah bakhair","Shab bakhair","Salam","Khuda Hafiz"};
          quizBank[1].push_back(QuizQuestion("Good Morning in Balochi?",o,0,"Subah bakhair = Good Morning")); }
        { string o[4]={"Maan","Baap","Bira","Bach"};
          quizBank[1].push_back(QuizQuestion("Mother in Balochi?",o,0,"Maan = Mother")); }
        // ---- Intermediate (5) ----
        { string o[4]={"Sajji","Kaak","Landhi","Rosh"};
          quizBank[2].push_back(QuizQuestion("Famous Balochi roasted whole lamb dish?",o,0,"Sajji is the iconic Balochi dish.")); }
        { string o[4]={"Sibi","Quetta","Turbat","Khuzdar"};
          quizBank[2].push_back(QuizQuestion("Famous cultural festival city in Balochistan?",o,0,"Sibi Festival.")); }
        { string o[4]={"Sardar","Jirga","Lashkar","Qazi"};
          quizBank[2].push_back(QuizQuestion("The Balochi tribal chief is called?",o,0,"Sardar = tribal chief")); }
        { string o[4]={"Brahui","Sindhi","Pashto","Persian"};
          quizBank[2].push_back(QuizQuestion("Which language is also spoken in Balochistan alongside Balochi?",o,0,"Brahui is a Dravidian language spoken in Balochistan.")); }
        { string o[4]={"Kaak","Sajji","Landhi","Dumpukht"};
          quizBank[2].push_back(QuizQuestion("Traditional hard Balochi bread?",o,0,"Kaak = traditional hard bread.")); }
        // ---- Advanced (5) ----
        { string o[4]={"Jirga","Loya Jirga","Panchayat","Majlis"};
          quizBank[3].push_back(QuizQuestion("Balochi tribal council system?",o,0,"Jirga is the traditional council.")); }
        { string o[4]={"Saroz","Dhol","Rubab","Sitar"};
          quizBank[3].push_back(QuizQuestion("Traditional Balochi musical instrument?",o,0,"Saroz is the iconic Balochi instrument.")); }
        { string o[4]={"Balochi","Brahui","Persian","Arabic"};
          quizBank[3].push_back(QuizQuestion("Balochi belongs to which language family?",o,2,"Balochi is an Iranian/Northwestern Iranian language.")); }
        { string o[4]={"Izzat","Melmastia","Jirga","Ghairat"};
          quizBank[3].push_back(QuizQuestion("Balochi concept of hospitality toward guests?",o,1,"Melmastia = hospitality code.")); }
        { string o[4]={"Mir Chakar Rind","Doda Gorgej","Bibi Nani","Gwahram"};
          quizBank[3].push_back(QuizQuestion("Famous Balochi hero from the 15th century epic?",o,0,"Mir Chakar Khan Rind is the legendary hero.")); }
    }
    void initPronunciation() {
        pronunciationMap["Assalam o Alaikum"]= "As-sa-lam o a-lai-kum (soft tone, formal greeting)";
        pronunciationMap["Shukria"]= "Shuk-ri-ya (light stress on first syllable)";
        pronunciationMap["Sajji"]  = "Saj-ji (double j held slightly, stress on first)";
        pronunciationMap["Salam"]  = "Sa-lam (two equal syllables, crisp final m)";
        pronunciationMap["Aab"]    = "Aab (long aa + b, like 'arm' + b)";
        pronunciationMap["Maan"]   = "Maan (long aa, like 'father' + n)";
        pronunciationMap["Dost"]   = "Dost (rhymes with 'most', soft d)";
        pronunciationMap["Jirga"]  = "Jir-ga (roll the r lightly, short final a)";
        pronunciationMap["Ghar"]   = "Ghur (gh from back of throat, held + ur)";
        pronunciationMap["Kaak"]   = "Kaak (long aa, like 'car' doubled, crisp final k)";
    }

public:
    Balochi() : Language("Balochi") {
        initDictionary(); initVocabulary(); initQuizBank(); initPronunciation();
    }
    void translate(const string& word) override {
        if (!requireLogin()) return;
        string key=word; toLower(key);
        cout << "\n--- Balochi Translation ---\n";
        if (dictionary.count(key)) cout << word << " = " << dictionary[key] << "\n";
        else cout << "Word not found.\nTry: hello, water, friend, teacher, love...\n";
    }
    void runQuiz(int level) override {
        if (!requireLogin()) return;
        string names[]={"","Beginner","Intermediate","Advanced"};
        if (level<1||level>3){cout<<"Invalid level.\n";return;}
        if (!quizBank.count(level)||quizBank[level].empty()){cout<<"No quiz available.\n";return;}
        runQuizSession(quizBank[level],"Balochi "+names[level]+" Quiz");
    }
    void startGame(int choice) override {
        if (!requireLogin()) return;
        vector<pair<string,string>> hangmanWords={
            {"aab","water"},{"ghar","home"},{"kitab","book"},
            {"dost","friend"},{"naan","bread"},{"sajji","Balochi roast"},
            {"maan","mother"},{"bira","brother"},{"ustad","teacher"},{"chai","tea"}};
        vector<pair<string,string>> matchPairs={
            {"hello","Salam"},{"water","Aab"},{"bread","Naan"},
            {"friend","Dost"},{"teacher","Ustad"},{"book","Kitab"},
            {"thank you","Shukria"},{"house","Ghar"},{"mother","Maan"},{"love","Mohabbat"}};
        if (choice==1){
            srand((unsigned int)time(0));
            int idx=rand()%(int)hangmanWords.size();
            string target=hangmanWords[idx].first,hint=hangmanWords[idx].second;
            string display(target.size(),'_'); set<char> tried; int wrong=0,maxWrong=6;
            cout<<"\n--- HANGMAN (Balochi) ---\nHint: "<<hint<<"  | Length: "<<target.size()<<"\n\n";
            while(wrong<maxWrong&&display!=target){
                cout<<"Word  : "; for(char c:display)cout<<c<<" ";
                cout<<"\nTried : "; for(char c:tried)cout<<c<<" ";
                cout<<"\nGuess a letter: "; char g=tolower(readChar());
                if(tried.count(g)){cout<<"Already tried!\n\n";continue;}
                tried.insert(g); bool hit=false;
                for(int i=0;i<(int)target.size();i++) if(target[i]==g){display[i]=g;hit=true;}
                if(hit)cout<<"Good guess!\n\n"; else{wrong++;cout<<"Wrong! ("<<wrong<<"/"<<maxWrong<<")\n\n";}
            }
            if(display==target){cout<<"CORRECT! Word: "<<target<<"\n";int earned=(maxWrong-wrong)*10;
                cout<<"Points: "<<earned<<"\n";
                if(currentUser){currentUser->addScore(earned);FileHandler::saveUser(*currentUser);}}
            else cout<<"Game over! Word was: "<<target<<"\n";
        } else if (choice==2){
            srand((unsigned int)time(0)); vector<pair<string,string>> sh=matchPairs;
            for(int i=(int)sh.size()-1;i>0;i--) swap(sh[i],sh[rand()%(i+1)]);
            int numQ=min(5,(int)sh.size()),score=0;
            cout<<"\n--- WORD MATCH (Balochi) ---\n";
            for(int i=0;i<numQ;i++){
                string correct=sh[i].second; vector<string> opts; opts.push_back(correct);
                vector<int> used; used.push_back(i);
                while((int)opts.size()<4){int r=rand()%(int)sh.size();bool dup=false;
                    for(int u:used) if(u==r){dup=true;break;}
                    if(!dup){opts.push_back(sh[r].second);used.push_back(r);}}
                for(int k=(int)opts.size()-1;k>0;k--) swap(opts[k],opts[rand()%(k+1)]);
                int ci=0; for(int k=0;k<4;k++) if(opts[k]==correct)ci=k;
                cout<<"Q"<<i+1<<" - \""<<sh[i].first<<"\"\n";
                for(int k=0;k<4;k++) cout<<"  "<<k+1<<". "<<opts[k]<<"\n";
                cout<<"Answer (1-4): "; int ch=readInt();
                if(ch-1==ci){cout<<"Correct!\n\n";score+=10;} else cout<<"Wrong! Answer: "<<correct<<"\n\n";
            }
            cout<<"Score: "<<score<<"/"<<numQ*10<<"\n";
            if(currentUser){currentUser->addScore(score);FileHandler::saveUser(*currentUser);}
        } else cout<<"Invalid game choice.\n";
    }
    void showProgress() override { if (!requireLogin()) return; currentUser->displayProfile(); }
    void showLeaderboard() override {
        auto e=FileHandler::loadLeaderboard(); cout<<"\n"; line(50,'=');
        cout<<"             GLOBAL LEADERBOARD\n"; line(50,'=');
        cout<<left<<setw(5)<<"Rank"<<setw(18)<<"Username"<<setw(10)<<"Score"<<"Streak\n"; line(50,'-');
        if(e.empty())cout<<"  No entries yet.\n";
        for(int i=0;i<(int)e.size()&&i<10;i++)
            cout<<left<<setw(5)<<i+1<<setw(18)<<e[i].username<<setw(10)<<e[i].score<<e[i].streak<<" days\n";
        line(50,'=');
    }
    void loadModule(int level) override {
        if (!requireLogin()) return;
        map<int,vector<string>> cats;
        cats[1]={"greetings","numbers"}; cats[2]={"colors","food"}; cats[3]={"family"};
        if (!cats.count(level)){cout<<"Invalid level.\n";return;}
        string names[]={"","Beginner","Intermediate","Advanced"};
        cout<<"\n"; line(50,'='); cout<<"  BALOCHI VOCABULARY -- "<<names[level]<<"\n"; line(50,'=');
        for (const string& cat:cats[level]){
            cout<<"\n  ["<<cat<<"]\n"; cout<<left<<setw(14)<<"English"<<setw(20)<<"Balochi"<<"Example\n"; line(60,'-');
            if (!vocabulary.count(cat)) continue;
            for (const auto& w:vocabulary.at(cat)){
                cout<<left<<setw(14)<<w.english<<setw(20)<<w.punjabi<<w.example<<"\n";
                currentUser->addMasteredWord(w.english);
            }
        }
        currentUser->awardBadge("First Steps"); FileHandler::saveUser(*currentUser);
    }
    void dailyChallenge() override {
        if (!requireLogin()) return;
        cout<<"\n=== DAILY BALOCHI CHALLENGE -- "<<User::getCurrentDate()<<" ===\n";
        srand((unsigned int)time(0)); auto& pool=quizBank[1];
        if(pool.empty()){cout<<"No questions.\n";return;}
        vector<QuizQuestion> daily; set<int> picked;
        for(int att=0;(int)daily.size()<3&&att<100;att++){int r=rand()%(int)pool.size();
            if(!picked.count(r)){daily.push_back(pool[r]);picked.insert(r);}}
        runQuizSession(daily,"Daily Balochi Challenge");
        currentUser->updateStreak(); FileHandler::saveUser(*currentUser);
        cout<<"Streak: "<<currentUser->getStreak()<<" days\n";
    }
    void pronunciationGuide(const string& word) override {
        if (!requireLogin()) return;
        cout<<"\n--- BALOCHI PRONUNCIATION: "<<word<<" ---\n";
        if (pronunciationMap.count(word)){
            cout<<pronunciationMap.at(word)<<"\n";
            cout<<"\n--- General Balochi Rules ---\n";
            cout<<"  1. Aspirated consonants (kh, gh): from back of throat\n";
            cout<<"  2. Long vowels (aa, ee): held twice as long\n";
            cout<<"  3. Balochi is an Iranian language, stress on root syllables\n";
        } else {
            cout<<"No guide for '"<<word<<"'.\n";
            cout<<"Available: Assalam o Alaikum, Shukria, Sajji, Salam, Aab, Maan, Dost, Jirga, Ghar, Kaak\n";
        }
    }
    void revisionQuiz() override {
        if (!requireLogin()) return;
        auto weak=currentUser->getWeakAreas(); cout<<"\n=== BALOCHI REVISION QUIZ ===\n";
        if(weak.empty()){cout<<"No weak areas! General refresher.\n\n";runQuizSession(quizBank[1],"Balochi Revision");return;}
        vector<QuizQuestion> rev;
        for(auto& kv:quizBank) for(const auto& q:kv.second) rev.push_back(q);
        if((int)rev.size()>6) rev.resize(6);
        runQuizSession(rev,"Balochi Revision Quiz");
    }
    void showVocabulary(const string& cat) override {
        if (!requireLogin()) return;
        if (!vocabulary.count(cat)){cout<<"Category not found.\nAvailable: greetings, numbers, colors, food, family\n";return;}
        cout<<"\n--- BALOCHI VOCABULARY: "<<cat<<" ---\n";
        cout<<left<<setw(14)<<"English"<<setw(20)<<"Balochi"<<"Example\n"; line(60,'-');
        for (const auto& w:vocabulary.at(cat))
            cout<<left<<setw(14)<<w.english<<setw(20)<<w.punjabi<<w.example<<"\n";
    }
};

// ============================================================
//  PASHTO CULTURE  (overrides INSIDE the class)
// ============================================================
class PashtoCulture : public Culture {
public:
    PashtoCulture() : Culture("Pashto", "Khyber Pakhtunkhwa / Afghanistan", 90) {
        addPhrase({"Assalam o Alaikum","Peace greeting"});
        addPhrase({"Sta sanga ye?","How are you?"});
        addPhrase({"Manana","Thank you"});
        addPhrase({"Khuda Hafiz","Goodbye"});
        addPhrase({"Bakhshana","Sorry"});
        addPhrase({"Ho","Yes"});
        addPhrase({"Na","No"});
        addPhrase({"Sta nom sa da?","What is your name?"});
        addPhrase({"Sabah bakhair","Good Morning"});
        addPhrase({"Shpa bakhair","Good Night"});
        addPhrase({"Pani","Water"});
        addPhrase({"Doodai","Food"});
        addPhrase({"Malgaray","Friend"});
        addPhrase({"Ustad","Teacher"});
        addPhrase({"Kor","House"});
        addPhrase({"Kitab","Book"});
        addPhrase({"Makhtab","School"});
        addPhrase({"Hospital","Hospital"});
        addPhrase({"Zra kha de","I am happy"});
        addPhrase({"Dera manana","Thank you very much"});

        addProverb({"Sabar ka phal khwand war kawi","Patience brings reward","Patience"});
        addProverb({"Ilm roshni da","Knowledge is light","Education"});
        addProverb({"Mehnat kamyabi ra warkawi","Hard work brings success","Hard Work"});
        addProverb({"Rastya hamesha ghalaba kawi","Truth always wins","Truth"});
        addProverb({"Dost pa sakhtyo ke pejani","Friend known in hardship","Friendship"});
        addProverb({"Wakht hes cha ta intizar na kawi","Time waits for none","Time"});
        addProverb({"Neki kabhi zaya na ze","Good deeds never wasted","Goodness"});
        addProverb({"Zaban zakhm rawanawi","Words hurt deeply","Speech"});
        addProverb({"Mohabbat har shey ghat kawi","Love conquers all","Love"});
        addProverb({"Takabbur bad naseebi da","Pride is downfall","Humility"});
        addProverb({"Melmastia da izzat da","Hospitality is honor","Hospitality"});
        addProverb({"Da shujaat pa zra ke da","Bravery lies in the heart","Courage"});
        addProverb({"Pashtunwali da zindagi da","Pashtunwali is the code of life","Ethics"});
        addProverb({"Kor de watan da","Home is the homeland","Patriotism"});
        addProverb({"Ghairat da Pukhto da","Honor is Pashto","Pride"});
        addProverb({"Zan zar zameen","Woman, gold, land are the three honors","Values"});
        addProverb({"Badal da Pukhto da","Revenge is Pashto code","Code"});
        addProverb({"Nanawati da ghairat da","Seeking asylum is honorable","Forgiveness"});
        addProverb({"Jirga da insaf da","Jirga is justice","Justice"});
        addProverb({"Ittifaq da taraf da","Unity brings victory","Unity"});

        addTaboo({"Buzurgano ta be-izzati makawa","Respect elders","Respect"});
        addTaboo({"Drugh ma waya","Do not lie","Truth"});
        addTaboo({"Mehmaan na ignore kawa","Respect guests","Hospitality"});
        addTaboo({"Zyat ghusa makawa","Avoid anger","Calmness"});
        addTaboo({"Masjid ke shor makawa","No noise in mosque","Respect"});
        addTaboo({"Ghar safar baat na rawarawa","Keep family matters private","Privacy"});
        addTaboo({"Zulm makawa","Do not oppress","Justice"});
        addTaboo({"Takabbur makawa","Avoid arrogance","Humility"});
        addTaboo({"Khware na zaya kawa","Don't waste food","Gratitude"});
        addTaboo({"Bachyan ta zarar makawa","Protect children","Kindness"});

        addTerm({"Doctor","Doctor","MEDICAL"}); addTerm({"Hospital","Hospital","MEDICAL"});
        addTerm({"Fever","Bukhar","MEDICAL"}); addTerm({"Court","Adalat","LEGAL"});
        addTerm({"Law","Qanoon","LEGAL"}); addTerm({"Judge","Jaj","LEGAL"});
        addTerm({"School","School","EDUCATIONAL"}); addTerm({"Teacher","Ustad","EDUCATIONAL"});
        addTerm({"Student","Shagird","EDUCATIONAL"}); addTerm({"Book","Kitab","EDUCATIONAL"});

        addAspect({"Pashto Culture","Strong tribal values and traditions"});
        addAspect({"Attan Dance","Traditional Pashtun group dance"});
        addAspect({"Pashto Poetry","Rich tradition from Khushal Khan Khattak"});
        addAspect({"Hospitality (Melmastia)","Very strong guest respect culture"});
        addAspect({"Pashtunwali","Traditional ethical code of Pashtuns"});
        addAspect({"Traditional Dress","Shalwar kameez with pakol cap"});
        addAspect({"Jirga System","Community council decision system"});
        addAspect({"Rubab Music","Rubab and folk music tradition"});
        addAspect({"Mountain Life","Life in rugged hilly regions"});
        addAspect({"Tribal Unity","Strong clan-based society"});

        addFood({"Kabuli Pulao","Rice with meat and raisins"});
        addFood({"Chapli Kebab","Spicy minced meat patties"});
        addFood({"Namak Mandi Karahi","Traditional meat karahi"});
        addFood({"Sajji","Roasted meat dish"});
        addFood({"Rosh","Boiled meat dish"});
        addFood({"Landhi","Sun-dried meat"});
        addFood({"Bolani","Stuffed flatbread"});
        addFood({"Chai","Tea"});
        addFood({"Mantu","Dumplings"});
        addFood({"Sheer Chai","Milk tea"});
    }

    string translate(string e) override {
        if (e=="hello")     return "Da Khoday Pa Aman";
        if (e=="thank you") return "Manana";
        if (e=="yes")       return "Bale";
        if (e=="no")        return "Na";
        if (e=="friend")    return "Dost";
        return "Translation not found";
    }
    CulturalTaboo getCulturalTaboo(string t) override {
        if (t=="guest")   return {"Never mistreat guest","Hospitality","Pashtunwali code"};
        if (t=="revenge") return {"Badal is sacred","Revenge","Part of Pashtunwali"};
        if (t=="elders")  return {"Respect elders","Avoid disrespect","Core Pashto value"};
        return {"Not found","N/A","N/A"};
    }
    Proverb getProverb(string t) override {
        if (t=="hospitality") return {"Melmastia da izzat da","Hospitality","Hospitality is honor"};
        if (t=="courage")     return {"Da shujaat pa zra ke da","Courage","Bravery lies in the heart"};
        return {"Not found","N/A","N/A"};
    }
    string getGreeting(int f) override {
        switch(f) {
            case 0: return "Staray Mashay!";
            case 1: return "Da Khoday Pa Aman";
            case 2: return "Khush Aamdeed";
            case 3: return "Pa Aman";
            default: return "Da Khoday Pa Aman";
        }
    }
    string getDomainTerm(string e, string d) override {
        if (d=="poetry") { if (e=="poem") return "Sher"; if (e=="poet") return "Shaair"; }
        if (d=="law")    { if (e=="code") return "Pashtunwali"; }
        return "Term not found";
    }
};

// ============================================================
//  PASHTO LANGUAGE CLASS
// ============================================================
class Pashto : public Language {
private:
    map<string,string>            dictionary;
    map<string,vector<VocabWord>> vocabulary;
    map<int,vector<QuizQuestion>> quizBank;
    map<string,string>            pronunciationMap;
    void toLower(string& s) const { transform(s.begin(),s.end(),s.begin(),::tolower); }

    void runQuizSession(const vector<QuizQuestion>& questions, const string& label) {
        int correct=0,total=(int)questions.size();
        if (!total){ cout<<"No questions.\n"; return; }
        cout<<"\n====="<<label<<"=====\n";
        for (int i=0;i<total;i++){
            const QuizQuestion& q=questions[i];
            cout<<"\nQ"<<i+1<<": "<<q.question<<"\nA. "<<q.options[0]<<"\nB. "<<q.options[1]
                <<"\nC. "<<q.options[2]<<"\nD. "<<q.options[3]<<"\nAnswer: ";
            char ans=toupper(readChar()); int idx=ans-'A';
            if (idx==q.correctIndex){ cout<<"Correct!\n"; correct++; }
            else cout<<"Wrong! Correct: "<<(char)('A'+q.correctIndex)<<"\n";
        }
        int pts=correct*20;
        cout<<"\nScore: "<<correct<<"/"<<total<<"  Points: +"<<pts<<"\n";
        if (currentUser){ currentUser->addScore(pts); FileHandler::saveUser(*currentUser); }
    }

    void initDictionary() {
        dictionary["hello"]="Assalam o Alaikum"; dictionary["thank you"]="Manana";
        dictionary["yes"]="Ho"; dictionary["no"]="Na"; dictionary["water"]="Oba";
        dictionary["food"]="Doodai"; dictionary["friend"]="Malgaray"; dictionary["teacher"]="Ustad";
        dictionary["book"]="Kitab"; dictionary["house"]="Kor"; dictionary["love"]="Meena";
        dictionary["good"]="Shta"; dictionary["bad"]="Bad"; dictionary["go"]="Zaa";
        dictionary["come"]="Rasha"; dictionary["write"]="Likal"; dictionary["read"]="Lalal";
        dictionary["big"]="Loya"; dictionary["small"]="Wach"; dictionary["school"]="Makhtab";
    }
    void initVocabulary() {
        vocabulary["greetings"]={
            VocabWord("Hello",       "Da Khoday Pa Aman","","","","","","","Pashto greeting"),
            VocabWord("Good Morning","Sabah bakhair","","","","","","","Morning greeting"),
            VocabWord("Welcome",     "Khush Aamdeed","","","","","","","Welcoming guests"),
            VocabWord("Goodbye",     "Khuda Hafiz","","","","","","","Farewell"),
            VocabWord("Thank you",   "Manana","","","","","","","Gratitude")
        };
        vocabulary["numbers"]={
            VocabWord("One",  "Yaw","","","","","","","Yaw = 1"),
            VocabWord("Two",  "Dwa","","","","","","","Dwa = 2"),
            VocabWord("Three","Dre","","","","","","","Dre = 3"),
            VocabWord("Four", "Salor","","","","","","","Salor = 4"),
            VocabWord("Five", "Pinja","","","","","","","Pinja = 5")
        };
        vocabulary["colors"]={
            VocabWord("Red",  "Sur",  "","","","","","","Sur rang"),
            VocabWord("Blue", "Sheen","","","","","","","Sheen rang"),
            VocabWord("Green","Shne", "","","","","","","Shne rang"),
            VocabWord("White","Spin", "","","","","","","Spin rang"),
            VocabWord("Black","Tor",  "","","","","","","Tor rang")
        };
        vocabulary["food"]={
            VocabWord("Bread","Dodai",  "","","","","","","Dodai khao"),
            VocabWord("Water","Oba",    "","","","","","","Oba piyo"),
            VocabWord("Meat", "Gosht",  "","","","","","","Gosht khaana"),
            VocabWord("Rice", "Wruze",  "","","","","","","Wruze pakao"),
            VocabWord("Tea",  "Chai",   "","","","","","","Chai piyo")
        };
        vocabulary["family"]={
            VocabWord("Mother","Mor",  "","","","","","","Mor pyaari"),
            VocabWord("Father","Plaar","","","","","","","Plaar taaqatwar"),
            VocabWord("Brother","Wror","","","","","","","Wror saath"),
            VocabWord("Sister","Khor", "","","","","","","Khor meherbaan"),
            VocabWord("Child","Halak", "","","","","","","Halak masoom")
        };
    }
    void initQuizBank() {
        // ---- Beginner (5) ----
        { string o[4]={"Manana","Shukria","Salam","Kitab"};
          quizBank[1].push_back(QuizQuestion("Thank you in Pashto?",o,0,"Manana means thank you")); }
        { string o[4]={"Oba","Aag","Hawa","Zameen"};
          quizBank[1].push_back(QuizQuestion("Water in Pashto?",o,0,"Oba = water in Pashto")); }
        { string o[4]={"Malgaray","Dushman","Ghar","Kitab"};
          quizBank[1].push_back(QuizQuestion("Friend in Pashto?",o,0,"Malgaray = friend")); }
        { string o[4]={"Mor","Plaar","Wror","Khor"};
          quizBank[1].push_back(QuizQuestion("Mother in Pashto?",o,0,"Mor = Mother in Pashto")); }
        { string o[4]={"Sabah bakhair","Shpa bakhair","Da Khoday","Manana"};
          quizBank[1].push_back(QuizQuestion("Good Morning in Pashto?",o,0,"Sabah bakhair = Good Morning")); }
        // ---- Intermediate (5) ----
        { string o[4]={"Pashtunwali","Pakhtunkhwa","Jirga","Lashkar"};
          quizBank[2].push_back(QuizQuestion("The traditional code of Pashtun ethics?",o,0,"Pashtunwali is the tribal code.")); }
        { string o[4]={"Chapli Kebab","Sajji","Karahi","Pulao"};
          quizBank[2].push_back(QuizQuestion("Famous flat minced meat kebab from Peshawar?",o,0,"Chapli Kebab.")); }
        { string o[4]={"Melmastia","Badal","Nanawati","Ghairat"};
          quizBank[2].push_back(QuizQuestion("Pashtunwali principle of hospitality?",o,0,"Melmastia = hospitality.")); }
        { string o[4]={"Jirga","Loya Jirga","Panchayat","Majlis"};
          quizBank[2].push_back(QuizQuestion("Pashtun community council system?",o,0,"Jirga = community council.")); }
        { string o[4]={"Attan","Khattak","Bhangra","Luddi"};
          quizBank[2].push_back(QuizQuestion("Traditional Pashtun group dance?",o,0,"Attan is the traditional Pashtun dance.")); }
        // ---- Advanced (5) ----
        { string o[4]={"Khushal Khan Khattak","Rehman Baba","Hamza Baba","Ghani Khan"};
          quizBank[3].push_back(QuizQuestion("The warrior poet and national hero of Pashtuns?",o,0,"Khushal Khan Khattak.")); }
        { string o[4]={"Iranian","Turkic","Dravidian","Semitic"};
          quizBank[3].push_back(QuizQuestion("Pashto belongs to which language family?",o,0,"Pashto is an Eastern Iranian language.")); }
        { string o[4]={"Badal","Nanawati","Melmastia","Nang"};
          quizBank[3].push_back(QuizQuestion("Pashtunwali code for seeking asylum/forgiveness?",o,1,"Nanawati = seeking asylum.")); }
        { string o[4]={"Rubab","Dhol","Tabla","Sitar"};
          quizBank[3].push_back(QuizQuestion("Traditional Pashto national instrument?",o,0,"Rubab is the national instrument of Afghanistan/Pashto culture.")); }
        { string o[4]={"Subject-Object-Verb","Subject-Verb-Object","Verb-Object-Subject","Object-Subject-Verb"};
          quizBank[3].push_back(QuizQuestion("Pashto sentence structure (like Urdu)?",o,0,"Pashto follows SOV order.")); }
    }
    void initPronunciation() {
        pronunciationMap["Manana"]   = "Ma-na-na (equal stress, soft and respectful)";
        pronunciationMap["Assalam o Alaikum"] = "As-salam o alai-kum (formal tone)";
        pronunciationMap["Oba"]      = "O-ba (short O + ba, Pashto for water)";
        pronunciationMap["Malgaray"] = "Mal-ga-ray (3 syllables, stress on first)";
        pronunciationMap["Mor"]      = "Mor (rhymes with 'more', short and crisp)";
        pronunciationMap["Chapli"]   = "Chap-li (stress on first, like 'chaplet')";
        pronunciationMap["Pashtunwali"] = "Push-tun-wa-li (4 equal syllables, soft final i)";
        pronunciationMap["Melmastia"] = "Mel-mas-ti-a (4 syllables, stress on 'mas')";
        pronunciationMap["Kor"]      = "Kor (like 'core', Pashto for house)";
        pronunciationMap["Attan"]    = "At-tan (double t held, short final an)";
    }

public:
    Pashto() : Language("Pashto") {
        initDictionary(); initVocabulary(); initQuizBank(); initPronunciation();
    }
    void translate(const string& word) override {
        if (!requireLogin()) return;
        string key=word; toLower(key);
        cout<<"\n--- Pashto Translation ---\n";
        if (dictionary.count(key)) cout<<word<<" = "<<dictionary[key]<<"\n";
        else cout<<"Word not found.\nTry: hello, water, friend, love, teacher...\n";
    }
    void runQuiz(int level) override {
        if (!requireLogin()) return;
        string names[]={"","Beginner","Intermediate","Advanced"};
        if (level<1||level>3){cout<<"Invalid level.\n";return;}
        if (!quizBank.count(level)||quizBank[level].empty()){cout<<"No quiz available.\n";return;}
        runQuizSession(quizBank[level],"Pashto "+names[level]+" Quiz");
    }
    void startGame(int choice) override {
        if (!requireLogin()) return;
        vector<pair<string,string>> hangmanWords={
            {"oba","water"},{"kor","home"},{"kitab","book"},
            {"malgaray","friend"},{"dodai","bread"},{"mor","mother"},
            {"wror","brother"},{"ustad","teacher"},{"chai","tea"},{"meena","love"}};
        vector<pair<string,string>> matchPairs={
            {"hello","Da Khoday Pa Aman"},{"water","Oba"},{"bread","Dodai"},
            {"friend","Malgaray"},{"teacher","Ustad"},{"book","Kitab"},
            {"thank you","Manana"},{"house","Kor"},{"mother","Mor"},{"love","Meena"}};
        if (choice==1){
            srand((unsigned int)time(0));
            int idx=rand()%(int)hangmanWords.size();
            string target=hangmanWords[idx].first,hint=hangmanWords[idx].second;
            string display(target.size(),'_'); set<char> tried; int wrong=0,maxWrong=6;
            cout<<"\n--- HANGMAN (Pashto) ---\nHint: "<<hint<<"  | Length: "<<target.size()<<"\n\n";
            while(wrong<maxWrong&&display!=target){
                cout<<"Word  : "; for(char c:display)cout<<c<<" ";
                cout<<"\nTried : "; for(char c:tried)cout<<c<<" ";
                cout<<"\nGuess a letter: "; char g=tolower(readChar());
                if(tried.count(g)){cout<<"Already tried!\n\n";continue;}
                tried.insert(g); bool hit=false;
                for(int i=0;i<(int)target.size();i++) if(target[i]==g){display[i]=g;hit=true;}
                if(hit)cout<<"Good guess!\n\n"; else{wrong++;cout<<"Wrong! ("<<wrong<<"/"<<maxWrong<<")\n\n";}
            }
            if(display==target){cout<<"CORRECT! Word: "<<target<<"\n";int earned=(maxWrong-wrong)*10;
                cout<<"Points: "<<earned<<"\n";
                if(currentUser){currentUser->addScore(earned);FileHandler::saveUser(*currentUser);}}
            else cout<<"Game over! Word was: "<<target<<"\n";
        } else if (choice==2){
            srand((unsigned int)time(0)); vector<pair<string,string>> sh=matchPairs;
            for(int i=(int)sh.size()-1;i>0;i--) swap(sh[i],sh[rand()%(i+1)]);
            int numQ=min(5,(int)sh.size()),score=0;
            cout<<"\n--- WORD MATCH (Pashto) ---\n";
            for(int i=0;i<numQ;i++){
                string correct=sh[i].second; vector<string> opts; opts.push_back(correct);
                vector<int> used; used.push_back(i);
                while((int)opts.size()<4){int r=rand()%(int)sh.size();bool dup=false;
                    for(int u:used) if(u==r){dup=true;break;}
                    if(!dup){opts.push_back(sh[r].second);used.push_back(r);}}
                for(int k=(int)opts.size()-1;k>0;k--) swap(opts[k],opts[rand()%(k+1)]);
                int ci=0; for(int k=0;k<4;k++) if(opts[k]==correct)ci=k;
                cout<<"Q"<<i+1<<" - \""<<sh[i].first<<"\"\n";
                for(int k=0;k<4;k++) cout<<"  "<<k+1<<". "<<opts[k]<<"\n";
                cout<<"Answer (1-4): "; int ch=readInt();
                if(ch-1==ci){cout<<"Correct!\n\n";score+=10;} else cout<<"Wrong! Answer: "<<correct<<"\n\n";
            }
            cout<<"Score: "<<score<<"/"<<numQ*10<<"\n";
            if(currentUser){currentUser->addScore(score);FileHandler::saveUser(*currentUser);}
        } else cout<<"Invalid game choice.\n";
    }
    void showProgress() override { if (!requireLogin()) return; currentUser->displayProfile(); }
    void showLeaderboard() override {
        auto e=FileHandler::loadLeaderboard(); cout<<"\n"; line(50,'=');
        cout<<"             GLOBAL LEADERBOARD\n"; line(50,'=');
        cout<<left<<setw(5)<<"Rank"<<setw(18)<<"Username"<<setw(10)<<"Score"<<"Streak\n"; line(50,'-');
        if(e.empty())cout<<"  No entries yet.\n";
        for(int i=0;i<(int)e.size()&&i<10;i++)
            cout<<left<<setw(5)<<i+1<<setw(18)<<e[i].username<<setw(10)<<e[i].score<<e[i].streak<<" days\n";
        line(50,'=');
    }
    void loadModule(int level) override {
        if (!requireLogin()) return;
        map<int,vector<string>> cats;
        cats[1]={"greetings","numbers"}; cats[2]={"colors","food"}; cats[3]={"family"};
        if (!cats.count(level)){cout<<"Invalid level.\n";return;}
        string names[]={"","Beginner","Intermediate","Advanced"};
        cout<<"\n"; line(50,'='); cout<<"  PASHTO VOCABULARY -- "<<names[level]<<"\n"; line(50,'=');
        for (const string& cat:cats[level]){
            cout<<"\n  ["<<cat<<"]\n"; cout<<left<<setw(14)<<"English"<<setw(20)<<"Pashto"<<"Example\n"; line(60,'-');
            if (!vocabulary.count(cat)) continue;
            for (const auto& w:vocabulary.at(cat)){
                cout<<left<<setw(14)<<w.english<<setw(20)<<w.punjabi<<w.example<<"\n";
                currentUser->addMasteredWord(w.english);
            }
        }
        currentUser->awardBadge("First Steps"); FileHandler::saveUser(*currentUser);
    }
    void dailyChallenge() override {
        if (!requireLogin()) return;
        cout<<"\n=== DAILY PASHTO CHALLENGE -- "<<User::getCurrentDate()<<" ===\n";
        srand((unsigned int)time(0)); auto& pool=quizBank[1];
        if(pool.empty()){cout<<"No questions.\n";return;}
        vector<QuizQuestion> daily; set<int> picked;
        for(int att=0;(int)daily.size()<3&&att<100;att++){int r=rand()%(int)pool.size();
            if(!picked.count(r)){daily.push_back(pool[r]);picked.insert(r);}}
        runQuizSession(daily,"Daily Pashto Challenge");
        currentUser->updateStreak(); FileHandler::saveUser(*currentUser);
        cout<<"Streak: "<<currentUser->getStreak()<<" days\n";
    }
    void pronunciationGuide(const string& word) override {
        if (!requireLogin()) return;
        cout<<"\n--- PASHTO PRONUNCIATION: "<<word<<" ---\n";
        if (pronunciationMap.count(word)){
            cout<<pronunciationMap.at(word)<<"\n";
            cout<<"\n--- General Pashto Rules ---\n";
            cout<<"  1. Retroflex consonants: tongue tip curves back\n";
            cout<<"  2. Short vowels are very common in Pashto\n";
            cout<<"  3. Pashto is stress-initial; first syllable loudest\n";
        } else {
            cout<<"No guide for '"<<word<<"'.\n";
            cout<<"Available: Manana, Assalam o Alaikum, Oba, Malgaray, Mor, Chapli, Pashtunwali, Melmastia, Kor, Attan\n";
        }
    }
    void revisionQuiz() override {
        if (!requireLogin()) return;
        auto weak=currentUser->getWeakAreas(); cout<<"\n=== PASHTO REVISION QUIZ ===\n";
        if(weak.empty()){cout<<"No weak areas! General refresher.\n\n";runQuizSession(quizBank[1],"Pashto Revision");return;}
        vector<QuizQuestion> rev;
        for(auto& kv:quizBank) for(const auto& q:kv.second) rev.push_back(q);
        if((int)rev.size()>6) rev.resize(6);
        runQuizSession(rev,"Pashto Revision Quiz");
    }
    void showVocabulary(const string& cat) override {
        if (!requireLogin()) return;
        if (!vocabulary.count(cat)){cout<<"Category not found.\nAvailable: greetings, numbers, colors, food, family\n";return;}
        cout<<"\n--- PASHTO VOCABULARY: "<<cat<<" ---\n";
        cout<<left<<setw(14)<<"English"<<setw(20)<<"Pashto"<<"Example\n"; line(60,'-');
        for (const auto& w:vocabulary.at(cat))
            cout<<left<<setw(14)<<w.english<<setw(20)<<w.punjabi<<w.example<<"\n";
    }
};

// ============================================================
//  KASHMIRI CULTURE  (overrides INSIDE the class)
// ============================================================
class KashmiriCulture : public Culture {
public:
    KashmiriCulture() : Culture("Kashmiri", "Kashmir", 90) {
        addPhrase({"Assalam o Alaikum","Peace greeting"});
        addPhrase({"Tuhhi chukh kyah haal?","How are you?"});
        addPhrase({"Shukriya","Thank you"});
        addPhrase({"Khuda Hafiz","Goodbye"});
        addPhrase({"Maaf kariv","Sorry"});
        addPhrase({"Hyun","Yes"});
        addPhrase({"Na","No"});
        addPhrase({"Tuhund naam kya chukh?","What is your name?"});
        addPhrase({"Subah bakhair","Good Morning"});
        addPhrase({"Sham bakhair","Good Evening"});
        addPhrase({"Pani","Water"});
        addPhrase({"Khanas","Food"});
        addPhrase({"Yaar","Friend"});
        addPhrase({"Ustad","Teacher"});
        addPhrase({"Ghar","House"});
        addPhrase({"Kitab","Book"});
        addPhrase({"School","School"});
        addPhrase({"Hospital","Hospital"});
        addPhrase({"Dil khush chukh","I am happy"});
        addPhrase({"Bohat shukriya","Thank you very much"});

        addProverb({"Sabar kariv ta fal mithas chukh","Patience brings sweet reward","Patience"});
        addProverb({"Ilm roshni chukh","Knowledge is light","Education"});
        addProverb({"Mehnat kamyabi chu","Hard work leads to success","Hard Work"});
        addProverb({"Sach hamesha jeetith chukh","Truth always wins","Truth"});
        addProverb({"Dost mushkil man pehchan","Friend known in hardship","Friendship"});
        addProverb({"Waqt kisih ka intezar na kare","Time waits for none","Time"});
        addProverb({"Achai kabhi zaya na chukh","Good deeds never wasted","Goodness"});
        addProverb({"Zaban ka zakham gehra chukh","Words hurt deeply","Speech"});
        addProverb({"Mohabbat sab kuch jeetith chukh","Love conquers all","Love"});
        addProverb({"Takabbur zawaal chukh","Pride leads to downfall","Humility"});
        addProverb({"Sabar chu gul","Patience is a flower","Patience"});
        addProverb({"Sach chu roshan","Truth is light","Truth"});
        addProverb({"Mehman chu barkat","Guest brings blessings","Hospitality"});
        addProverb({"Dal manz machhli","Fish in Dal Lake (life at its source)","Nature"});
        addProverb({"Chinar chu taaqat","Chinar tree is strength","Resilience"});
        addProverb({"Zafran chu qeemat","Saffron is precious","Value"});
        addProverb({"Pheran chu watan","Pheran is the homeland (clothing as identity)","Identity"});
        addProverb({"Kehwa chu sehat","Kehwa is health","Wellness"});
        addProverb({"Kashmir chu jannat","Kashmir is paradise","Beauty"});
        addProverb({"Wazwan chu izzat","Wazwan feast is honor","Hospitality"});

        addTaboo({"Buzurgan ki beizzati na kariv","Respect elders","Respect"});
        addTaboo({"Jhoot na boliv","Do not lie","Truth"});
        addTaboo({"Mehmaan ignore na kariv","Respect guests","Hospitality"});
        addTaboo({"Gussa control kariv","Control anger","Calmness"});
        addTaboo({"Masjid mein shor na kariv","No noise in mosque","Respect"});
        addTaboo({"Ghar ki baat bahar na kariv","Keep family matters private","Privacy"});
        addTaboo({"Zulm na kariv","Do not oppress","Justice"});
        addTaboo({"Takabbur na kariv","Avoid arrogance","Humility"});
        addTaboo({"Khane ki na-shukri na kariv","Respect food","Gratitude"});
        addTaboo({"Bachon ko pyaar kariv","Love children","Kindness"});

        addTerm({"Doctor","Doctor","MEDICAL"}); addTerm({"Hospital","Hospital","MEDICAL"});
        addTerm({"Fever","Bukhar","MEDICAL"}); addTerm({"Court","Adalat","LEGAL"});
        addTerm({"Law","Qanoon","LEGAL"}); addTerm({"Judge","Jaj","LEGAL"});
        addTerm({"School","School","EDUCATIONAL"}); addTerm({"Teacher","Ustad","EDUCATIONAL"});
        addTerm({"Student","Talib e Ilm","EDUCATIONAL"}); addTerm({"Book","Kitab","EDUCATIONAL"});

        addAspect({"Kashmiri Culture","Rich heritage of mountains and valleys"});
        addAspect({"Pheran Dress","Traditional long woolen dress"});
        addAspect({"Wazwan Feast","Traditional multi-course meal"});
        addAspect({"Saffron Farming","Famous saffron cultivation in Kashmir"});
        addAspect({"Shikara Ride","Boat rides on Dal Lake"});
        addAspect({"Pashmina Shawls","World-famous fine wool shawls"});
        addAspect({"Sufi Tradition","Deep spiritual culture"});
        addAspect({"Kashmiri Poetry","Rich poetic and literary tradition"});
        addAspect({"Tourism Culture","Valley known as paradise on earth"});
        addAspect({"Folk Music","Traditional Kashmiri musical heritage"});

        addFood({"Wazwan","Traditional multi-course feast"});
        addFood({"Rogan Josh","Spiced lamb curry"});
        addFood({"Yakhni","Yogurt-based mutton dish"});
        addFood({"Dum Aloo","Spiced potato curry"});
        addFood({"Gushtaba","Minced meatballs in yogurt gravy"});
        addFood({"Kahwa","Traditional green tea with spices"});
        addFood({"Seekh Kabab","Grilled meat skewers"});
        addFood({"Modur Pulao","Sweet rice dish"});
        addFood({"Sheermal","Sweet flatbread"});
        addFood({"Haak","Leafy green vegetable dish"});
    }

    string translate(string e) override {
        if (e=="hello")     return "Adaab";
        if (e=="thank you") return "Shukriya";
        if (e=="love")      return "Mohabbat";
        if (e=="water")     return "Aab";
        if (e=="bread")     return "Tchot";
        if (e=="friend")    return "Dost";
        return "Translation not found";
    }
    CulturalTaboo getCulturalTaboo(string t) override {
        if (t=="elders")      return {"Respect elders","Avoid disrespect","Deep Kashmiri value"};
        if (t=="hospitality") return {"Never refuse kehwa","Hospitality","Guests are sacred"};
        if (t=="religion")    return {"Avoid irreverence in shrines","Faith respect","Strong taboo"};
        return {"Not found","N/A","N/A"};
    }
    Proverb getProverb(string t) override {
        if (t=="patience")    return {"Sabar chu gul","Patience","Patience is a flower"};
        if (t=="truth")       return {"Sach chu roshan","Truth","Truth is light"};
        if (t=="hospitality") return {"Mehman chu barkat","Hospitality","Guest brings blessings"};
        return {"Not found","N/A","N/A"};
    }
    string getGreeting(int f) override {
        switch(f) {
            case 0: return "Kya haal chu?";
            case 1: return "Adaab";
            case 2: return "Assalam o Alaikum";
            case 3: return "Khush Aamdeed";
            default: return "Adaab";
        }
    }
    string getDomainTerm(string e, string d) override {
        if (d=="food")     { if (e=="tea") return "Kehwa"; if (e=="bread") return "Tchot"; }
        if (d=="clothing") { if (e=="shawl") return "Pashmina"; if (e=="cloak") return "Phiran"; }
        if (d=="music")    { if (e=="song") return "Rouf"; }
        return "Term not found";
    }
};

// ============================================================
//  KASHMIRI LANGUAGE CLASS
// ============================================================
class Kashmiri : public Language {
private:
    map<string,string>            dictionary;
    map<string,vector<VocabWord>> vocabulary;
    map<int,vector<QuizQuestion>> quizBank;
    map<string,string>            pronunciationMap;
    void toLower(string& s) const { transform(s.begin(),s.end(),s.begin(),::tolower); }

    void runQuizSession(const vector<QuizQuestion>& questions, const string& label) {
        int correct=0,total=(int)questions.size();
        if (!total){ cout<<"No questions.\n"; return; }
        cout<<"\n====="<<label<<"=====\n";
        for (int i=0;i<total;i++){
            const QuizQuestion& q=questions[i];
            cout<<"\nQ"<<i+1<<": "<<q.question<<"\nA. "<<q.options[0]<<"\nB. "<<q.options[1]
                <<"\nC. "<<q.options[2]<<"\nD. "<<q.options[3]<<"\nAnswer: ";
            char ans=toupper(readChar()); int idx=ans-'A';
            if (idx==q.correctIndex){ cout<<"Correct!\n"; correct++; }
            else cout<<"Wrong! Correct: "<<(char)('A'+q.correctIndex)<<"\n";
        }
        int pts=correct*20;
        cout<<"\nScore: "<<correct<<"/"<<total<<"  Points: +"<<pts<<"\n";
        if (currentUser){ currentUser->addScore(pts); FileHandler::saveUser(*currentUser); }
    }

    void initDictionary() {
        dictionary["hello"]="Assalam o Alaikum"; dictionary["thank you"]="Shukriya";
        dictionary["yes"]="Hyun"; dictionary["no"]="Na"; dictionary["water"]="Pani";
        dictionary["food"]="Khanas"; dictionary["friend"]="Yaar"; dictionary["teacher"]="Ustad";
        dictionary["book"]="Kitab"; dictionary["house"]="Ghar"; dictionary["love"]="Pyar";
        dictionary["good"]="Changa"; dictionary["bad"]="Bura"; dictionary["go"]="Gachun";
        dictionary["come"]="Aanun"; dictionary["write"]="Likhan"; dictionary["read"]="Parhan";
        dictionary["big"]="Vado"; dictionary["small"]="Chhoto"; dictionary["school"]="School";
    }
    void initVocabulary() {
        vocabulary["greetings"]={
            VocabWord("Hello",       "Adaab","","","","","","","Kashmiri greeting"),
            VocabWord("Good Morning","Subah bakhair","","","","","","","Morning greeting"),
            VocabWord("Welcome",     "Khush Aamdeed","","","","","","","Welcoming guests"),
            VocabWord("Goodbye",     "Khuda Hafiz","","","","","","","Farewell"),
            VocabWord("Thank you",   "Shukriya","","","","","","","Gratitude")
        };
        vocabulary["numbers"]={
            VocabWord("One",  "Akh","","","","","","","Akh = 1"),
            VocabWord("Two",  "Zih","","","","","","","Zih = 2"),
            VocabWord("Three","Trey","","","","","","","Trey = 3"),
            VocabWord("Four", "Tsoor","","","","","","","Tsoor = 4"),
            VocabWord("Five", "Panch","","","","","","","Panch = 5")
        };
        vocabulary["colors"]={
            VocabWord("Red",  "Lool","","","","","","","Lool rang"),
            VocabWord("Blue", "Neel","","","","","","","Neel rang"),
            VocabWord("Green","Poz", "","","","","","","Poz rang"),
            VocabWord("White","Sfed","","","","","","","Sfed rang"),
            VocabWord("Black","Kaali","","","","","","","Kaali rang")
        };
        vocabulary["food"]={
            VocabWord("Bread","Tchot",   "","","","","","","Tchot khao"),
            VocabWord("Water","Aab",     "","","","","","","Aab piyo"),
            VocabWord("Meat", "Maaz",    "","","","","","","Maaz khaana"),
            VocabWord("Rice", "Bata",    "","","","","","","Bata pakao"),
            VocabWord("Tea",  "Kehwa",   "","","","","","","Kehwa piyo")
        };
        vocabulary["family"]={
            VocabWord("Mother","Mouj", "","","","","","","Mouj pyaari"),
            VocabWord("Father","Bab",  "","","","","","","Bab taaqatwar"),
            VocabWord("Brother","Bae", "","","","","","","Bae saath"),
            VocabWord("Sister","Zon",  "","","","","","","Zon meherbaan"),
            VocabWord("Child", "Kour", "","","","","","","Kour masoom")
        };
    }
    void initQuizBank() {
        // ---- Beginner (5) ----
        { string o[4]={"Shukriya","Salam","Kitab","Ghar"};
          quizBank[1].push_back(QuizQuestion("Thank you in Kashmiri?",o,0,"Shukriya means thanks")); }
        { string o[4]={"Aab","Aag","Hawa","Zameen"};
          quizBank[1].push_back(QuizQuestion("Water in Kashmiri?",o,0,"Aab = water in Kashmiri")); }
        { string o[4]={"Yaar","Dushman","Ghar","Kitab"};
          quizBank[1].push_back(QuizQuestion("Friend in Kashmiri?",o,0,"Yaar = friend")); }
        { string o[4]={"Mouj","Bab","Bae","Kour"};
          quizBank[1].push_back(QuizQuestion("Mother in Kashmiri?",o,0,"Mouj = Mother in Kashmiri")); }
        { string o[4]={"Adaab","Shukriya","Khuda Hafiz","Hyun"};
          quizBank[1].push_back(QuizQuestion("Hello/Greeting in Kashmiri?",o,0,"Adaab = respectful greeting")); }
        // ---- Intermediate (5) ----
        { string o[4]={"Wazwan","Biryani","Pulao","Karahi"};
          quizBank[2].push_back(QuizQuestion("Famous Kashmiri multi-course feast?",o,0,"Wazwan is the traditional feast.")); }
        { string o[4]={"Kehwa","Chai","Lassi","Sherbet"};
          quizBank[2].push_back(QuizQuestion("Traditional Kashmiri spiced green tea?",o,0,"Kehwa is the traditional tea.")); }
        { string o[4]={"Pashmina","Ajrak","Phulkari","Khaddar"};
          quizBank[2].push_back(QuizQuestion("World-famous fine wool shawl from Kashmir?",o,0,"Pashmina shawl.")); }
        { string o[4]={"Pheran","Shalwar","Kameez","Lungi"};
          quizBank[2].push_back(QuizQuestion("Traditional Kashmiri long woolen dress?",o,0,"Pheran is the traditional Kashmiri garment.")); }
        { string o[4]={"Saffron","Tulip","Rose","Jasmine"};
          quizBank[2].push_back(QuizQuestion("Kashmir is famous for which precious spice?",o,0,"Kashmiri saffron is world-famous.")); }
        // ---- Advanced (5) ----
        { string o[4]={"Dal Lake","Wular Lake","Nagin Lake","Mansarovar"};
          quizBank[3].push_back(QuizQuestion("Famous lake where Shikara rides are taken?",o,0,"Dal Lake in Srinagar.")); }
        { string o[4]={"Dardic","Turkic","Iranian","Dravidian"};
          quizBank[3].push_back(QuizQuestion("Kashmiri belongs to which sub-group of Indo-Aryan?",o,0,"Kashmiri is a Dardic language.")); }
        { string o[4]={"Lal Ded","Habba Khatoon","Arnimaal","Mahjoor"};
          quizBank[3].push_back(QuizQuestion("14th century Kashmiri mystic poetess?",o,0,"Lal Ded (Lalleshwari) is the legendary poetess.")); }
        { string o[4]={"Rogan Josh","Wazwan","Yakhni","Gushtaba"};
          quizBank[3].push_back(QuizQuestion("Kashmiri meatball dish cooked in yogurt gravy?",o,3,"Gushtaba = meatballs in yogurt.")); }
        { string o[4]={"Sufiana Kalam","Rouf","Chakri","Hafiz Nagma"};
          quizBank[3].push_back(QuizQuestion("Traditional Kashmiri dance form performed by women?",o,1,"Rouf is performed by Kashmiri women.")); }
    }
    void initPronunciation() {
        pronunciationMap["Adaab"]   = "A-daab (respectful, long aa in middle)";
        pronunciationMap["Shukriya"]= "Shuk-ri-ya (stress on first syllable)";
        pronunciationMap["Kehwa"]   = "Keh-wa (keh from back of throat, soft wa)";
        pronunciationMap["Wazwan"]  = "Waz-wan (waz rhymes with 'jazz', short wan)";
        pronunciationMap["Pashmina"]= "Pash-mi-na (3 syllables, stress on PASH)";
        pronunciationMap["Pheran"]  = "Phe-ran (light f + he, roll r slightly)";
        pronunciationMap["Tchot"]   = "Tchot (tch like 'ch' in 'church' + ot)";
        pronunciationMap["Aab"]     = "Aab (long aa + b, like 'arm' + b)";
        pronunciationMap["Mouj"]    = "Mouj (rhymes with 'rouge', short uj)";
        pronunciationMap["Hyun"]    = "Hyun (like 'hyoon', Kashmiri yes)";
    }

public:
    Kashmiri() : Language("Kashmiri") {
        initDictionary(); initVocabulary(); initQuizBank(); initPronunciation();
    }
    void translate(const string& word) override {
        if (!requireLogin()) return;
        string key=word; toLower(key);
        cout<<"\n--- Kashmiri Translation ---\n";
        if (dictionary.count(key)) cout<<word<<" = "<<dictionary[key]<<"\n";
        else cout<<"Word not found.\nTry: hello, water, friend, love, teacher...\n";
    }
    void runQuiz(int level) override {
        if (!requireLogin()) return;
        string names[]={"","Beginner","Intermediate","Advanced"};
        if (level<1||level>3){cout<<"Invalid level.\n";return;}
        if (!quizBank.count(level)||quizBank[level].empty()){cout<<"No quiz available.\n";return;}
        runQuizSession(quizBank[level],"Kashmiri "+names[level]+" Quiz");
    }
    void startGame(int choice) override {
        if (!requireLogin()) return;
        vector<pair<string,string>> hangmanWords={
            {"aab","water"},{"ghar","home"},{"kitab","book"},
            {"yaar","friend"},{"tchot","bread"},{"mouj","mother"},
            {"bae","brother"},{"ustad","teacher"},{"kehwa","green tea"},{"maaz","meat"}};
        vector<pair<string,string>> matchPairs={
            {"hello","Adaab"},{"water","Aab"},{"bread","Tchot"},
            {"friend","Yaar"},{"teacher","Ustad"},{"book","Kitab"},
            {"thank you","Shukriya"},{"house","Ghar"},{"mother","Mouj"},{"yes","Hyun"}};
        if (choice==1){
            srand((unsigned int)time(0));
            int idx=rand()%(int)hangmanWords.size();
            string target=hangmanWords[idx].first,hint=hangmanWords[idx].second;
            string display(target.size(),'_'); set<char> tried; int wrong=0,maxWrong=6;
            cout<<"\n--- HANGMAN (Kashmiri) ---\nHint: "<<hint<<"  | Length: "<<target.size()<<"\n\n";
            while(wrong<maxWrong&&display!=target){
                cout<<"Word  : "; for(char c:display)cout<<c<<" ";
                cout<<"\nTried : "; for(char c:tried)cout<<c<<" ";
                cout<<"\nGuess a letter: "; char g=tolower(readChar());
                if(tried.count(g)){cout<<"Already tried!\n\n";continue;}
                tried.insert(g); bool hit=false;
                for(int i=0;i<(int)target.size();i++) if(target[i]==g){display[i]=g;hit=true;}
                if(hit)cout<<"Good guess!\n\n"; else{wrong++;cout<<"Wrong! ("<<wrong<<"/"<<maxWrong<<")\n\n";}
            }
            if(display==target){cout<<"CORRECT! Word: "<<target<<"\n";int earned=(maxWrong-wrong)*10;
                cout<<"Points: "<<earned<<"\n";
                if(currentUser){currentUser->addScore(earned);FileHandler::saveUser(*currentUser);}}
            else cout<<"Game over! Word was: "<<target<<"\n";
        } else if (choice==2){
            srand((unsigned int)time(0)); vector<pair<string,string>> sh=matchPairs;
            for(int i=(int)sh.size()-1;i>0;i--) swap(sh[i],sh[rand()%(i+1)]);
            int numQ=min(5,(int)sh.size()),score=0;
            cout<<"\n--- WORD MATCH (Kashmiri) ---\n";
            for(int i=0;i<numQ;i++){
                string correct=sh[i].second; vector<string> opts; opts.push_back(correct);
                vector<int> used; used.push_back(i);
                while((int)opts.size()<4){int r=rand()%(int)sh.size();bool dup=false;
                    for(int u:used) if(u==r){dup=true;break;}
                    if(!dup){opts.push_back(sh[r].second);used.push_back(r);}}
                for(int k=(int)opts.size()-1;k>0;k--) swap(opts[k],opts[rand()%(k+1)]);
                int ci=0; for(int k=0;k<4;k++) if(opts[k]==correct)ci=k;
                cout<<"Q"<<i+1<<" - \""<<sh[i].first<<"\"\n";
                for(int k=0;k<4;k++) cout<<"  "<<k+1<<". "<<opts[k]<<"\n";
                cout<<"Answer (1-4): "; int ch=readInt();
                if(ch-1==ci){cout<<"Correct!\n\n";score+=10;} else cout<<"Wrong! Answer: "<<correct<<"\n\n";
            }
            cout<<"Score: "<<score<<"/"<<numQ*10<<"\n";
            if(currentUser){currentUser->addScore(score);FileHandler::saveUser(*currentUser);}
        } else cout<<"Invalid game choice.\n";
    }
    void showProgress() override { if (!requireLogin()) return; currentUser->displayProfile(); }
    void showLeaderboard() override {
        auto e=FileHandler::loadLeaderboard(); cout<<"\n"; line(50,'=');
        cout<<"             GLOBAL LEADERBOARD\n"; line(50,'=');
        cout<<left<<setw(5)<<"Rank"<<setw(18)<<"Username"<<setw(10)<<"Score"<<"Streak\n"; line(50,'-');
        if(e.empty())cout<<"  No entries yet.\n";
        for(int i=0;i<(int)e.size()&&i<10;i++)
            cout<<left<<setw(5)<<i+1<<setw(18)<<e[i].username<<setw(10)<<e[i].score<<e[i].streak<<" days\n";
        line(50,'=');
    }
    void loadModule(int level) override {
        if (!requireLogin()) return;
        map<int,vector<string>> cats;
        cats[1]={"greetings","numbers"}; cats[2]={"colors","food"}; cats[3]={"family"};
        if (!cats.count(level)){cout<<"Invalid level.\n";return;}
        string names[]={"","Beginner","Intermediate","Advanced"};
        cout<<"\n"; line(50,'='); cout<<"  KASHMIRI VOCABULARY -- "<<names[level]<<"\n"; line(50,'=');
        for (const string& cat:cats[level]){
            cout<<"\n  ["<<cat<<"]\n"; cout<<left<<setw(14)<<"English"<<setw(20)<<"Kashmiri"<<"Example\n"; line(60,'-');
            if (!vocabulary.count(cat)) continue;
            for (const auto& w:vocabulary.at(cat)){
                cout<<left<<setw(14)<<w.english<<setw(20)<<w.punjabi<<w.example<<"\n";
                currentUser->addMasteredWord(w.english);
            }
        }
        currentUser->awardBadge("First Steps"); FileHandler::saveUser(*currentUser);
    }
    void dailyChallenge() override {
        if (!requireLogin()) return;
        cout<<"\n=== DAILY KASHMIRI CHALLENGE -- "<<User::getCurrentDate()<<" ===\n";
        srand((unsigned int)time(0)); auto& pool=quizBank[1];
        if(pool.empty()){cout<<"No questions.\n";return;}
        vector<QuizQuestion> daily; set<int> picked;
        for(int att=0;(int)daily.size()<3&&att<100;att++){int r=rand()%(int)pool.size();
            if(!picked.count(r)){daily.push_back(pool[r]);picked.insert(r);}}
        runQuizSession(daily,"Daily Kashmiri Challenge");
        currentUser->updateStreak(); FileHandler::saveUser(*currentUser);
        cout<<"Streak: "<<currentUser->getStreak()<<" days\n";
    }
    void pronunciationGuide(const string& word) override {
        if (!requireLogin()) return;
        cout<<"\n--- KASHMIRI PRONUNCIATION: "<<word<<" ---\n";
        if (pronunciationMap.count(word)){
            cout<<pronunciationMap.at(word)<<"\n";
            cout<<"\n--- General Kashmiri Rules ---\n";
            cout<<"  1. Kashmiri is a Dardic language; some sounds are unique\n";
            cout<<"  2. ts and tsh sounds are common (like 'ch' in church)\n";
            cout<<"  3. Nasal vowels (like French) appear in Kashmiri\n";
        } else {
            cout<<"No guide for '"<<word<<"'.\n";
            cout<<"Available: Adaab, Shukriya, Kehwa, Wazwan, Pashmina, Pheran, Tchot, Aab, Mouj, Hyun\n";
        }
    }
    void revisionQuiz() override {
        if (!requireLogin()) return;
        auto weak=currentUser->getWeakAreas(); cout<<"\n=== KASHMIRI REVISION QUIZ ===\n";
        if(weak.empty()){cout<<"No weak areas! General refresher.\n\n";runQuizSession(quizBank[1],"Kashmiri Revision");return;}
        vector<QuizQuestion> rev;
        for(auto& kv:quizBank) for(const auto& q:kv.second) rev.push_back(q);
        if((int)rev.size()>6) rev.resize(6);
        runQuizSession(rev,"Kashmiri Revision Quiz");
    }
    void showVocabulary(const string& cat) override {
        if (!requireLogin()) return;
        if (!vocabulary.count(cat)){cout<<"Category not found.\nAvailable: greetings, numbers, colors, food, family\n";return;}
        cout<<"\n--- KASHMIRI VOCABULARY: "<<cat<<" ---\n";
        cout<<left<<setw(14)<<"English"<<setw(20)<<"Kashmiri"<<"Example\n"; line(60,'-');
        for (const auto& w:vocabulary.at(cat))
            cout<<left<<setw(14)<<w.english<<setw(20)<<w.punjabi<<w.example<<"\n";
    }
};

// ============================================================
//  GILGITI CULTURE  (overrides INSIDE the class)
// ============================================================
class GilgitiCulture : public Culture {
public:
    GilgitiCulture() : Culture("Gilgiti", "Gilgit Baltistan", 90) {
        addPhrase({"Assalam o Alaikum","Peace greeting"});
        addPhrase({"Kya haal a?","How are you?"});
        addPhrase({"Shukriya","Thank you"});
        addPhrase({"Khuda Hafiz","Goodbye"});
        addPhrase({"Maaf karo","Sorry"});
        addPhrase({"Haan","Yes"});
        addPhrase({"Nahi","No"});
        addPhrase({"Tumhara naam kya hai?","What is your name?"});
        addPhrase({"Subah bakhair","Good Morning"});
        addPhrase({"Sham bakhair","Good Evening"});
        addPhrase({"Pani","Water"});
        addPhrase({"Khana","Food"});
        addPhrase({"Dost","Friend"});
        addPhrase({"Ustaad","Teacher"});
        addPhrase({"Ghar","House"});
        addPhrase({"Kitab","Book"});
        addPhrase({"School","School"});
        addPhrase({"Hospital","Hospital"});
        addPhrase({"Main khush hoon","I am happy"});
        addPhrase({"Bohat shukriya","Thank you very much"});

        addProverb({"Sabar ka phal meetha hota hai","Patience brings reward","Patience"});
        addProverb({"Ilm roshni hai","Knowledge is light","Education"});
        addProverb({"Mehnat kamyabi deti hai","Hard work brings success","Hard Work"});
        addProverb({"Sach hamesha jeetta hai","Truth always wins","Truth"});
        addProverb({"Dost mushkil mein pehchana jata hai","Friend known in hardship","Friendship"});
        addProverb({"Waqt kisi ka intezar nahi karta","Time waits for none","Time"});
        addProverb({"Achai kabhi zaya nahi hoti","Good deeds never wasted","Goodness"});
        addProverb({"Zaban ka zakham gehra hota hai","Words hurt deeply","Speech"});
        addProverb({"Mohabbat sab kuch jeet leti hai","Love conquers all","Love"});
        addProverb({"Takabbur girata hai","Pride leads to downfall","Humility"});
        addProverb({"Biradarī mein taqat hai","Strength lies in brotherhood","Unity"});
        addProverb({"Sabar se kamyabi milti hai","Patience brings success","Patience"});
        addProverb({"Pahadon ki tarah mazboot raho","Be strong like the mountains","Strength"});
        addProverb({"Pani paharon se bhi rasta banata hai","Water finds its way through mountains","Perseverance"});
        addProverb({"Khubsurat wadi, khubsurat dil","Beautiful valley, beautiful heart","Nature"});
        addProverb({"Hunza ka mewa duniya ka sab se meetha","Hunza's fruit is the world's sweetest","Pride"});
        addProverb({"Choti galiyan bade log","Small alleys produce great people","Humility"});
        addProverb({"Buzurg ka tajruba khazana hai","Elder's experience is a treasure","Wisdom"});
        addProverb({"Dost woh jo door se bhi yaad aaye","True friend is remembered even from afar","Friendship"});
        addProverb({"Roshan dil roshan zindagi","Bright heart, bright life","Positivity"});

        addTaboo({"Buzurgon ki beizzati na karo","Respect elders","Respect"});
        addTaboo({"Jhoot na bolo","Do not lie","Truth"});
        addTaboo({"Mehmaan ko ignore na karo","Respect guests","Hospitality"});
        addTaboo({"Zyada gussa na karo","Avoid anger","Calmness"});
        addTaboo({"Masjid mein shor na karo","No noise in mosque","Respect"});
        addTaboo({"Ghar ki baat bahar na le jao","Keep family matters private","Privacy"});
        addTaboo({"Zulm na karo","Do not oppress","Justice"});
        addTaboo({"Takabbur na karo","Avoid arrogance","Humility"});
        addTaboo({"Khane ko zaya na karo","Do not waste food","Gratitude"});
        addTaboo({"Bachon se pyaar karo","Be kind to children","Kindness"});

        addTerm({"Doctor","Doctor","MEDICAL"}); addTerm({"Hospital","Hospital","MEDICAL"});
        addTerm({"Fever","Bukhar","MEDICAL"}); addTerm({"Court","Adalat","LEGAL"});
        addTerm({"Law","Qanoon","LEGAL"}); addTerm({"Judge","Jaj","LEGAL"});
        addTerm({"School","School","EDUCATIONAL"}); addTerm({"Teacher","Ustaad","EDUCATIONAL"});
        addTerm({"Student","Talib e Ilm","EDUCATIONAL"}); addTerm({"Book","Kitab","EDUCATIONAL"});

        addAspect({"Gilgit Baltistan Culture","Mountain culture with rich traditions"});
        addAspect({"Hunza Valley","Beautiful valley known for longevity and peace"});
        addAspect({"Traditional Dress","Warm woolen clothes for cold climate"});
        addAspect({"Mountain Life","Life in high-altitude regions"});
        addAspect({"Local Music","Traditional instruments and folk songs"});
        addAspect({"Fruit Culture","Famous for apricots, apples, cherries"});
        addAspect({"Tourism","Popular destination for natural beauty"});
        addAspect({"Hospitality","Very welcoming and peaceful culture"});
        addAspect({"Handicrafts","Woodwork and handmade crafts"});
        addAspect({"Ancient History","Buddhist and Silk Route historical influence"});

        addFood({"Chapshuro","Stuffed meat bread"});
        addFood({"Diram Fitti","Traditional dry bread"});
        addFood({"Mamtu","Steamed dumplings"});
        addFood({"Balay","Noodle soup dish"});
        addFood({"Harissa","Wheat and meat dish"});
        addFood({"Apricot Oil","Special local oil product"});
        addFood({"Gyal","Traditional bread"});
        addFood({"Butter Tea","Salted tea with butter"});
        addFood({"Chai","Tea"});
        addFood({"Fresh Fruits","Apples, apricots, cherries"});
    }

    string translate(string e) override {
        if (e=="hello")     return "Salam";
        if (e=="thank you") return "Shukriya";
        if (e=="friend")    return "Dost";
        if (e=="water")     return "Aab";
        if (e=="love")      return "Mohabbat";
        return "Translation not found";
    }
    CulturalTaboo getCulturalTaboo(string t) override {
        if (t=="hospitality") return {"Guests are sacred","Hospitality","Strong Gilgit tradition"};
        if (t=="elders")      return {"Respect elders","Avoid disrespect","Core cultural value"};
        if (t=="religion")    return {"Avoid irreverence in mosques","Faith respect","Deep taboo"};
        return {"Not found","N/A","N/A"};
    }
    Proverb getProverb(string t) override {
        if (t=="unity")     return {"Biradarī mein taqat hai","Unity","Strength in brotherhood"};
        if (t=="patience")  return {"Sabar se kamyabi milti hai","Patience","Patience brings success"};
        if (t=="strength")  return {"Pahadon ki tarah mazboot raho","Strength","Be strong like mountains"};
        return {"Not found","N/A","N/A"};
    }
    string getGreeting(int f) override {
        switch(f) {
            case 0: return "Salam dost!";
            case 1: return "Assalam o Alaikum";
            case 2: return "Adaab";
            case 3: return "Khush Aamdeed";
            default: return "Assalam o Alaikum";
        }
    }
    string getDomainTerm(string e, string d) override {
        if (d=="food")     { if (e=="bread") return "Chapshuro"; if (e=="tea") return "Namkeen Chai"; }
        if (d=="clothing") { if (e=="cap") return "Gilgiti Topi"; if (e=="shawl") return "Pattu Shawl"; }
        if (d=="music")    { if (e=="instrument") return "Dumak"; }
        return "Term not found";
    }
};

// ============================================================
//  GILGITI LANGUAGE CLASS
// ============================================================
class Gilgiti : public Language {
private:
    map<string,string>            dictionary;
    map<string,vector<VocabWord>> vocabulary;
    map<int,vector<QuizQuestion>> quizBank;
    map<string,string>            pronunciationMap;
    void toLower(string& s) const { transform(s.begin(),s.end(),s.begin(),::tolower); }

    void runQuizSession(const vector<QuizQuestion>& questions, const string& label) {
        int correct=0,total=(int)questions.size();
        if (!total){ cout<<"No questions.\n"; return; }
        cout<<"\n====="<<label<<"=====\n";
        for (int i=0;i<total;i++){
            const QuizQuestion& q=questions[i];
            cout<<"\nQ"<<i+1<<": "<<q.question<<"\nA. "<<q.options[0]<<"\nB. "<<q.options[1]
                <<"\nC. "<<q.options[2]<<"\nD. "<<q.options[3]<<"\nAnswer: ";
            char ans=toupper(readChar()); int idx=ans-'A';
            if (idx==q.correctIndex){ cout<<"Correct!\n"; correct++; }
            else cout<<"Wrong! Correct: "<<(char)('A'+q.correctIndex)<<"\n";
        }
        int pts=correct*20;
        cout<<"\nScore: "<<correct<<"/"<<total<<"  Points: +"<<pts<<"\n";
        if (currentUser){ currentUser->addScore(pts); FileHandler::saveUser(*currentUser); }
    }

    void initDictionary() {
        dictionary["hello"]="Assalam o Alaikum"; dictionary["thank you"]="Shukriya";
        dictionary["yes"]="Haan"; dictionary["no"]="Nahi"; dictionary["water"]="Pani";
        dictionary["food"]="Khana"; dictionary["friend"]="Dost"; dictionary["teacher"]="Ustaad";
        dictionary["book"]="Kitab"; dictionary["house"]="Ghar"; dictionary["love"]="Mohabbat";
        dictionary["good"]="Acha"; dictionary["bad"]="Bura"; dictionary["go"]="Jana";
        dictionary["come"]="Aana"; dictionary["write"]="Likhna"; dictionary["read"]="Parhna";
        dictionary["big"]="Bara"; dictionary["small"]="Chota"; dictionary["school"]="School";
    }
    void initVocabulary() {
        vocabulary["greetings"]={VocabWord("Hello","Assalam o Alaikum","","","","","","","Greeting"),
                                  VocabWord("Good Morning","Subah bakhair","","","","","","","Morning")};
    }
    void initQuizBank() {
        { string o[4]={"Shukriya","Salam","Kitab","Ghar"};
          quizBank[1].push_back(QuizQuestion("Thank you in Gilgiti?",o,0,"Shukriya means thanks")); }
        { string o[4]={"Pani","Aag","Hawa","Zameen"};
          quizBank[1].push_back(QuizQuestion("Water in Gilgiti?",o,0,"Pani = water")); }
        // Intermediate
        { string o[4]={"Chapshuro","Diram Fitti","Mamtu","Balay"};
          quizBank[2].push_back(QuizQuestion("Gilgiti stuffed meat bread?",o,0,"Chapshuro is the stuffed meat bread.")); }
        { string o[4]={"Hunza","Gilgit","Skardu","Chitral"};
          quizBank[2].push_back(QuizQuestion("Valley famous for longevity in GB?",o,0,"Hunza Valley.")); }
        // Advanced
        { string o[4]={"K2","Nanga Parbat","Rakaposhi","Gasherbrum"};
          quizBank[3].push_back(QuizQuestion("Second highest mountain in the world in GB?",o,0,"K2 is in Gilgit Baltistan.")); }
        { string o[4]={"Dumak","Surna","Sitar","Rubab"};
          quizBank[3].push_back(QuizQuestion("Traditional drum of Gilgit Baltistan?",o,0,"Dumak is the traditional drum.")); }
    }
    void initPronunciation() {
        pronunciationMap["Assalam o Alaikum"]="As-sa-lam o a-lai-kum";
        pronunciationMap["Shukriya"]="Shuk-ri-ya";
    }

public:
    Gilgiti() : Language("Gilgiti") {
        initDictionary(); initVocabulary(); initQuizBank(); initPronunciation();
    }
    void translate(const string& word) override {
        if (!requireLogin()) return;
        string key=word; toLower(key);
        cout<<"\n--- Gilgiti Translation ---\n";
        if (dictionary.count(key)) cout<<word<<" = "<<dictionary[key]<<"\n";
        else cout<<"Word not found.\n";
    }
    void runQuiz(int level) override {
        if (!requireLogin()) return;
        if (!quizBank.count(level)){ cout<<"No quiz available.\n"; return; }
        runQuizSession(quizBank[level],"Gilgiti Quiz Level "+to_string(level));
    }
    void startGame(int c) override { if (!requireLogin()) return; cout<<"Games not added for Gilgiti yet.\n"; }
    void showProgress() override { if (!requireLogin()) return; currentUser->displayProfile(); }
    void showLeaderboard() override {
        auto e=FileHandler::loadLeaderboard();
        cout<<"\n--- Gilgiti Leaderboard ---\n";
        for (int i=0;i<(int)e.size()&&i<10;i++) cout<<i+1<<". "<<e[i].username<<" - "<<e[i].score<<"\n";
    }
    void loadModule(int l) override {
        if (!requireLogin()) return;
        cout<<"\n--- Gilgiti Vocabulary ---\n";
        for (auto& p:vocabulary) for (auto& w:p.second) cout<<w.english<<" -> "<<w.punjabi<<"\n";
    }
    void dailyChallenge() override {
        if (!requireLogin()) return;
        runQuizSession(quizBank[1],"Daily Gilgiti Challenge");
        currentUser->updateStreak(); FileHandler::saveUser(*currentUser);
    }
    void pronunciationGuide(const string& word) override {
        if (!requireLogin()) return;
        if (pronunciationMap.count(word)) cout<<pronunciationMap[word]<<"\n";
        else cout<<"No guide found.\n";
    }
    void revisionQuiz() override { if (!requireLogin()) return; runQuiz(1); }
    void showVocabulary(const string& cat) override {
        if (!requireLogin()) return;
        if (!vocabulary.count(cat)){ cout<<"Category not found.\n"; return; }
        for (auto& w:vocabulary[cat]) cout<<w.english<<" - "<<w.punjabi<<"\n";
    }
};

// ============================================================
//  AUTH SYSTEM
// ============================================================
class AuthSystem {
public:
    // Sign-up now also collects email
    static bool signup(User& outUser) {
        cout << "\n";
        line(40,'=');
        cout << "          CREATE YOUR ACCOUNT\n";
        line(40,'=');
        cout << "Username  : ";
        string u = readLine();
        if (u.empty()) { cout << "Username cannot be empty.\n"; return false; }
        if (FileHandler::userExists(u)) { cout << "Username already taken.\n"; return false; }

        cout << "Email     : ";
        string em = readLine();
        // Basic email validation
        if (em.find('@') == string::npos || em.find('.') == string::npos) {
            cout << "Please enter a valid email address.\n"; return false;
        }

        cout << "Password  : ";
        string p = readLine();
        cout << "Confirm   : ";
        string p2 = readLine();
        if (p.empty())  { cout << "Password cannot be empty.\n"; return false; }
        if (p != p2)    { cout << "Passwords do not match.\n";   return false; }

        outUser = User(u, p, em);
        FileHandler::saveUser(outUser);
        FileHandler::updateLeaderboard(outUser);
        cout << "\nAccount created! Welcome, " << u << "!\n";
        cout << "Email saved: " << em << "\n";
        return true;
    }

    static bool login(User& outUser) {
        cout << "\n";
        line(40,'=');
        cout << "               LOGIN\n";
        line(40,'=');
        cout << "Username: "; string u = readLine();
        cout << "Password: "; string p = readLine();
        if (!FileHandler::validateLogin(u, p)) {
            cout << "Wrong username or password.\n"; return false;
        }
        outUser = FileHandler::loadUser(u);
        outUser.updateStreak();
        FileHandler::saveUser(outUser);
        cout << "\nWelcome back, " << u << "!\n";
        cout << "Email on file: " << outUser.getEmail() << "\n";
        cout << "Current streak: " << outUser.getStreak() << " days\n";

        // Show any newly unlocked streak badges
        auto badges = outUser.getBadges();
        for (const auto& b : badges)
            if (b.earned && b.name.find("Streak") != string::npos)
                cout << "  [BADGE] " << b.name << " - " << b.description << "\n";
        return true;
    }
};

// ============================================================
//  MAIN FUNCTION
// ============================================================
int main() {
    cout << "\n";
    line(54, '=');
    cout << "     PAKISTAN CULTURE LINGUISTIC SIMULATOR\n";
    cout << "     Learn Languages | Explore Cultures\n";
    line(54, '=');

    // ---- All language/culture objects ----
    Punjabi   punjabi;
    Urdu      urdu;
    Sindhi    sindhi;
    Balochi   balochi;
    Pashto    pashto;
    Kashmiri  kashmiri;
    Gilgiti   gilgiti;

    // ---- All culture objects ----
    PunjabiCulture  punjabiCulture;
    UrduCulture     urduCulture;
    SindhiCulture   sindhiCulture;
    BalochiCulture  balochiCulture;
    PashtoCulture   pashtoCulture;
    KashmiriCulture kashmiriCulture;
    GilgitiCulture  gilgitiCulture;

    // ---- Language pointers array (for easy dispatch) ----
    Language* langs[7] = {&punjabi, &urdu, &sindhi, &balochi, &pashto, &kashmiri, &gilgiti};
    Culture*  cults[7] = {&punjabiCulture, &urduCulture, &sindhiCulture,
                           &balochiCulture, &pashtoCulture, &kashmiriCulture, &gilgitiCulture};
    string langNames[7] = {"Punjabi","Urdu","Sindhi","Balochi","Pashto","Kashmiri","Gilgiti"};

    User currentUser;
    bool loggedIn = false;

    // ---- MANDATORY Sign-Up / Login Gate ----
    cout << "\nYou must create an account or log in to access the system.\n";
    while (!loggedIn) {
        cout << "\n  1. Sign Up (new user)\n"
             << "  2. Login   (existing user)\n"
             << "  3. Exit\n"
             << "Choice: ";
        int c = readInt();
        if      (c == 1) loggedIn = AuthSystem::signup(currentUser);
        else if (c == 2) loggedIn = AuthSystem::login(currentUser);
        else if (c == 3) { cout << "Goodbye!\n"; return 0; }
        else             cout << "Invalid choice.\n";
    }

    // Connect logged-in user to all language objects
    for (int i = 0; i < 7; i++) langs[i]->setCurrentUser(&currentUser);

    // ---- MAIN APPLICATION LOOP ----
    int mainChoice = -1;
    while (mainChoice != 0) {

        cout << "\n"; line(54,'=');
        cout << "         MAIN MENU  -- " << currentUser.getUsername()
             << " [" << currentUser.getLevelName() << " | Score: "
             << currentUser.getTotalScore() << " | Streak: "
             << currentUser.getStreak() << " days]\n";
        line(54,'=');
        cout << "   1.  Learn a Language\n";
        cout << "   2.  Explore a Culture\n";
        cout << "   3.  Mini Games\n";
        cout << "   4.  Global Leaderboard\n";
        cout << "   5.  My Profile & Progress\n";
        cout << "   6.  Daily Challenge\n";
        cout << "   7.  Add Custom Culture Aspect\n";
        cout << "   8.  View Custom Culture Aspects\n";
        cout << "   0.  Save & Exit\n";
        line(54,'=');
        cout << "Choice: ";
        mainChoice = readInt();

        if (mainChoice == 1) {
            // ---- Language Selection ----
            cout << "\n--- CHOOSE A LANGUAGE ---\n";
            for (int i = 0; i < 7; i++)
                cout << "  " << (i+1) << ". " << langNames[i] << "\n";
            cout << "  0. Back\nChoice: ";
            int lc = readInt();
            if (lc < 1 || lc > 7) { if (lc != 0) cout << "Invalid.\n"; continue; }
            Language* lang = langs[lc-1];

            // Language sub-menu
            int langChoice = -1;
            while (langChoice != 0) {
                cout << "\n--- " << langNames[lc-1] << " MENU ---\n";
                cout << "  1. Translate a word\n";
                cout << "  2. Vocabulary Module (Beginner)\n";
                cout << "  3. Vocabulary Module (Intermediate)\n";
                cout << "  4. Vocabulary Module (Advanced)\n";
                cout << "  5. Quiz - Beginner\n";
                cout << "  6. Quiz - Intermediate\n";
                cout << "  7. Quiz - Advanced\n";
                cout << "  8. Daily Challenge\n";
                cout << "  9. Revision Quiz\n";
                cout << " 10. Pronunciation Guide\n";
                cout << "  0. Back\nChoice: ";
                langChoice = readInt();

                switch (langChoice) {
                    case 1: { cout << "Enter word: "; string w=readLine(); lang->translate(w); break; }
                    case 2: lang->loadModule(1); break;
                    case 3: lang->loadModule(2); break;
                    case 4: lang->loadModule(3); break;
                    case 5: lang->runQuiz(1); break;
                    case 6: lang->runQuiz(2); break;
                    case 7: lang->runQuiz(3); break;
                    case 8: lang->dailyChallenge(); break;
                    case 9: lang->revisionQuiz(); break;
                    case 10: { cout << "Enter word for pronunciation guide: "; string w=readLine(); lang->pronunciationGuide(w); break; }
                    case 0: break;
                    default: cout << "Invalid choice.\n";
                }
                if (langChoice != 0) pause();
            }
        }

        else if (mainChoice == 2) {
            // ---- Culture Explorer ----
            cout << "\n--- CHOOSE A CULTURE ---\n";
            for (int i=0;i<7;i++) cout << "  " << (i+1) << ". " << langNames[i] << "\n";
            cout << "  0. Back\nChoice: ";
            int cc = readInt();
            if (cc < 1 || cc > 7) { if (cc != 0) cout << "Invalid.\n"; continue; }
            Culture* cult = cults[cc-1];

            cout << "\n--- " << langNames[cc-1] << " CULTURE ---\n";
            cout << "  1. Display all culture information\n";
            cout << "  2. Look up a proverb by topic\n";
            cout << "  3. Look up a taboo by topic\n";
            cout << "  4. Translate a phrase\n";
            cout << "  5. Get greeting (formality level)\n";
            cout << "  0. Back\nChoice: ";
            int cultChoice = readInt();

            switch (cultChoice) {
                case 1: cult->displayAll(); break;
                case 2: { cout << "Topic (e.g. patience, truth, love): "; string t=readLine();
                          Proverb p = cult->getProverb(t);
                          cout << "\"" << p.text << "\"\n  Meaning: " << p.meaning << "\n"; break; }
                case 3: { cout << "Topic (e.g. elders, hospitality): "; string t=readLine();
                          CulturalTaboo tab = cult->getCulturalTaboo(t);
                          cout << "Rule: " << tab.rule << "\n  Reason: " << tab.reason
                               << "\n  Alternative: " << tab.alternative << "\n"; break; }
                case 4: { cout << "English phrase: "; string e=readLine();
                          cout << "Translation: " << cult->translate(e) << "\n"; break; }
                case 5: { cout << "Formality (0=casual, 1=normal, 2=formal, 3=very formal): ";
                          int f = readInt();
                          cout << "Greeting: " << cult->getGreeting(f) << "\n"; break; }
                case 0: break;
                default: cout << "Invalid choice.\n";
            }
            pause();
        }

        else if (mainChoice == 3) {
            // ---- Mini Games ----
            cout << "\n--- MINI GAMES ---\n";
            cout << "  Choose a language:\n";
            for (int i = 0; i < 7; i++) cout << "  " << (i+1) << ". " << langNames[i] << "\n";
            cout << "  0. Back\nChoice: ";
            int lc = readInt();
            if (lc < 1 || lc > 7) { if (lc != 0) cout << "Invalid.\n"; continue; }
            cout << "\n  1. Hangman    (" << langNames[lc-1] << " words)\n";
            cout << "  2. Word Match (" << langNames[lc-1] << " English matching)\n";
            cout << "  0. Back\nChoice: ";
            int gc = readInt();
            if (gc == 1 || gc == 2) {
                langs[lc-1]->startGame(gc);
            } else if (gc != 0) {
                cout << "Invalid choice.\n";
            }
            pause();
        }

        else if (mainChoice == 4) {
            // ---- Leaderboard ----
            punjabi.showLeaderboard();
            pause();
        }

        else if (mainChoice == 5) {
            // ---- Profile & Progress ----
            currentUser.displayProfile();
            currentUser.displayStreakPlan();
            auto weak = currentUser.getWeakAreas();
            if (!weak.empty()) {
                cout << "\nWeak areas needing revision:\n";
                for (const auto& w : weak) cout << "  - " << w << "\n";
            }
            pause();
        }

        else if (mainChoice == 6) {
            // ---- Daily Challenge ----
            cout << "\n--- DAILY CHALLENGE ---\n";
            cout << "Choose language: ";
            for (int i=0;i<7;i++) cout << (i+1) << "." << langNames[i] << " ";
            cout << "\nChoice: ";
            int dc = readInt();
            if (dc >= 1 && dc <= 7) langs[dc-1]->dailyChallenge();
            else cout << "Invalid.\n";
            pause();
        }

        else if (mainChoice == 7) {
            // ---- Add Custom Culture Aspect ----
            cout << "\n--- ADD CUSTOM CULTURE ASPECT ---\n";
            cout << "Which culture? (e.g. Punjabi, Urdu, Sindhi, Balochi, Pashto, Kashmiri, Gilgiti): ";
            string cultureName = readLine();
            cout << "Aspect title: ";
            string title = readLine();
            cout << "Description: ";
            string desc = readLine();
            if (!title.empty() && !desc.empty()) {
                FileHandler::saveCustomAspect(cultureName, currentUser.getUsername(), title, desc);
                currentUser.awardBadge("Culture Explorer");
                FileHandler::saveUser(currentUser);
                cout << "Custom aspect saved successfully!\n";
            } else {
                cout << "Title and description cannot be empty.\n";
            }
            pause();
        }

        else if (mainChoice == 8) {
            // ---- View Custom Culture Aspects ----
            cout << "\n--- USER-ADDED CULTURE ASPECTS ---\n";
            FileHandler::showCustomAspects();
            pause();
        }

        else if (mainChoice == 0) {
            FileHandler::saveUser(currentUser);
            FileHandler::updateLeaderboard(currentUser);
            cout << "\nProgress saved. Goodbye, " << currentUser.getUsername() << "!\n";
            cout << "Your streak: " << currentUser.getStreak() << " days\n";
            cout << "Keep learning! Challo Ji!\n";
        }

        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}