# -*- coding:utf-8 -*-
# __Time__    : 2018/3/20 22:05
# __Author__  : Aden An Chen
import xlwt
import sys
from os import remove
error_filename = "class_manager_error_temp"
text_filename = sys.argv[1]
xls_filename = sys.argv[2]


def write_error(error_msg):
    with open(r"D:\{}.{}".format(error_filename, error_filename), "w") as error_file:
        error_file.write(error_msg)
    try:
        remove(text_filename)
    except WindowsError:
        pass


def output():
    if xls_filename[-4:] != ".xls":
        write_error("xls filename error")

    workbook = xlwt.Workbook(encoding="utf-8")
    sheet = workbook.add_sheet("Sheet1", cell_overwrite_ok=True)
    with open(text_filename, "r") as text_file:
        course_num_per_day = int(text_file.readline())
        index = 0
        for day in [u"一", u"二", u"三", u"四", u"五"]:
            index += 1
            sheet.write(0, index, u"星期" + day)
        for num in xrange(1, course_num_per_day + 1):
            sheet.write(num, 0, num)

        day_index = 0
        for line in text_file:
            day_index += 1
            course = line.split(" ")
            while "" in course:
                course.remove("")

            for i in xrange(1, len(course) + 1):
                course[i - 1].replace("\1", " ")
                sheet.write(i, day_index, course[i - 1])
    workbook.save(xls_filename)
    remove(text_filename)

try:
    output()
except BaseException, e:
    write_error(str(e))
