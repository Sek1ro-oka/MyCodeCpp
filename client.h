#include <iomanip>
#include "entrustment.h"

#define Base_Gain 20
#define Base_Loss 15
#define C 50
#define Min_Rate 0.1

typedef string client_string;
typedef double client_double;
typedef int client_int;

// 用户类
class client
{
public:
    client() = default;
    client(client_string name, client_string gender, client_string phone, client_string password, client_string email, bool authority = false) : m_name(name), m_gender(gender), m_phone(phone), m_password(password), m_email(email), m_authority(authority)
    {
        m_code++;
        m_star = 1200;  // 初始星值为1200
        m_credit = 100; // 信誉分满分100分
        m_number_completed_entrustment = 0;
        m_number_uncompleted_entrustment = 0;
        m_number_entrustment = m_number_completed_entrustment + m_number_uncompleted_entrustment;

        for (int digit = 0; digit < length - m_code; ++digit)
            m_id += '0';
        m_id += to_string(m_code);
        if (authority)
            m_id += '1';
        else
            m_id += '0';

        ofstream ofs;
        ofs.open("面向对象课设\\用户信息.txt", ios::app);
        ofs << left
            << setw(space_length) << "姓名: " << m_name << endl
            << setw(space_length) << "序列号: " << m_id << endl
            << setw(space_length) << "性别: " << m_gender << endl
            << setw(space_length) << "手机号: " << m_phone << endl
            << setw(space_length) << "账户密码: " << m_password << endl
            << setw(space_length) << "邮箱地址: " << m_email << endl
            << setw(space_length) << "星级: " << m_star << endl
            << setw(space_length) << "信誉分: " << m_credit << endl
            << setw(space_length) << "承接委托数量: " << m_number_entrustment << endl
            << setw(space_length) << "完成委托数量: " << m_number_completed_entrustment << endl
            << setw(space_length) << "未完成委托数量: " << m_number_uncompleted_entrustment << endl
            << setw(space_length) << "用户权限: ";
        if (m_authority)
            ofs << "管理员" << endl;
        else
            ofs << "普通用户" << endl;
        ofs << endl;
        ofs.close();
    }
    // 拷贝构造
    client(const client &other) : m_name(other.m_name), m_id(other.m_id), m_gender(other.m_gender), m_phone(other.m_phone), m_password(other.m_password), m_email(other.m_email), m_star(other.m_star), m_credit(other.m_credit), m_number_entrustment(other.m_number_entrustment), m_number_completed_entrustment(other.m_number_completed_entrustment), m_number_uncompleted_entrustment(other.m_number_uncompleted_entrustment), m_accept_history(other.m_accept_history), m_dispatch_history(other.m_dispatch_history), m_authority(other.m_authority) { m_code++; }
    // 移动构造
    client(client &&other) noexcept : m_name(std::move(other.m_name)), m_id(std::move(other.m_id)), m_gender(std::move(other.m_gender)), m_phone(std::move(other.m_phone)), m_password(std::move(other.m_password)), m_email(std::move(other.m_email)), m_star(other.m_star), m_credit(other.m_credit), m_number_entrustment(other.m_number_entrustment), m_number_completed_entrustment(other.m_number_completed_entrustment), m_number_uncompleted_entrustment(other.m_number_uncompleted_entrustment), m_accept_history(std::move(other.m_accept_history)), m_dispatch_history(std::move(other.m_dispatch_history)), m_authority(other.m_authority)
    {
        other.m_star = 0.0;
        other.m_credit = 0.0;
        other.m_number_entrustment = 0;
        other.m_number_completed_entrustment = 0;
        other.m_number_uncompleted_entrustment = 0;
        other.m_authority = NULL;
    }

