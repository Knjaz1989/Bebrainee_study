import requests
import sys
import time
from bs4 import BeautifulSoup
from file_working import save_data_to_file
from threading import Thread
from queue import Queue


def parsing(format, start_page=1, data=False):
    first_page = requests.get(f"https://moe-online.ru/news")
    first_soup = BeautifulSoup(first_page.text, 'html.parser')
    last_page = int(first_soup.find('ul', class_="pagination").text.strip().split('\n')[-1])
    if data:
        start_page = int(sorted(data)[-1]) + 1
    if start_page > last_page:
        print(f'Страницы с номером "{start_page}" не существует')
        sys.exit()
    sites_queue = Queue()
    data_queue = Queue()
    meta_data = []
    threads = []
    site_thread = Thread(target=get_site, args=(start_page, last_page, sites_queue))
    site_thread.start()
    threads.append(site_thread)
    data_threads = Thread(target=check_sites_queue, args=(sites_queue, data_queue))
    data_threads.start()
    threads.append(data_threads)
    write_thread = Thread(target=check_data_queue, args=(data_queue, data, format, meta_data))
    write_thread.start()
    threads.append(write_thread)

    for thread in threads:
        thread.join()

    return meta_data


def get_site(start_page, last_page, queue):
    for page in range(start_page, last_page + 1):
        response = requests.get(f"https://moe-online.ru/news?page={page}")
        if response.status_code != 200:
            queue.put(False)
            break
        queue.put(item=[str(page), response.text])
    else:
        queue.put(False)


def check_sites_queue(sites_queue, data_queue):
    while True:
        if sites_queue.empty():
            time.sleep(0.3)
            continue
        item = sites_queue.get()
        if not item:
            data_queue.put(item=False)
            break
        page, value = item
        tmp_list, tmp_dict = [], {}
        soup = BeautifulSoup(value, 'html.parser')
        news = soup.find_all('a', class_='plitka_text')
        for item_news in news:
            item_name = item_news.text.strip()
            tmp_list.append(item_name)
        tmp_dict[page] = tmp_list
        data_queue.put(item=tmp_dict)


def check_data_queue(data_queue, data, format, meta_data):
    data_dict = {}
    if data:
        data_dict.update(data)
    news_count = 0
    page_count = 0
    while True:
        if data_queue.empty():
            time.sleep(0.3)
            continue
        queue_item = data_queue.get()
        if not queue_item:
            meta_data.extend([page_count, news_count])
            break
        page_count += 1
        news_count += len(list(queue_item.values())[0])
        data_dict.update(queue_item)
    save_data_to_file(data_dict, format)
