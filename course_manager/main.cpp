//!	Author: Aden An Chen
//!	Date:   3.6.5
#include "data_io.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <cstdlib>



bool generated_time_table = false;
const std::string text_time_table_filename = "D:\\class_manager_temp.class_manager_temp";
const std::string error_filename = "D:\\class_manager_error_temp.class_manager_error_temp";
std::string time_table_filename = "D:\\class_manager_temp.xls";


void dfs(int pos)
{
    /**
    *   @param pos:
    *       position filling
    */
    //for (int i = 0; i < pos; ++i)putchar(' ');printf("start %d\n", pos);
    int course_period = (pos / data::c_weekday_num) + 1;
    int day_index = (pos % data::c_weekday_num) + 1;

    if (pos == data::course_num_per_day * data::c_weekday_num)
    {
        // last level
        generated_time_table = true;
        return;
    }

    for (int course_id = 1; course_id <= data::course_num; ++course_id)
    {
        // for each course
        if (course_period > 1 && course_id == data::time_table[day_index][course_period - 1].course_id)
        {
            continue;
        }
        if (data::course[course_id].teacher_available[0] == 0)
        {
            // when there is no teacher teaching
            continue;
        }

        if ((!data::course[course_id].settle) && (data::course[course_id].period_available[course_period]))
        {
            // if course[course_id] not settle
            for (int teacher_list_index = 1;
                teacher_list_index <= data::course[course_id].teacher_available[day_index][0];
                ++teacher_list_index)
            {
                // for each teacher
                ++data::course[course_id].set_time;
                if (data::course[course_id].set_time == data::course[course_id].num)
                {
                    // when all course[course_id] settle
                    data::course[course_id].settle = true;
                }

                // start dfs of next level
                data::time_table[day_index][course_period].course_id = course_id;
                dfs(pos + 1);
                if (generated_time_table)
                {
                    // when time table generated in the last dfs
                    return;
                }
                --data::course[course_id].set_time;
                if (data::course[course_id].set_time != data::course[course_id].num)
                {
                    // when course[course_id] not set (after changing in  next dfs)
                    data::course[course_id].settle = false;
                }
            }
        }
    }
    return;
}


int main(int argc, char * argv[])
{
    try
    {
        // argv:	teacher_info_filename
        //			course_info_filename
        //			course_num_per_day
        //			time_table_filename

        data::teacher_info_file = argv[1];
        data::course_info_file = argv[2];
        data::course_num_per_day = atoi(argv[3]);
        time_table_filename = argv[4];
        data::get_info();
        std::cout << "��ʼ���ɿγ̱������ĵȴ�\n";
        time_t start_time = clock();
        dfs(0);

        std::ofstream file;
        file.open(text_time_table_filename);
        data::time_table_output(file);
        file.close();
        data::output_time_table_to_xls(text_time_table_filename, time_table_filename);
        if (!generated_time_table)
        {
            throw "�޷����ɿγ̱��������������\n";
        }
        const int max_time = 1500;
        const int sleep_time = 500;
        for (int i = 1; i <= max_time / sleep_time; ++i)
        {
            // output error
            std::ifstream file(error_filename);
            if (file)
            {
                std::string error_msg_str;
                std::getline(file, error_msg_str);
                char error_msg_char[100];
                strcpy_s(error_msg_char, error_msg_str.length() + 1, error_msg_str.c_str());
                data::UTF8_to_GB2312(error_msg_char);
                file.close();
                remove(error_filename.c_str());
                throw error_msg_char;
            }
            file.close();
        }

        time_t end_time = clock();
        std::cout << "��ʱ:" << (end_time - start_time)*1.0/CLOCKS_PER_SEC << "sec\n";
        std::cout << "�γ̱����ɳɹ�" << "\n";
        data::output_time_table_to_console();
        std::cout << "���»س�����";
        std::cin.get();
    }
    catch (const char* error_msg)
    {
        std::cout << error_msg << "\n";
        std::cout << "���»س�����";
        std::cin.get();
    }
    return 0;
}
