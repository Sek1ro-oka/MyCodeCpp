#ifndef ENTRUSTMENT_H
#define ENTRUSTMENT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

using namespace std;
typedef string client_string;

#define length 6
#define space_length 20

typedef string entrustment_string;
typedef double entrustment_double;
typedef int entrustment_int;

class entrustment
{
public:
    entrustment() = default;

    entrustment(entrustment_string name,
                entrustment_string time,
                entrustment_string location,
                entrustment_string content,
                entrustment_double profit,
                client_string boss)
        : e_name(name),
          e_time(time),
          e_location(location),
          e_content(content),
          e_profit(profit),
          boss_id(boss),
          worker_id(""),
          e_state(false),
          e_finished(false)
    {
        for (char c : time)
            if (isalnum(c))
                e_id += c;
    }

    /* ===== getter ===== */
    entrustment_string getId() const { return e_id; }
    entrustment_string getName() const { return e_name; }
    entrustment_string getTime() const { return e_time; }
    entrustment_string getLocation() const { return e_location; }
    entrustment_string getContent() const { return e_content; }
    entrustment_double getStar() const { return e_star; }
    entrustment_double getProfit() const { return e_profit; }
    entrustment_string getWorkId() const { return worker_id; }
    entrustment_string getBossId() const { return boss_id; }
    const vector<string> &getComments() const { return e_comments; }

    /* ===== setter ===== */
    void setId(const entrustment_string &v) { e_id = v; }
    bool getState() const { return e_state; }
    void setState(bool v) { e_state = v; }
    void setStar(entrustment_double v) { e_star = v; }
    void setWorker(const client_string &id) { worker_id = id; }
    bool isFinished() const { return e_finished; }
    void setFinished(bool v) { e_finished = v; }

    /* ===== 序列化 ===== */
    string serialize() const
    {
        ostringstream oss;

        oss << e_id << "\n"
            << e_name << "\n"
            << e_time << "\n"
            << e_location << "\n"
            << e_content << "\n"
            << e_star << "\n"
            << e_profit << "\n"
            << e_state << "\n"
            << e_finished << "\n"
            << boss_id << "\n"
            << worker_id << "\n";

        // ===== 评论 =====
        oss << e_comments.size() << "\n";
        for (auto &c : e_comments)
            oss << c << "\n";

        oss << "END_ENTRUST\n";
        return oss.str();
    }

    static entrustment deserialize(ifstream &ifs)
    {
        entrustment e;
        string line;

        getline(ifs, e.e_id);
        if (e.e_id.empty())
            throw runtime_error("EOF");

        getline(ifs, e.e_name);
        getline(ifs, e.e_time);
        getline(ifs, e.e_location);
        getline(ifs, e.e_content);

        getline(ifs, line);
        e.e_star = stod(line);
        getline(ifs, line);
        e.e_profit = stod(line);
        getline(ifs, line);
        e.e_state = stoi(line);
        getline(ifs, line);
        e.e_finished = stoi(line);

        getline(ifs, e.boss_id);
        getline(ifs, e.worker_id);

        // ===== 评论 =====
        getline(ifs, line);
        int cnt = stoi(line);
        for (int i = 0; i < cnt; ++i)
        {
            getline(ifs, line);
            e.e_comments.push_back(line);
        }

        getline(ifs, line); // END_ENTRUST
        return e;
    }

    // 添加评论
    void addComment(const string &c)
    {
        e_comments.push_back(c);
    }

private:
    entrustment_string e_id;
    entrustment_string e_name;
    entrustment_string e_time;
    entrustment_string e_location;
    entrustment_string e_content;
    entrustment_double e_star{};
    entrustment_double e_profit{};

    vector<string> e_comments;
    client_string boss_id;
    client_string worker_id;

    bool e_state{};
    bool e_finished{};
};

#endif
