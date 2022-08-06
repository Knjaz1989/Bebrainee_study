import csv
from click import UsageError
from time import localtime


def save_to_csv(data):
    with open("data.csv", "w", encoding='utf-8') as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow(['page', 'item_news'])
        for page, news in data.items():
            for item_news in news:
                writer.writerow([page, item_news])


def get_csv():
    try:
        with open(f"data.csv", "r") as file:
            reader = csv.reader(file, delimiter=';')
            data = {}
            next(reader) #Skip header
            for page, item_news in reader:
                data.setdefault(page, []).append(item_news)
            return data
    except:
        raise UsageError(f'No such file: "data.csv" or use "--format" to choose another file format')


def create_metadata(start, end, pages: int, notes: int):
    with open("metadata.csv", "w", encoding='utf-8') as file:
        names = ['start_time', 'end_time', 'working_time', 'pages', 'notes']
        writer = csv.DictWriter(file, fieldnames=names, delimiter=";")
        writer.writeheader()
        st = localtime(start)
        en = localtime(end)
        writer.writerow({'start_time': f'{st.tm_hour:02}:{st.tm_min:02}:{st.tm_sec:02}',
                         'end_time': f'{en.tm_hour:02}:{en.tm_min:02}:{en.tm_sec:02}',
                         'working_time': f'{round(end - start, 3)} seconds',
                         'pages': pages,
                         'notes': notes
                         })
