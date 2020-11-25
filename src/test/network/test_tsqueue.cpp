#include <engine/network/net_tsqueue.h>
#include <iostream>

int main()
{
    samui::net::tsqueue<int> q;

    q.push_back(3);
    std::cout << "push back 3\n";
    std::cout << q.front() << "\n";

    q.push_back(4);
    std::cout << "push back 4\n";

    q.push_front(2);
    std::cout << "push front 2\n";

    std::cout << "pop front all\n";
    while(q.count() > 0)
    {
        std::cout << q.pop_front() << ", ";
    }
    std::cout << "\n";

    q.push_back(1);
    std::cout << "push back 1\n";
    std::cout << q.back() << "\n";

    q.push_back(2);
    std::cout << "push back 2\n";

    std::cout << "pop back\n";
    std::cout << q.pop_back() << "\n";

    std::cout << "empty?: " << q.empty() << "\n";

    q.clear();
    std::cout << "clear\n";
    std::cout << "empty?: " << q.empty() << "\n";

    return 0;
}