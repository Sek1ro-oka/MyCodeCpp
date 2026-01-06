#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cstdlib> // 用于system和srand/rand
using namespace std;

// 类型定义
typedef string client_string;
typedef string entrustment_string;
typedef double client_double;
typedef double entrustment_double;
typedef int client_int;
typedef int entrustment_int;

// 配置参数（可按需调整）
const int USER_COUNT = 2000;             // 生成用户数量
const int ADMIN_COUNT = 10;              // 管理员数量
const int ENTRUSTMENT_COUNT = 2000;      // 生成委托数量
const int MAX_COMMENTS_PER_ENTRUST = 10; // 每个委托最多评论数
const int MAX_ENTRUST_PER_USER = 20;     // 每个用户最多发布/承接委托数

// 初始化随机数生成器
void init_random()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

// 随机字符串生成
string random_string(int length, bool only_alpha = true)
{
    string chars;
    if (only_alpha)
    {
        chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    else
    {
        chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    }
    string res;
    for (int i = 0; i < length; ++i)
    {
        res += chars[rand() % chars.size()];
    }
    return res;
}

// 随机姓名生成（基于百家姓+常用名）
string random_name()
{
    const vector<string> surnames = {
        "赵", "钱", "孙", "李", "周", "吴", "郑", "王", "冯", "陈",
        "褚", "卫", "川", "沈", "曹", "杨", "朱", "秦", "尤", "许"};
    const vector<string> names = {
        "备", "云", "飞", "敏", "光", "普", "千", "洋", "朗", "娜",
        "涛", "明", "华", "权", "非", "军", "操", "刚", "勇", "蒙"};
    return surnames[rand() % surnames.size()] + names[rand() % names.size()];
}

// 随机手机号生成
string random_phone()
{
    const vector<string> prefixes = {"138", "139", "150", "159", "186", "189", "136", "177"};
    string phone = prefixes[rand() % prefixes.size()];
    for (int i = 0; i < 8; ++i)
    {
        phone += to_string(rand() % 10);
    }
    return phone;
}

// 随机邮箱生成
string random_email()
{
    const vector<string> domains = {"@qq.com", "@163.com", "@gmail.com", "@126.com", "@sina.com"};
    return random_string(8) + domains[rand() % domains.size()];
}

// 随机时间字符串（格式：YYYY-MM-DD HH:MM）
string random_time()
{
    int year = 2024 + rand() % 2;
    int month = 1 + rand() % 12;
    int day = 1 + rand() % 28; // 简化处理，避免月份天数问题
    int hour = rand() % 24;
    int minute = rand() % 60;

    ostringstream oss;
    oss << year << "-"
        << setw(2) << setfill('0') << month << "-"
        << setw(2) << setfill('0') << day << " "
        << setw(2) << setfill('0') << hour << ":"
        << setw(2) << setfill('0') << minute;
    return oss.str();
}

// 随机地点生成
string random_location()
{
    const vector<string> cities = {"北京市", "上海市", "广州市", "深圳市", "杭州市", "成都市", "武汉市", "西安市"};
    const vector<string> areas = {"朝阳区", "浦东新区", "天河区", "南山区", "西湖区", "武侯区", "洪山区", "雁塔区"};
    const vector<string> streets = {"XX街道", "XX路", "XX小区", "XX广场", "XX大厦"};
    return cities[rand() % cities.size()] + areas[rand() % areas.size()] + streets[rand() % streets.size()];
}

// 随机委托内容生成
string random_content()
{
    const vector<string> contents = {
        "帮忙取快递，送到楼下",
        "代买奶茶，甜度三分糖少冰",
        "宠物喂养，每天一次，共三天",
        "文件打印，需要彩色双面",
        "搬家帮忙，主要是小件物品",
        "辅导作业，小学五年级数学",
        "遛狗，每天半小时，一周",
        "代排队，医院挂号",
        "家电维修咨询，空调不制冷",
        "拍照服务，景点打卡"
        "照顾曼波",
        "帮我改bug吧",
    };
    return contents[rand() % contents.size()] + " " + random_string(5);
}

// 原工程的委托星级计算函数
entrustment_double calcEntrustStar(entrustment_double profit)
{
    if (profit <= 100)
        return 0.0;
    if (profit <= 200)
        return 1200.0;
    if (profit <= 300)
        return 1400.0;
    if (profit <= 500)
        return 1600.0;
    if (profit <= 800)
        return 1800.0;
    if (profit <= 1000)
        return 2000.0;
    if (profit <= 1500)
        return 2200.0;
    return 2400.0;
}

// 生成用户ID（0/1开头 + 5位数字）
string generate_user_id(bool is_admin, int seq)
{
    ostringstream oss;
    oss << (is_admin ? "1" : "0")
        << setw(5) << setfill('0') << seq;
    return oss.str();
}

// 生成委托ID（E + 5位数字）
string generate_entrust_id(int seq)
{
    ostringstream oss;
    oss << "E" << setw(5) << setfill('0') << seq;
    return oss.str();
}

// 生成随机评论
string random_comment(const string &username)
{
    const vector<string> comments = {
        "效率很高，值得推荐！",
        "服务态度很好，下次还找你",
        "有点小失误，但整体不错",
        "非常靠谱，按时完成",
        "价格合理，性价比高",
        "沟通很顺畅，好评",
        "比预期的还要好，点赞",
        "一般般，不算太满意",
        "ZAFUer",
        "不",
        "语",
        "只",
        "是",
        "一",
        "味",
        "点",
        "赞"};
    return username + ": " + comments[rand() % comments.size()];
}

// 用户数据结构（简化版，用于生成）
struct User
{
    string id;
    string name;
    string gender;
    string phone;
    string password;
    string email;
    double star;
    double credit;
    int completed;
    int uncompleted;
    int total;
    bool is_admin;
    vector<string> accept_ids;   // 承接的委托ID
    vector<string> dispatch_ids; // 发布的委托ID

    // 序列化为原工程格式
    string serialize() const
    {
        ostringstream oss;
        oss << id << "\n"
            << name << "\n"
            << gender << "\n"
            << phone << "\n"
            << password << "\n"
            << email << "\n"
            << star << "\n"
            << credit << "\n"
            << completed << "\n"
            << uncompleted << "\n"
            << total << "\n"
            << (is_admin ? "1\n" : "0\n");

        // 承接委托
        oss << accept_ids.size() << "\n";
        for (const auto &aid : accept_ids)
        {
            oss << aid << "\n";
        }

        // 发布委托
        oss << dispatch_ids.size() << "\n";
        for (const auto &did : dispatch_ids)
        {
            oss << did << "\n";
        }

        oss << "END_USER\n";
        return oss.str();
    }
};

// 委托数据结构（简化版，用于生成）
struct Entrustment
{
    string id;
    string name;
    string time;
    string location;
    string content;
    double star;
    double profit;
    bool state;    // 是否被承接
    bool finished; // 是否完成
    string boss_id;
    string worker_id;
    vector<string> comments;

    // 序列化为原工程格式
    string serialize() const
    {
        ostringstream oss;
        oss << id << "\n"
            << name << "\n"
            << time << "\n"
            << location << "\n"
            << content << "\n"
            << star << "\n"
            << profit << "\n"
            << (state ? "1\n" : "0\n")
            << (finished ? "1\n" : "0\n")
            << boss_id << "\n"
            << worker_id << "\n";

        // 评论
        oss << comments.size() << "\n";
        for (const auto &c : comments)
        {
            oss << c << "\n";
        }

        oss << "END_ENTRUST\n";
        return oss.str();
    }
};

// 创建目录
void create_dir(const string &dir_name)
{
    string cmd = "md " + dir_name + " 2>NUL"; // 2>NUL屏蔽重复创建的错误提示
    system(cmd.c_str());
}

int main()
{
    // 初始化随机数
    init_random();

    // 创建目标目录（面向对象课设）
    const string target_dir = "面向对象课设";
    create_dir(target_dir);

    // ========== 1. 生成用户 ==========
    vector<User> users;
    // 生成管理员
    for (int i = 0; i < ADMIN_COUNT; ++i)
    {
        User u;
        u.id = generate_user_id(true, i + 1);
        u.name = "管理员" + to_string(i + 1);
        u.gender = (rand() % 2) ? "男" : "女";
        u.phone = random_phone();
        u.password = "admin" + to_string(i + 1);
        u.email = "admin" + to_string(i + 1) + "@admin.com";
        u.star = 2000.0 + rand() % 800; // 管理员星级偏高
        u.credit = 100.0;               // 管理员信誉分满分
        u.completed = rand() % 30;
        u.uncompleted = rand() % 5;
        u.total = u.completed + u.uncompleted;
        u.is_admin = true;
        users.push_back(u);
    }

    // 生成普通用户
    for (int i = 0; i < USER_COUNT - ADMIN_COUNT; ++i)
    {
        User u;
        u.id = generate_user_id(false, ADMIN_COUNT + i + 1);
        u.name = random_name();
        u.gender = (rand() % 2) ? "男" : "女";
        u.phone = random_phone();
        u.password = "123456" + random_string(3);
        u.email = random_email();
        u.star = 1200.0 + rand() % 1400; // 1200-2600
        u.credit = 90.0 + rand() % 11;   // 90-100
        u.completed = rand() % 20;
        u.uncompleted = rand() % 10;
        u.total = u.completed + u.uncompleted;
        u.is_admin = false;
        users.push_back(u);
    }

    // ========== 2. 生成委托 ==========
    vector<Entrustment> entrustments;
    for (int i = 0; i < ENTRUSTMENT_COUNT; ++i)
    {
        Entrustment e;
        e.id = generate_entrust_id(i + 1);
        e.name = "委托_" + random_string(6);
        e.time = random_time();
        e.location = random_location();
        e.content = random_content();
        e.profit = 20.0 + rand() % 2000; // 20-2020元
        e.star = calcEntrustStar(e.profit);
        e.state = (rand() % 3) != 0;                 // 约2/3的委托被承接
        e.finished = (e.state && (rand() % 2)) != 0; // 被承接的委托约50%完成

        // 随机选择发布者（老板）
        int boss_idx = rand() % users.size();
        e.boss_id = users[boss_idx].id;
        users[boss_idx].dispatch_ids.push_back(e.id);
        // 限制每个用户发布的委托数
        if (users[boss_idx].dispatch_ids.size() > MAX_ENTRUST_PER_USER)
        {
            users[boss_idx].dispatch_ids.erase(users[boss_idx].dispatch_ids.begin());
        }

        // 随机选择承接者（工人）（如果委托被承接）
        e.worker_id = "";
        if (e.state)
        {
            int worker_idx;
            do
            {
                worker_idx = rand() % users.size();
            } while (users[worker_idx].id == e.boss_id); // 不能自己承接自己的委托

            e.worker_id = users[worker_idx].id;
            users[worker_idx].accept_ids.push_back(e.id);
            // 限制每个用户承接的委托数
            if (users[worker_idx].accept_ids.size() > MAX_ENTRUST_PER_USER)
            {
                users[worker_idx].accept_ids.erase(users[worker_idx].accept_ids.begin());
            }
        }

        // 生成随机评论
        int comment_count = rand() % (MAX_COMMENTS_PER_ENTRUST + 1);
        for (int j = 0; j < comment_count; ++j)
        {
            int commenter_idx = rand() % users.size();
            e.comments.push_back(random_comment(users[commenter_idx].name));
        }

        entrustments.push_back(e);
    }

    // ========== 3. 写入users.txt（到面向对象课设目录） ==========
    string user_file_path = target_dir + "\\users.txt";
    ofstream user_file(user_file_path);
    if (!user_file)
    {
        cerr << "无法创建 " << user_file_path << " 文件！" << endl;
        return 1;
    }
    for (const auto &user : users)
    {
        user_file << user.serialize();
    }
    user_file.close();
    cout << "已生成 " << users.size() << " 个用户数据到 " << user_file_path << endl;

    // ========== 4. 写入entrustments.txt（到面向对象课设目录） ==========
    string entrust_file_path = target_dir + "\\entrustments.txt";
    ofstream entrust_file(entrust_file_path);
    if (!entrust_file)
    {
        cerr << "无法创建 " << entrust_file_path << " 文件！" << endl;
        return 1;
    }
    for (const auto &entrust : entrustments)
    {
        entrust_file << entrust.serialize();
    }
    entrust_file.close();
    cout << "已生成 " << entrustments.size() << " 个委托数据到 " << entrust_file_path << endl;

    cout << "\n文件生成完成！路径：./" << target_dir << "/" << endl;
    return 0;
}
