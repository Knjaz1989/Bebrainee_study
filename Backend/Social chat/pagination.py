import math


class Paginator:

    def __init__(self, data: list, page_number: int, limit: int):
        self.__data = data
        self.__limit = limit
        self.__page_number = self.check_page(page_number)

    def pages_count(self):
        count = math.ceil(len(self.__data) / self.__limit)
        return count

    def check_page(self, page_number):
        count = self.pages_count()
        if page_number > count:
            return count
        return page_number

    def data(self):
        data = self.__data[(self.__page_number - 1) * self.__limit: self.__page_number * self.__limit]
        return data

    def current_page(self):
        return self.__page_number

    def has_prev(self):
        if self.__page_number < 2:
            return False
        return True

    def prev_page(self):
        if self.has_prev():
            return self.__page_number - 1
        return None

    def has_next(self):
        if self.__page_number > self.pages_count() - 1:
            return False
        return True

    def next_page(self):
        if self.has_next():
            return self.__page_number + 1
        return None
