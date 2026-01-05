#include <conio.h>
#include <limits>
#include "server.h"

using namespace std;

/* ================== 页面枚举 ================== */
enum class Page
{
    LOGIN,          // 登录
    REGISTER,       // 注册
    ENTRUST_LIST,   // 委托列表
    MY_DISPATCH,    // 我发布的委托
    MY_ACCEPT,      // 我承接的委托
    USER_INFO,      // 用户个人信息
    USER_PUBLISH,   // 发布委托
    ENTRUST_DETAIL, // 委托详情
    ACCEPT_DETAIL,  // ? 新增：承接委托详情
    USER_EDIT       // 编辑个人页面

};

/* ================== 可切换页面表 ================== */
vector<Page> tabPages = {
    Page::ENTRUST_LIST,
    Page::MY_DISPATCH,
    Page::MY_ACCEPT,
    Page::USER_INFO};

void clear()
{
    system("cls");
}

/* ================== 委托详情 ================== */
void drawEntrustDetail(const entrustment *e)
{
    cout << "【委托详情】\n\n";
    cout << "标题：" << e->getName() << endl;
    cout << "编号：" << e->getId() << endl;
    cout << "时间：" << e->getTime() << endl;
    cout << "地点：" << e->getLocation() << endl;
    cout << "内容：" << e->getContent() << endl;
    cout << "报酬：" << e->getProfit() << endl;

    cout << "\n----- 评论区 -----\n";
    for (auto &c : e->getComments())
        cout << "- " << c << endl;
}