    client &operator=(client &&other) noexcept
    {
        if (this != &other)
        {
            m_name = std::move(other.m_name);
            m_id = std::move(other.m_id);
            m_gender = std::move(other.m_gender);
            m_phone = std::move(other.m_phone);
            m_password = std::move(other.m_password);
            m_email = std::move(other.m_email);
            m_star = other.m_star;
            m_credit = other.m_credit;
            m_number_entrustment = other.m_number_entrustment;
            m_number_completed_entrustment = other.m_number_completed_entrustment;
            m_number_uncompleted_entrustment = other.m_number_uncompleted_entrustment;
            m_accept_history = std::move(other.m_accept_history);
            m_dispatch_history = std::move(other.m_dispatch_history);
            m_authority = other.m_authority;

            // 重置原对象
            other.m_star = 0.0;
            other.m_credit = 0;
            other.m_number_entrustment = 0;
            other.m_number_completed_entrustment = 0;
            other.m_number_uncompleted_entrustment = 0;
            other.m_authority = NULL;
        }
        return *this;
    }

    // ===== 基本信息 =====
    client_string getName() const { return m_name; }
    client_string getGender() const { return m_gender; }
    client_string getID() const { return m_id; }
    client_string getPhone() const { return m_phone; }
    client_string getPassword() const { return m_password; }
    client_string getEmail() const { return m_email; }
    client_double getStar() const { return m_star; }
    client_double getCreait() const { return m_credit; }
    client_int getSN() const { return m_number_completed_entrustment; }
    client_int getFN() const { return m_number_uncompleted_entrustment; }
    client_int getS() const { return m_number_entrustment; }

    void setName(const string &v) { m_name = v; }
    void setGender(const string &v) { m_gender = v; }
    void setPhone(const string &v) { m_phone = v; }
    void setEmail(const string &v) { m_email = v; }

    // ===== 委托历史 =====
    const vector<entrustment> &getDispatchHistory() const { return m_dispatch_history; }
    const vector<entrustment> &getAcceptHistory() const { return m_accept_history; }

    // ===== 权限 & 规则 =====
    bool isAdmin() { return m_authority; }
    static client_int getCode() { return m_code; }
    void setCreait(client_double creait) { m_credit = creait; }

    // 显示用户的具体信息
    void showInformation()
    {
        cout << "=========================用户信息如下=========================\n";
        cout << left
             << setw(space_length) << "姓名: " << m_name << endl
             << setw(space_length) << "序列号: " << m_id << endl
             << setw(space_length) << "性别: " << m_gender << endl
             << setw(space_length) << "手机号: " << m_phone << endl
             << setw(space_length) << "账户密码: " << m_password << endl
             << setw(space_length) << "邮箱地址: " << m_email << endl
             << setw(space_length) << "星级: " << m_star << endl
             << setw(space_length) << "信誉分: " << m_credit << endl
             << setw(space_length) << "承接委托数量: " << m_number_entrustment << endl
             << setw(space_length) << "完成委托数量: " << m_number_completed_entrustment << endl
             << setw(space_length) << "未完成委托数量: " << m_number_uncompleted_entrustment << endl
             << setw(space_length) << "用户权限: ";
        if (m_authority)
            cout << "管理员" << endl;
        else
            cout << "普通用户" << endl;
        cout << setw(space_length) << "段位: " << getStarLevel() << endl;
    }

    void show_accpect_history()
    {
        if (m_accept_history.size() <= 0)
            return;
        cout << "=========================历史承接委托信息如下=========================\n";
        cout << left;
        for (auto event : m_accept_history)
            cout << setw(space_length) << "序列号: " << event.getId() << endl
                 << setw(space_length) << "标题: " << event.getName() << endl
                 << setw(space_length) << "时间: " << event.getTime() << endl
                 << setw(space_length) << "地点: " << event.getLocation() << endl
                 << setw(space_length) << "内容: " << event.getContent() << endl
                 << setw(space_length) << "星级: " << event.getStar() << endl
                 << setw(space_length) << "报酬: " << event.getProfit() << endl
                 << endl;
        cout << "历史承接委托数量: " << m_accept_history.size() << endl;
    }

