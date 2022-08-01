import csv
import ast
from time import localtime
from click import UsageError


def save_to_csv(data):
    with open("data.csv", "w", encoding='utf-8') as file:
        names = ['page', 'news']
        writer = csv.DictWriter(file, fieldnames=names)
        writer.writeheader()
        for page, news in data.items():
            writer.writerow({'page': page, 'news': news})


def get_csv():
    try:
        with open(f"data.csv", "r") as file:
            reader = csv.DictReader(file)
            data = {}
            for item in reader:
                data[item['page']] = ast.literal_eval(item['news'])
            return data
    except:
        raise UsageError(f'No such file: "data.csv" or use "--format" to choose another file format')


def create_metadata(start, end, pages: int, notes: int):
    with open("metadata.csv", "w", encoding='utf-8') as file:
        names = ['start_time', 'end_time', 'working_time', 'pages', 'notes']
        writer = csv.DictWriter(file, fieldnames=names)
        writer.writeheader()
        st = localtime(start)
        en = localtime(end)
        writer.writerow({'start_time': f'{st.tm_hour:02}:{st.tm_min:02}:{st.tm_sec:02}',
                         'end_time': f'{en.tm_hour:02}:{en.tm_min:02}:{en.tm_sec:02}',
                         'working_time': f'{round(end - start, 3)} seconds',
                         'pages': pages,
                         'notes': notes
                         })