/* ================== 主函数 ================== */
int main()
{
    server srv;
    // 读档
    srv.loadFromFile();

    //  预设管理员（只在第一次）
    if (srv.getAllUsers().empty())
    {
        srv.creat_administrator_account(
            "管理员",
            "男",
            "admin",
            "admin",
            "admin@qq.com");
    }

    Page page = Page::LOGIN;        // 最开始的登录界面
    client *current_user = nullptr; // 登录用户
    const entrustment *current = nullptr;

    int loginCursor = 0; // 0=用户登录 1=管理员登录 2=注册 3=退出
    int cursor = 0;      // 光标
    string phone, password;

    while (true)
    {
        clear();

        /* ================== 登录界面 ================== */
        if (page == Page::LOGIN)
        {
            cout << "====== 帮个忙 ======\n\n";
            // 菜单页面
            const char *menu[] = {
                "普通用户登录",
                "管理员登录",
                "注册",
                "退出"};

            for (int i = 0; i < 3; ++i)
            {
                cout << (i == loginCursor ? "> " : "  ")
                     << menu[i] << endl;
            }

            cout << "\nW/S 选择  Enter 确认\n";
        }

        /* ================== 注册界面 ================== */
        else if (page == Page::REGISTER)
        {
            string name, gender, email;
            cout << "====== 注册 ======\n\n";
            cout << "姓名：";
            cin >> name;
            cout << "性别：";
            cin >> gender;
            cout << "手机号：";
            cin >> phone;
            cout << "密码：";
            cin >> password;
            cout << "邮箱：";
            cin >> email;

            srv.register_client(name, gender, phone, password, email);
            cout << "\n注册成功，按任意键返回登录...";
            cout << "\n当前注册用户数量: " << srv.getAllUsers().size();
            page = Page::LOGIN;
        }

        /* ================== 委托列表 ================== */
        else if (page == Page::ENTRUST_LIST)
        {
            cout << "【委托列表】\n\n";
            int idx = 0; // 光标
            for (auto &p : srv.getAllEntrustments())
            {
                cout << (idx == cursor ? "> " : "  ")
                     << p.second.getName() << endl;
                idx++;
            }

            cout << "\nA/D 切页  W/S 移动  Enter 查看  ESC 退出\n";
        }

        /* ================== 我发布 ================== */
        else if (page == Page::MY_DISPATCH)
        {
            cout << "【我发布的委托】\n\n";
            const auto &list = current_user->getDispatchHistory();
            for (int i = 0; i < (int)list.size(); ++i)
                cout << (i == cursor ? "> " : "  ")
                     << list[i].getName() << endl;

            cout << "\nA/D 切页  W/S 移动  [R] 删除委托  Enter 查看  ESC 返回\n";
        }

        /* ================== 我承接 ================== */
        else if (page == Page::MY_ACCEPT)
        {
            cout << "【我承接的委托】\n\n";
            const auto &list = current_user->getAcceptHistory();
            for (int i = 0; i < (int)list.size(); ++i)
                cout << (i == cursor ? "> " : "  ")
                     << list[i].getName() << endl;

            cout << "\nA/D 切页  W/S 移动  Enter 查看  ESC 退出\n";
        }

        /* ================== 个人信息 ================== */
        else if (page == Page::USER_INFO)
        {
            cout << "【个人信息】\n\n";
            cout << "姓名：" << current_user->getName() << endl;
            cout << "性别：" << current_user->getGender() << endl;
            cout << "手机：" << current_user->getPhone() << endl;
            cout << "邮箱：" << current_user->getEmail() << endl;
            cout << "星级：" << current_user->getStar()
                 << "（" << current_user->getStarLevel() << "）\n";
            cout << "信誉分：" << current_user->getCreait() << endl;

            cout << "\n[1] 修改个人信息  [Enter] 发布委托  [ESC] 返回\n";
        }

        /* ================== 编辑个人信息 ================== */
        else if (page == Page::USER_EDIT)
        {
            clear();
            cout << "【修改个人信息】\n\n";
            cout << "1. 姓名\n";
            cout << "2. 性别\n";
            cout << "3. 手机\n";
            cout << "4. 邮箱\n";
            cout << "5. 密码\n";
            cout << "ESC 返回\n\n";

            char ch = _getch();
            string v;

            if (ch == '1')
            {
                cout << "\n新姓名：";
                cin >> v;
                current_user->setName(v);
            }
            else if (ch == '2')
            {
                cout << "\n新性别：";
                cin >> v;
                current_user->setGender(v);
            }
            else if (ch == '3')
            {
                cout << "\n新手机号：";
                cin >> v;
                current_user->setPhone(v);
            }
            else if (ch == '4')
            {
                cout << "\n新邮箱：";
                cin >> v;
                current_user->setEmail(v);
            }

            else if (ch == '5')
            {
                cout << "\n原密码: ";
                cin >> v;
                if (v != current_user->getPassword())
                {
                    cout << "\n密码错误，按任意键返回...";
                    _getch();
                    page = Page::USER_INFO;
                    continue;
                }
            }
            else if (ch == 27)
            {
                page = Page::USER_INFO;
                continue;
            }

            cout << "\n修改成功，按任意键返回...";
            _getch();
            page = Page::USER_INFO;
        }

        /* ================== 发布委托 ================== */
        else if (page == Page::USER_PUBLISH)
        {
            string n, t, l, c;
            double p;
            cout << "【发布委托】\n\n";

            cout << "输入 q 可取消发布\n\n";

            cout << "标题：";
            cin >> n;
            if (n == "q" || n == "Q")
            {
                page = Page::USER_INFO;
                continue;
            }
            cout << "时间：";
            cin >> t;
            if (n == "q" || n == "Q")
            {
                page = Page::USER_INFO;
                continue;
            }
            cout << "地点：";
            cin >> l;
            if (n == "q" || n == "Q")
            {
                page = Page::USER_INFO;
                continue;
            }
            cout << "内容：";
            cin >> c;
            if (n == "q" || n == "Q")
            {
                page = Page::USER_INFO;
                continue;
            }
            cout << "报酬：";
            cin >> p;
            if (n == "q" || n == "Q")
            {
                page = Page::USER_INFO;
                continue;
            }

            srv.dispatch(current_user->getID(), n, t, l, c, p);

            cout << "\n发布成功，按任意键返回...";
            _getch();
            page = Page::MY_DISPATCH; // 切换页面
        }

        /* ================== 委托详情 ================== */
        else if (page == Page::ENTRUST_DETAIL && current)
        {
            drawEntrustDetail(current);
            cout << "\n[Enter] 接受委托  [C] 评论  [ESC] 返回\n";
        }

        /*=================承接委托详情页=================*/
        else if (page == Page::ACCEPT_DETAIL && current)
        {
            drawEntrustDetail(current);
            cout << "\n[1] 完成委托";
            cout << "\n[2] 委托失败";
            cout << "\n[ESC] 返回\n";
        }

        /* ================== 键盘处理 ================== */
        char ch = _getch();

        if (page == Page::ENTRUST_DETAIL && (ch == 'c' || ch == 'C'))
        {
            string comment;

            cout << "\n输入评论: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, comment);

            if (!comment.empty())
            {
                entrustment *e = srv.find_entrustment(current->getId());
                if (e)
                {
                    e->addComment(current_user->getName() + ": " + comment);
                }
            }
        }

        if (page == Page::ACCEPT_DETAIL)
        {
            if (ch == '1')
            {
                srv.complete_event(current_user->getID(), current->getId());
                page = Page::MY_ACCEPT;
                cursor = 0;
            }
            else if (ch == '2')
            {
                srv.finish_fail_event(current_user->getID(), current->getId());
                page = Page::MY_ACCEPT;
                cursor = 0;
            }
        }

        if (page == Page::MY_DISPATCH && (ch == 'r' || ch == 'R'))
        {
            auto &list = current_user->accessDispatchHistory();
            if (cursor < list.size())
            {
                bool ok = srv.cancel_dispatch(
                    current_user->getID(),
                    list[cursor].getId());

                if (!ok)
                {
                    cout << "\n只能删除自己发布的委托!";
                    _getch();
                }

                cursor = 0;
            }
        }

        if (page == Page::LOGIN)
        {
            if (ch == 'w')
                loginCursor = max(0, loginCursor - 1);
            if (ch == 's')
                loginCursor = min(2, loginCursor + 1);
        }

        if (ch == '1' && page == Page::USER_INFO)
        {
            page = Page::USER_EDIT;
        }

        if (ch == 27) // ESC
        {
            if (page == Page::ENTRUST_DETAIL)
                page = Page::ENTRUST_LIST;
            else if (page == Page::USER_PUBLISH)
                page = Page::USER_INFO;
            else
                break;
        }

        if (ch == 'w')
            cursor = max(0, cursor - 1);

        if (ch == 's')
            cursor++;

        auto it = find(tabPages.begin(), tabPages.end(), page);
        if (it != tabPages.end())
        {
            int idx = it - tabPages.begin();

            if (ch == 'a')
            {
                idx = (idx - 1 + tabPages.size()) % tabPages.size();
                page = tabPages[idx];
                cursor = 0;
            }
            else if (ch == 'd')
            {
                idx = (idx + 1) % tabPages.size();
                page = tabPages[idx];
                cursor = 0;
            }
        }

        if (ch == 13) // Enter
        {
            if (page == Page::LOGIN)
            {
                if (loginCursor == 0) // 用户登录
                {
                    clear();
                    cout << "====== 登录 ======\n\n";
                    cout << "手机号：";
                    cin >> phone;
                    cout << "密码：";
                    cin >> password;

                    current_user = srv.login(phone, password);
                    if (current_user)
                    {
                        cout << endl
                             << "登录成功！按任意键继续...";
                        page = Page::ENTRUST_LIST;
                        cursor = 0;
                    }
                    else
                    {
                        cout << "\n登录失败，按任意键返回...";
                        _getch();
                    }
                }
                else if (loginCursor == 1) // 管理员登录
                {
                    clear();
                    cout << "====== 管理员登录 ======\n\n";
                    cout << "手机号：";
                    cin >> phone;
                    cout << "密码：";
                    cin >> password;

                    current_user = srv.login(phone, password);
                    if (current_user && current_user->isAdmin())
                    {
                        cout << "\n管理员登录成功!";
                        _getch();
                        page = Page::ENTRUST_LIST;
                        cursor = 0;
                    }
                    else
                    {
                        cout << "\n管理员账号或密码错误!";
                        _getch();
                    }
                }
                else if (loginCursor == 2) // 注册
                {
                    page = Page::REGISTER;
                }
                else // 退出
                {
                    return 0;
                }
            }

            else if (page == Page::ENTRUST_LIST)
            {
                int idx = 0;
                for (auto &p : srv.getAllEntrustments())
                {
                    if (idx == cursor)
                    {
                        current = &p.second;
                        page = Page::ENTRUST_DETAIL;
                        break;
                    }
                    idx++;
                }
            }
            else if (page == Page::ENTRUST_DETAIL && current)
            {
                if (srv.accept_clint(current_user->getID(), current->getId()) == 1)
                    cout << "\n接受成功, 按任意键继续...";
                else if (srv.accept_clint(current_user->getID(), current->getId()) == 0)
                    cout << "\n错误! 委托不存在, 按任意键继续...";
                else if (srv.accept_clint(current_user->getID(), current->getId()) == -1)
                    cout << "\n您的信誉分过低, 暂时无法接受新委托, 按任意键继续...";
                _getch();
                page = Page::MY_ACCEPT;
                cursor = 0;
            }

            else if (page == Page::USER_INFO)
            {
                page = Page::USER_PUBLISH;
            }

            else if (page == Page::USER_INFO)
            {
                page = Page::USER_PUBLISH;
            }

            else if (page == Page::MY_ACCEPT)
            {
                const auto &list = current_user->getAcceptHistory();
                if (cursor < list.size())
                {
                    current = &list[cursor];
                    page = Page::ACCEPT_DETAIL;
                }
            }
        }
    }

    srv.saveToFile();

    return 0;
}
