#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
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
        m_star = 1200;  // 初始星值为1200
        m_credit = 100; // 信誉分满分100分
        m_number_completed_entrustment = 0;
        m_number_uncompleted_entrustment = 0;
        m_number_entrustment = m_number_completed_entrustment + m_number_uncompleted_entrustment;

        if (authority)
            m_id += '1';
        else
            m_id += '0';
    }
    // 拷贝构造
    client(const client &other) : m_name(other.m_name), m_id(other.m_id), m_gender(other.m_gender), m_phone(other.m_phone), m_password(other.m_password), m_email(other.m_email), m_star(other.m_star), m_credit(other.m_credit), m_number_entrustment(other.m_number_entrustment), m_number_completed_entrustment(other.m_number_completed_entrustment), m_number_uncompleted_entrustment(other.m_number_uncompleted_entrustment), m_accept_ids(other.m_accept_ids), m_dispatch_ids(other.m_dispatch_ids), m_authority(other.m_authority) {}
    // 移动构造
    client(client &&other) noexcept : m_name(std::move(other.m_name)), m_id(std::move(other.m_id)), m_gender(std::move(other.m_gender)), m_phone(std::move(other.m_phone)), m_password(std::move(other.m_password)), m_email(std::move(other.m_email)), m_star(other.m_star), m_credit(other.m_credit), m_number_entrustment(other.m_number_entrustment), m_number_completed_entrustment(other.m_number_completed_entrustment), m_number_uncompleted_entrustment(other.m_number_uncompleted_entrustment), m_accept_ids(std::move(other.m_accept_ids)), m_dispatch_ids(std::move(other.m_dispatch_ids)), m_authority(other.m_authority)
    {
        other.m_star = 0.0;
        other.m_credit = 0.0;
        other.m_number_entrustment = 0;
        other.m_number_completed_entrustment = 0;
        other.m_number_uncompleted_entrustment = 0;
        other.m_authority = NULL;
    }

    client &operator=(const client &other)
    {
        if (this != &other)
        {
            m_name = other.m_name;
            m_id = other.m_id;
            m_gender = other.m_gender;
            m_phone = other.m_phone;
            m_password = other.m_password;
            m_email = other.m_email;
            m_star = other.m_star;
            m_credit = other.m_credit;
            m_number_completed_entrustment = other.m_number_completed_entrustment;
            m_number_uncompleted_entrustment = other.m_number_uncompleted_entrustment;
            m_number_entrustment = other.m_number_entrustment;
            m_dispatch_ids = other.m_dispatch_ids;
            m_accept_ids = other.m_accept_ids;
            m_authority = other.m_authority;
        }
        return *this;
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
            m_accept_ids = std::move(other.m_accept_ids);
            m_dispatch_ids = std::move(other.m_dispatch_ids);
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
    void setPassword(const string &v) { m_password = v; }
    void setID(const string &v) { m_id = v; }

    // 添加 ID
    void addAcceptId(const string &id) { m_accept_ids.push_back(id); };
    void addDispatchId(const string &id) { m_dispatch_ids.push_back(id); };

    // 删除 ID
    void removeAcceptId(const string &id)
    {
        m_accept_ids.erase(remove(m_accept_ids.begin(), m_accept_ids.end(), id),
                           m_accept_ids.end());
    };
    void removeDispatchId(const string &id)
    {
        m_dispatch_ids.erase(remove(m_dispatch_ids.begin(), m_dispatch_ids.end(), id),
                             m_dispatch_ids.end());
    };

    // ===== 委托历史 =====
    const vector<entrustment_string> &getDispatchHistory() const { return m_dispatch_ids; }
    const vector<entrustment_string> &getAcceptHistory() const { return m_accept_ids; }
    // 获取承接历史引用
    vector<entrustment_string> &accessAcceptHistory() { return m_accept_ids; }
    // 获取发布历史引用
    vector<entrustment_string> &accessDispatchHistory() { return m_dispatch_ids; }

    // ===== 权限 & 规则 =====
    bool isAdmin() { return m_authority; }
    void setCreait(client_double creait) { m_credit = creait; }

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

    // ===== 写入文件 =====
    string serialize() const
    {
        ostringstream oss;
        oss << m_id << "\n"
            << m_name << "\n"
            << m_gender << "\n"
            << m_phone << "\n"
            << m_password << "\n"
            << m_email << "\n"
            << m_star << "\n"
            << m_credit << "\n"
            << m_number_completed_entrustment << "\n"
            << m_number_uncompleted_entrustment << "\n"
            << m_number_entrustment << "\n";
        if (m_authority)
            oss << "1\n";
        else
            oss << "0\n";

        // ===== 承接委托 =====
        oss << m_accept_ids.size() << "\n";
        for (auto &id : m_accept_ids)
            oss << id << "\n";

        // ===== 发布委托 =====
        oss << m_dispatch_ids.size() << "\n";
        for (auto &id : m_dispatch_ids)
            oss << id << "\n";

        oss << "END_USER\n";
        return oss.str();
    }

    // ===== 从文件读取 =====
    static client deserialize(ifstream &ifs)
    {
        client u;
        string line;

        // === 跳过空行 ===
        while (getline(ifs, line))
        {
            if (!line.empty())
            {
                u.m_id = line;
                break;
            }
        }

        if (u.m_id.empty())
            throw runtime_error("EOF");

        getline(ifs, u.m_name);
        getline(ifs, u.m_gender);
        getline(ifs, u.m_phone);
        getline(ifs, u.m_password);
        getline(ifs, u.m_email);

        getline(ifs, line);
        u.m_star = stod(line);

        getline(ifs, line);
        u.m_credit = stod(line);

        getline(ifs, line);
        u.m_number_completed_entrustment = stoi(line);
        getline(ifs, line);
        u.m_number_uncompleted_entrustment = stoi(line);
        getline(ifs, line);
        u.m_number_entrustment = stoi(line);

        getline(ifs, line);
        u.m_authority = (line == "1");

        // ===== 承接委托 =====
        getline(ifs, line);
        int acceptCnt = stoi(line);
        for (int i = 0; i < acceptCnt; ++i)
        {
            getline(ifs, line);
            u.m_accept_ids.push_back(line);
        }

        // ===== 发布委托 =====
        getline(ifs, line);
        int dispatchCnt = stoi(line);
        for (int i = 0; i < dispatchCnt; ++i)
        {
            getline(ifs, line);
            u.m_dispatch_ids.push_back(line);
        }

        getline(ifs, line); // END_USER
        return u;
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
    vector<entrustment_string> m_dispatch_ids;
    vector<entrustment_string> m_accept_ids;
    bool m_authority; // 账户权限
};
