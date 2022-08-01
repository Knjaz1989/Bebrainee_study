import sys
import requests
from bs4 import BeautifulSoup


def get_data_from_site(start_page=1, data=False):
    first_page = requests.get(f"https://moe-online.ru/news")
    first_soup = BeautifulSoup(first_page.text, 'html.parser')
    last_page = int(first_soup.find('ul', class_="pagination").text.strip().split('\n')[-1])
    news_dict = {}
    if data:
        news_dict = data
        start_page = int(sorted(data)[-1]) + 1
    if start_page > last_page:
        print(f'Страницы с номером "{start_page}" не существует')
        sys.exit()
    page_count = 0
    note_count = 0
    for page in range(start_page, last_page + 1):
        tmp_list = []
        response = requests.get(f"https://moe-online.ru/news?page={page}")
        if response.status_code != 200:
            print(f'Страница с номером "{page}" не была прочитана. Используйте флаг "--proceed", чтобы продолжить с данной страницы')
            break
        soup = BeautifulSoup(response.text, 'html.parser')
        news = soup.find_all('a', class_='plitka_text')
        for item_news in news:
            item_name = item_news.text.strip()
            tmp_list.append(item_name)
            note_count += 1
        news_dict[str(page)] = tmp_list
        page_count += 1
    return news_dict, page_count, note_count
