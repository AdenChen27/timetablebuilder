# __Time__    : 2018/3/5 19:35
# __Author__  : Aden An Chen
import xlrd
import sys
from os import remove

"""
teacher:
    parm: filename -t
    file format:
        "teacher_name" "class_available" "day_available"
        "aden" "math;cs" "135"("12345" == "0")

class_name:
    parm: filename -c
    file format:
        "class_name" "class_num"
"""
filename = sys.argv[1]
mode = sys.argv[2]
new_filename = "class_manager_temp"
error_filename = "class_manager_error_temp"


error = None
if mode == "-t":
    try:
        workbook = xlrd.open_workbook(filename)
        sheet = workbook.sheet_by_index(0)
        text = []

        for i in xrange(0, sheet.nrows):
            """
            teacher_name available_class_num available_class available_day
            aden 2 math cs 12345
            """
            available_class = sheet.cell(i, 1).value.split(";")
            available_day = str(int(sheet.cell(i, 2).value))
            if available_day == "0":
                available_day = "12345"
            temp_str = ""
            temp_str += "{} {} ".format(
                sheet.cell(i, 0).value.replace(" ", "").encode("utf-8").lower(),
                len(available_class)
            )
            for j in available_class:
                temp_str += "{} ".format(j.replace(" ", "").encode("utf-8").lower())
                temp_str += "{} {}\n".format(len(available_day), available_day)
            text.append((temp_str, len(available_day)))

        text.sort(key=lambda x: x[1])
        with open(r"D:\{}.{}".format(new_filename, new_filename), "w") as new_file:
            for string, day_num in text:
                new_file.write(string)
    except BaseException, e:
        error = e
else:
    try:
        workbook = xlrd.open_workbook(filename)
        sheet = workbook.sheet_by_index(0)
        text = []

        for i in xrange(0, sheet.nrows):
            """
            class_name class_num available_period
            cs 5 134
            """
            text.append(
                (
                    "{} {} {} {}\n".format(
                        sheet.cell(i, 0).value.replace(" ", "").encode("utf-8").lower(),
                        str(int(sheet.cell(i, 1).value)),
                        len(str(int(sheet.cell(i, 2).value))),
                        str(int(sheet.cell(i, 2).value))
                    ),
                    int(sheet.cell(i, 1).value)
                )
            )
        text.sort(key=lambda x: x[1])
        with open(r"D:\{}.{}".format(new_filename, new_filename), "w") as new_file:
            for string in text:
                new_file.write(string[0])
    except BaseException, e:
        error = e

if error:
    with open(r"D:\{}.{}".format(error_filename, error_filename), "w") as error_file:
        error_file.write(str(error))
    remove(r"D:\{}.{}".format(new_filename, new_filename))

