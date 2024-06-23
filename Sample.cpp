/**
 * @file Sample.cpp
 * @author  (avelstreet-0301@outlook.jp)
 * @brief 釣銭計算
 * @version 0.1
 * @date 2024/06/23 16:06:86
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "Sample.hpp"

void ChangeCalculator::read_csv(const std::string &filename)
{
    try
    {
        std::ifstream ifs_csv(filename);
        std::string line_str;

        while (getline(ifs_csv, line_str))
        {
            std::istringstream ifs_line(line_str);
            std::string cnm_str;
            int switch_cnt = 0;

            int key;
            int value;

            while (getline(ifs_line, cnm_str, ','))
            {
                switch (switch_cnt)
                {
                case 0:
                    key = std::stoi(cnm_str);
                    break;
                case 1:
                    value = std::stoi(cnm_str);
                    break;

                default:
                    break;
                }

                switch_cnt++;
            }

            money_map.insert({key, value});
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error : " << e.what() << "\n";
        std::exit(1);
    }
}

void ChangeCalculator::write_csv(const std::string &filename)
{
    try
    {
        std::ofstream ofs_csv(filename);

        for (const auto &element : money_map)
        {
            ofs_csv << element.first << ',' << element.second << '\n';
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error : " << e.what() << "\n";
        write_csv("error_temp.csv");
        std::cout << "  Data output to 'error_temp.csv'.\n";
    }
}

void ChangeCalculator::return_change()
{
    std::cout << "Return the change.\n";
    auto change_map = calculate_change();

    for (const auto &element : change_map)
    {
        std::cout << element.first << " -> " << element.second << "\n";
        money_map.at(element.first) -= element.second;
    }

    write_csv("sample.csv");
}

MoneyMap ChangeCalculator::calculate_change()
{
    MoneyMap change_map;
    int map_scale = money_map.size();
    int key_step[map_scale];

    try
    {
        int temp_change = money_buffer;

        { /* マップのキーを取得 */
            int cnt = 1;
            for (const auto &element : money_map)
            {
                key_step[(map_scale - cnt)] = element.first;
                cnt++;
            }
        }

        for (int cnt = 0; cnt < map_scale; cnt++)
        {
            auto rating_target = key_step[cnt];
            for (int money_cnt = 0; money_cnt <= money_map.at(rating_target); money_cnt++)
            {
                if (temp_change < rating_target || money_cnt == money_map.at(rating_target))
                {
                    change_map.insert({rating_target, money_cnt});
                    break;
                }
                else
                {
                    temp_change -= rating_target;
                }
            }
        }

        if (temp_change > 0)
        {
            std::cout << "釣銭切れ\n";
        }
        else if (temp_change < 0)
        {
            throw "It fell to a negative value.";
        }
        else
        { /* do nothing */
        }
    }
    catch (const char *error)
    {
        std::cerr << "Error : " << error << "\n";
        std::exit(1);
    }

    for (const auto &element : key_step)
    {
        try
        {
            if (change_map.at(element) == 0)
            {
                change_map.erase(element);
            }
        }
        catch (const std::out_of_range)
        { /* do nothing */
        }
    }

    return change_map;
}

int main()
{
    ChangeCalculator machine;

    machine.set_money_buffer(100);
    machine.return_change();

    std::cout << std::flush;
}