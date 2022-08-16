//!	Author: Aden An Chen
//!	Date:   3.6.18
/**
*	@description :	functions for data input and output
*/
#pragma once
#include <iostream>
#include <cstdio>
#include <fstream>

#include <string>
#include <map>

#include <windows.h>



namespace data
{
    struct _teacher
    {
        /**
        *   @description:   structure for teacher info
        */
        std::string name;
        int *course;
        // {available_course_num, course_id1, course2, etc.}
        bool *date;
        // {day1.is_available, day2.is_available, etc.}
    };


    struct _course
    {
        /**
        *   @description:   structure for course info
        */
        std::string name;
        int num;
            
        int set_time = 0;
        bool settle = false;
        int	**teacher_available;
        // teacher_available[day_num] = {
        //	available_teacher_num_on_day, 
        //	availeble_teacher_index1, available_teacher_index2, etc.
        // }
        bool *period_available;
        // period_available[period_index] = period is available
    };


    struct t_course
    {
        /**
        *   @description:	cell for time_table
        */
        int course_id = -1;
    };
    // constant
    std::string new_filename = "class_manager_temp";
    std::string error_filename = "class_manager_error_temp";
    const int c_max_num = 100;
    const int c_weekday_num = 5;
    const int c_max_teacher_on_same_course = 10;

    // information
    std::string teacher_info_file;
    std::string course_info_file;
    std::map    <std::string, int> get_course_id;
    _teacher    *teacher;
    _course     *course;
    int teacher_num = 0;
    int course_num = 0;
    int course_num_per_day;

    // result
    t_course    **time_table;


    void UTF8_to_Unicode(wchar_t* out, char *text)
    {
        char* uchar = (char *) out;
        uchar[1] = ((text[0] & 0x0F) << 4) + ((text[1] >> 2) & 0x0F);
        uchar[0] = ((text[1] & 0x03) << 6) + (text[2] & 0x3F);
    }


    void Unicode_to_GB2312(char* out, wchar_t data)
    {
        WideCharToMultiByte(CP_ACP, NULL, &data, 1, out, sizeof(wchar_t), NULL, NULL);
    }


    void _UTF8_to_GB2312(char *out, char *text)
    {
        int len = strlen(text);
        char Ctemp[4];
        memset(Ctemp, 0, 4);
        int i = 0, j = 0;
        while (i < len)
        {
            if (text[i] >= 0)
            {
                out[j++] = text[i++];
            }
            else
            {
                WCHAR Wtemp;
                UTF8_to_Unicode(&Wtemp, text + i);
                Unicode_to_GB2312(Ctemp, Wtemp);
                out[j] = Ctemp[0];
                out[j + 1] = Ctemp[1];
                i += 3;
                j += 2;
            }
        }
        out[j] = '\0';
        return;
    }


    void UTF8_to_GB2312(char *text)
    {
        char *out = new char[strlen(text)];
        _UTF8_to_GB2312(out, text);
        strcpy_s(text, strlen(out) + 1, out);
    }


    void data_init(void)
    {
        /**
        *   @description:   initialize array for teacher and course info
        */
        // teacher
        teacher = new _teacher[c_max_num];
        for (int i = 1; i < c_max_num; ++i)
        {
            teacher[i].course = new int[c_max_num];
            teacher[i].date = new bool[10];
            for (int j = 0; j < c_max_num; ++j)
            {
                teacher[i].course[j] = 0;
            }
            for (int j = 0; j < 10; ++j)
            {
                teacher[i].date[j] = false;
            }
        }

        // course
        course = new _course[c_max_num];
        for (int course_index = 1; course_index < c_max_num; ++course_index)
        {
            // course_teacher_available
            course[course_index].teacher_available = new int *[c_weekday_num + 1];
            for (int day_index = 1; day_index <= c_weekday_num; ++day_index)
            {
                course[course_index].teacher_available[day_index] = new int[c_max_teacher_on_same_course];
                course[course_index].teacher_available[day_index][0] = 0;
            }
            // course.period_available
            course[course_index].period_available = new bool [course_num_per_day + 1];
            for (int period_index = 1; period_index <= course_num_per_day; ++period_index)
            {
                // set to unavailable for every period for initialization
                course[course_index].period_available[period_index] = false;
            }
        }
    }


    bool time_table_valid(void)
    {
        /**
        *	@description :	to check if it is valid to create a time table
        *
        *   @return : time table is valid
        *
        *	@pre :	read course and teacher info
        */
        int input_course_num = 0;
        char error_msg[100];
        for (int i = 1; i <= course_num; ++i)
        {
            input_course_num += course[i].num;
        }
        if (input_course_num != course_num_per_day * c_weekday_num)
        {
            // course_num error
            sprintf_s(error_msg, 100, "[-]课程数据的总课程数量(%d)与应有的总课程数量(%d)不同",
                input_course_num, course_num_per_day*c_weekday_num);
            throw error_msg;
            return false;
        }
        return true;
    }


