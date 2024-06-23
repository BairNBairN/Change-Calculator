/**
 * @file Sample.hpp
 * @author  (avelstreet-0301@outlook.jp)
 * @brief 釣銭計算
 * @version 0.1
 * @date 2024/06/23 16:06:79
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <string>
#include <map>

using MoneyMap = std::map<int, int>;

class ChangeCalculator
{
private:
    MoneyMap money_map;
    int money_buffer;

    static const int MAXIMUM_MONEY_INVENTORY = 100;

    const std::string filename = "sample.csv";

protected:
    MoneyMap calculate_change();
    void read_csv(const std::string &filename);
    void write_csv(const std::string &filename);

public:
    ChangeCalculator() : money_buffer(0) { read_csv(filename); }
    ~ChangeCalculator() { write_csv(filename); }

    void set_money_buffer(const int _money_buffer) { money_buffer = _money_buffer; }

    void return_change();
};