#include <iostream>

using namespace std;
#define length 6
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
    }
    entrustment_string getId() { return e_id; }
    entrustment_string getName() { return e_name; }
    entrustment_string getTime() { return e_time; }
    entrustment_string getLocation() { return e_location; }
    entrustment_string getContent() { return e_content; }
    entrustment_double getStar() { return e_star; }
    entrustment_double getProfit() { return e_profit; }
    static entrustment_int getCode() { return e_code; }
    bool getState() { return e_state; }
    void setState(bool target) { e_state = target; }

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
};
entrustment_int entrustment::e_code = 0;