    void dfs_init(void)
    {
        /**
        *   @description: initialize for dfs
        *
        *	@pre :	read course and teacher info
        */
        // time table
        if (!time_table_valid())
        {
            return;
        }
        time_table = new t_course *[c_weekday_num + 1];
        for (int i = 1; i <= c_weekday_num; ++i)
        {
            time_table[i] = new t_course[course_num_per_day + 1];
        }
    }


    bool course_teachable(int teacher_index, int course_id)
    {
        /**
        *	@description:	get if a course is teachable for a teacher
        *
        *	@pre :	read teacher info
        */
        for (int i = 1; i <= teacher[teacher_index].course[0]; ++i)
        {
            if (teacher[teacher_index].course[i] == course_id)
            {
                return true;
            }
        }
        return false;
    }


    void read_teacher_info(std::fstream &file)
    {
        /**
        *	@description :	read teacher info
        *
        *   @param file :	teacher info file (.xls)
        *                   file:
        *                       teacher_name; class_available; day_available
        *
        *	@pre :		must have input.exe
        *
        *	@warning :	suffix of teacher info file must be ".xls"
        */
        // loop
        int teacher_index = 1;
        int course_id_index = 1; // id for course

        // temp
        std::string course_name;
        int day_num;
        char day;
        while (!file.eof())
        {
            // name
            file >> teacher[teacher_index].name;

            // available course
            file >> teacher[teacher_index].course[0];
            for (int i = 1; i <= teacher[teacher_index].course[0]; ++i)
            {
                file >> course_name;
                if (get_course_id.find(course_name) == get_course_id.end())
                {
                    char error_msg[100];
                    char error_course_name[100];
                    strcpy_s(error_course_name, course_name.length() + 1, course_name.c_str());
                    UTF8_to_GB2312(error_course_name);
                    sprintf_s(error_msg, "课程(%s)不在课程文件中", error_course_name);
                    throw error_msg;
                }
                teacher[teacher_index].course[i] = get_course_id.find(course_name)->second;
            }
            // available day
            file >> day_num;
            file.get();
            for (int i = 1; i <= day_num; ++i)
            {
                file.get(day);
                teacher[teacher_index].date[day - '0'] = true;

                // course[course_id].teacher_availeble
                int course_id;
                int next_teacher_index;
                for (int j = 1; j <= teacher[teacher_index].course[0]; ++j)
                {
                    course_id = teacher[teacher_index].course[j];
                    ++course[course_id].teacher_available[day - '0'][0];
                    next_teacher_index = course[course_id].teacher_available[day - '0'][0];
                    course[course_id].teacher_available[day - '0'][next_teacher_index] = teacher_index;
                }
            }
            teacher_index++;

        }
        teacher_num = teacher_index - 2;
        file.close();
        remove(("D:\\" + new_filename + "." + new_filename).c_str());
    }


    void read_course_info(std::fstream &file)
    {
        /**
        *	@description :	read course info
        *
        *   @param file :   course info file (.xls)
        *                   file:
        *                       course_name; course_number
        *
        *	@pre :		must have input.exe
        *
        *	@warning :	suffix of course info file must be ".xls"
        */
        int course_index = 1;
        int available_period_num;
        char available_period;
        std::string course_name;
        while (!file.eof())
        {
            // course.name
            file >> course_name;
            course[course_index].name = course_name;

            // course.num
            file >> course[course_index].num;
            if (file.fail())
            {
                // to avoid enter at back of file
                file.clear();
                break;
            }

            // add a mapping of course.name and course_index
            get_course_id[course_name] = course_index;

            // course.period_available
            file >> available_period_num;
            for (int i = 1; i <= available_period_num; ++i)
            {
                // for each available period
                file >> available_period;
                course[course_index].period_available[available_period - '0'] = true;
            }

            ++course_index;
        }
        course_num = course_index - 1;
        file.close();
        remove(("D:\\" + new_filename + "." + new_filename).c_str());
    }


    bool read(bool teacher_info)
    {
        /**
        *   @description :
        *       read teacher and course info to memory
        *       success ->  read it to memory; delete temp file
        *       fail    ->  output error massage
        *
        *   @param teacher_info:    true    ->  read teacher info
        *                           false   ->  read course info
        *
        *   @return :               true    ->  read successfully
        *                           false   ->  transfer fail / transfer time > max_time
        */
        const int max_time = 5000;
        const int sleep_time = 500;
        for (int i = 1; i <= max_time / sleep_time; ++i)
        {
            std::fstream file;
            file.open(("D:\\" + new_filename + "." + new_filename).c_str(), std::ios::in);
            if (file)
            {
                // when exist
                if (teacher_info)
                {
                    read_teacher_info(file);
                }
                else
                {
                    read_course_info(file);
                }
                return true;
            }
            file.close();
            file.open(("D:\\" + error_filename + "." + error_filename).c_str(), std::ios::in);
            if (file)
            {
                //when error
                std::string error_msg;
                std::getline(file, error_msg);
                file.close();
                remove(("D:\\" + error_filename + "." + error_filename).c_str());
                throw error_msg.c_str();
                return false;
            }
            file.close();
            Sleep(sleep_time);
        }
        return false;
    }


