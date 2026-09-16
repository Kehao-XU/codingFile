#include <iostream>
#include <vector>
#include <list>

int main() {
    std ::vector<int> numbers = {1, 2, 3, 4, 5};
    numbers.push_back(6);
    numbers.at(2) = 30;
    for (const int i : numbers) {
        std ::cout << i << " ";
    }
    std::cout << '\n';

    std::list<std::string> names = {"Alice", "Bob", "Charlie"};
    names.push_back("David");
    return 0;
}