    void show_dispatch_history()
    {
        if (m_dispatch_history.size() <= 0)
            return;
        cout << "=========================历史发布委托信息如下=========================\n";
        cout << left;
        for (auto event : m_dispatch_history)
            cout << setw(space_length) << "序列号: " << event.getId() << endl
                 << setw(space_length) << "标题: " << event.getName() << endl
                 << setw(space_length) << "时间: " << event.getTime() << endl
                 << setw(space_length) << "地点: " << event.getLocation() << endl
                 << setw(space_length) << "内容: " << event.getContent() << endl
                 << setw(space_length) << "星级: " << event.getStar() << endl
                 << setw(space_length) << "报酬: " << event.getProfit() << endl
                 << endl;
        cout << "发布委托数量: " << m_dispatch_history.size() << endl;
    }

    // 计算星级
    void updateStar(bool flag) // 委托成功flag = true 否则, flag = false
    {
        client_int total = m_number_completed_entrustment + m_number_uncompleted_entrustment;
        client_double deltaS;
        m_number_entrustment++;

        if (flag)
        {
            m_number_completed_entrustment++;
            if (total == 0) // 首次委托
                deltaS = Base_Gain;
            else
            {
                client_double K = 1 / (1 + m_number_entrustment * 1.0 / C);
                client_double Rate = m_number_completed_entrustment * 1.0 / m_number_entrustment;
                deltaS = Base_Gain * K * (1 - Rate);
            }
            m_star += deltaS;
        }

        else
        {
            m_number_uncompleted_entrustment++;
            if (total == 0)
                deltaS = -Base_Loss;
            else
            {
                client_double K = 1 / (1 + m_number_entrustment * 1.0 / C);
                client_double Rate = total * 1.0 / m_number_entrustment;
                client_double Rate_calc = max(Rate, Min_Rate);
                deltaS = -Base_Loss * K * (1.0 / Rate_calc);
            }
            m_star = max(m_star + deltaS, 0.0);
        }
    }

    // 创建委托
    entrustment Creat_entrustment(entrustment_string name, entrustment_string time, entrustment_string location, entrustment_string content, entrustment_double profit)
    {
        entrustment event(name, time, location, content, profit);
        m_dispatch_history.push_back(event);
        cout << "发布委托数量: " << m_dispatch_history.size() << endl;
        return event;
    }

    // 接受委托
    void Accept(entrustment &temp)
    {
        temp.setState(true);
        m_accept_history.push_back(temp);
    }

    // 委托未能如期完成
    void MissonFail(entrustment &tmp)
    {
        tmp.setState(false);
        updateStar(false);
    }

    // 完成委托时增加星值
    void MissonSuccess(entrustment &tmp)
    {
        tmp.setState(false);
        updateStar(true);
    }

    // 获取星级名称
    string getStarLevel() const
    {
        if (m_star < 1200)
            return "小白";
        if (m_star < 1400)
            return "青铜";
        if (m_star < 1600)
            return "铂金";
        if (m_star < 1800)
            return "黄金";
        if (m_star < 2000)
            return "白金";
        if (m_star < 2200)
            return "黑金";
        if (m_star < 2400)
            return "红名";
        return "传奇";
    }
    // 信誉判断
    bool canAcceptEntrustment()
    {
        return m_credit >= 90;
    }

private:
    client_string m_name;
    client_string m_id;
    client_string m_gender;
    client_string m_phone;
    client_string m_password;
    client_string m_email;
    client_double m_star;
    client_double m_credit;
    client_int m_number_completed_entrustment;
    client_int m_number_uncompleted_entrustment;
    client_int m_number_entrustment;
    vector<entrustment> m_dispatch_history;
    vector<entrustment> m_accept_history;
    static client_int m_code; // 用于生成序列号
    bool m_authority;         // 账户权限
};
client_int client::m_code = 0;