    void log_data(void)
    {
        /**
        *	@description :	log teacher data
        *
        *	@pre :	read teacher and course info
        */
        std::fstream file;
        file.open("D:\\teacher.txt", std::ios::out);
        file << teacher_num << "\n";
        for (int i = 1; i <= teacher_num; ++i)
        {
            file << teacher[i].name << " ";
            file << teacher[i].course[0] << " ";
            for (int j = 1; j <= teacher[i].course[0]; ++j)
                file << teacher[i].course[j] << " ";
            file << "\t";
            for (int j = 1; j < 10; ++j)
                file << teacher[i].date[j];
            file << "\n";
        }
        file.close();

        // log course data
        file.open("D:\\course.txt", std::ios::out);
        file << course_num << "\n";
        for (int i = 1; i <= course_num; ++i)
        {
            file << course[i].name << " " << course[i].num << "\n";
            for (int day_index = 1; day_index <= c_weekday_num; ++day_index)
            {
                file << day_index << ": ";
                for (int j = 1; j <= course[i].teacher_available[day_index][0]; ++j)
                {
                    file << course[i].teacher_available[day_index][j] << " ";
                }
                file << "\n";
            }
            for (int j = 1; j <= course_num_per_day; ++j)
            {
                file << course[i].period_available[j] << " ";
            }
            file << "\n";
        }
        file.close();
    }


    void get_info(void)
    {
        /**
        *   @description :
        *       read teaceher and course info
        *
        *	@pre :
        *		initialized teacher_info_filename, course_info_filename, course_num_per_day
        */
        data_init();
        system((".\\input.exe \"" + course_info_file + "\" -c").c_str());
        if (!read(false))
        {
            throw "读入学生信息失败";
        }
        system((".\\input.exe \"" + teacher_info_file + "\" -t").c_str());
        if (!read(true))
        {
            throw "读入老师信息失败";
        }
        dfs_init();
    }


    void time_table_output(std::ostream &out)
    {
        /**
        *	@description :	output time table
        *
        *	@pre : initialized time table
        */
        out << course_num_per_day << "\n";

        int course_id;
        std::string course_name;
        for (int i = 1; i <= data::c_weekday_num; ++i)
        {
            for (int j = 1; j <= data::course_num_per_day; ++j)
            {
                course_id = time_table[i][j].course_id;
                course_name = course[course_id].name;
                for (int k = 0; k < int(course_name.length()); ++k)
                {
                    // change space to '\0' for python command "split()"
                    if (course_name[k] == ' ')
                    {
                        course_name[k] = '\0';
                    }
                }
                out << course_name << " ";
            }
            out << "\n";
        }
    }


    void output_time_table_to_console(void)
    {
        /**
        *	@description :	output time table to console(in GB2312)
        *
        *	@pre : initialized time table
        */
        int course_id;
        int max_output_string_length = -0x7fffffff;
        std::string course_name;
        char gb2312_course_name[100];
        for (int i = 1; i <= data::c_weekday_num; ++i)
        {
            // get max output string length
            for (int j = 1; j <= data::course_num_per_day; ++j)
            {
                course_id = time_table[i][j].course_id;
                course_name = course[course_id].name;
                if (int(course_name.length()) > max_output_string_length)
                {
                    max_output_string_length = course_name.length();
                }
            }
        }

        std::cout << "  星期一  星期二  星期三  星期四  星期五\n";
        for (int i = 1; i <= data::course_num_per_day; ++i)
        {
            // output time table
            std::cout << i << " ";
            for (int j = 1; j <= data::c_weekday_num; ++j)
            {
                course_id = time_table[j][i].course_id;
                course_name = course[course_id].name;
                strcpy_s(gb2312_course_name, course_name.length() + 1, course_name.c_str());
                UTF8_to_GB2312(gb2312_course_name);
                std::cout << gb2312_course_name << "    ";
            }
            std::cout << "\n";
        }
    }


    void output_time_table_to_xls(std::string text_filename, std::string xls_filename)
    {
        /**
        *	@description :	write time table from text to excel file (".xls")
        *
        *	@pre :	output time table to temp file (".txt")
        *
        *	@param text_filename:
        *		filename of temp text file(".txt")
        *
        *	@param xls_filename:
        *		filename of excel filename to write tile table
        */
        system((".\\output.exe \"" + text_filename + "\" \"" + xls_filename + "\"").c_str());
    }
}
