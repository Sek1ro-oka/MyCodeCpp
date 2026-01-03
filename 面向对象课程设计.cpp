#include <iostream>
#include <conio.h>
#include <windows.h>
#include "server.h"

using namespace std;

/* ================== 页面枚举 ================== */
enum class Page
{
    LOGIN,
    REGISTER,
    ENTRUST_LIST,
    MY_DISPATCH,
    MY_ACCEPT,
    USER_INFO,
    USER_PUBLISH,
    ENTRUST_DETAIL
};

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

    // 预置一个测试用户
    srv.register_client("张三", "男", "123", "123", "a@qq.com");

    Page page = Page::LOGIN;
    client *current_user = nullptr;
    const entrustment *current = nullptr;

    int loginCursor = 0; // 0=登录 1=注册 2=退出
    int cursor = 0;
    string phone, password;

    while (true)
    {
        clear();

        /* ================== 登录界面 ================== */
        if (page == Page::LOGIN)
        {
            cout << "====== 欢迎使用委托系统 ======\n\n";

            const char *menu[] = {
                "登录",
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
            _getch();
            page = Page::LOGIN;
        }

        /* ================== 委托列表 ================== */
        else if (page == Page::ENTRUST_LIST)
        {
            cout << "【委托列表】\n\n";
            int idx = 0;
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
        }

        /* ================== 我承接 ================== */
        else if (page == Page::MY_ACCEPT)
        {
            cout << "【我承接的委托】\n\n";
            const auto &list = current_user->getAcceptHistory();
            for (int i = 0; i < (int)list.size(); ++i)
                cout << (i == cursor ? "> " : "  ")
                     << list[i].getName() << endl;
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

            cout << "\n[Enter] 发布委托  [ESC] 返回\n";
        }

        /* ================== 发布委托 ================== */
        else if (page == Page::USER_PUBLISH)
        {
            string n, t, l, c;
            double p;

            cout << "【发布委托】\n\n";
            cout << "标题：";
            cin >> n;
            cout << "时间：";
            cin >> t;
            cout << "地点：";
            cin >> l;
            cout << "内容：";
            cin >> c;
            cout << "报酬：";
            cin >> p;

            srv.dispatch(current_user->getID(), n, t, l, c, p);

            cout << "\n发布成功，按任意键返回...";
            _getch();
            page = Page::MY_DISPATCH;
        }

        /* ================== 委托详情 ================== */
        else if (page == Page::ENTRUST_DETAIL && current)
        {
            drawEntrustDetail(current);
            cout << "\n[Enter] 接受委托  [ESC] 返回\n";
        }

        /* ================== 键盘处理 ================== */
        char ch = _getch();
        if (page == Page::LOGIN)
        {
            if (ch == 'w')
                loginCursor = max(0, loginCursor - 1);
            if (ch == 's')
                loginCursor = min(2, loginCursor + 1);
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

        if (ch == 'a')
        {
            page = Page((int(page) + 6) % 7);
            cursor = 0;
        }

        if (ch == 'd')
        {
            page = Page((int(page) + 1) % 7);
            cursor = 0;
        }

        if (ch == 13) // Enter
        {
            if (page == Page::LOGIN)
            {
                if (loginCursor == 0) // 登录
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
                        page = Page::ENTRUST_LIST;
                        cursor = 0;
                    }
                    else
                    {
                        cout << "\n登录失败，按任意键返回...";
                        _getch();
                    }
                }
                else if (loginCursor == 1) // 注册
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
                srv.accept_clint(current_user->getID(), current->getId());
                page = Page::MY_ACCEPT;
                cursor = 0;
            }
            else if (page == Page::USER_INFO)
            {
                page = Page::USER_PUBLISH;
            }
        }
    }

    return 0;
}
