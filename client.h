#include <iostream>
#include <vector>
#include <fstream>

#define length 6
#define space_length 20

using namespace std;
typedef string entrustment_string;
typedef double entrustment_double;
typedef int entrustment_int;

// 委托类
class entrustment
{
public:
    entrustment() = default;
    entrustment(entrustment_string name, entrustment_string time, entrustment_string location, entrustment_string content, entrustment_double profit) : e_name(name), e_time(time), e_location(location), e_content(content), e_star(0), e_profit(profit), e_state(false)
    {
        e_code++;
        for (auto i : time)
            if (isalnum(i))
                e_id += i;

        for (int digit = 0; digit < length - e_code; ++digit)
            e_id += '0';

        e_id += to_string(e_code);
        e_star = 1200;

        ofstream ofs;
        ofs.open("面向对象课设\\委托信息.txt", ios::app);
        ofs << left
            << setw(space_length) << "序列号: " << e_id << endl
            << setw(space_length) << "标题: " << e_name << endl
            << setw(space_length) << "时间: " << e_time << endl
            << setw(space_length) << "地点: " << e_location << endl
            << setw(space_length) << "内容: " << e_content << endl
            << setw(space_length) << "星级: " << e_star << endl
            << setw(space_length) << "报酬: " << e_profit << endl
            << endl;

        ofs.close();
    }
    entrustment_string getId() const { return e_id; }
    entrustment_string getName() const { return e_name; }
    entrustment_string getTime() const { return e_time; }
    entrustment_string getLocation() const { return e_location; }
    entrustment_string getContent() const { return e_content; }
    entrustment_double getStar() const { return e_star; }
    entrustment_double getProfit() const { return e_profit; }
    static entrustment_int getCode() { return e_code; }
    bool getState() { return e_state; }
    void setState(bool target) { e_state = target; }

    void addComment(const string &c)
    {
        comments.push_back(c);
    }

    const vector<string> &getComments() const
    {
        return comments;
    }

private:
    entrustment_string e_id;
    entrustment_string e_name;
    entrustment_string e_time;
    entrustment_string e_location;
    entrustment_string e_content;
    entrustment_double e_star;
    entrustment_double e_profit;
    static entrustment_int e_code; // 用于生成序列号
    bool e_state;                  // 委托接受状态
    vector<entrustment_string> comments;
};
entrustment_int entrustment::e_code = 0;
