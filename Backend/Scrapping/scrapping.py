import json
import click
import requests
import sys
from bs4 import BeautifulSoup
from time import sleep, time


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
            # item_link = item_news['href']
            tmp_list.append(item_name)
        news_dict[page] = tmp_list
    return news_dict


def get_data_from_json():
    with open("data.json", "r") as file:
        data = json.load(file)
        return data


def save_data_to_json(data):
    with open("data.json", "w", encoding="utf-8") as file:
        json.dump(data, file, indent=4, ensure_ascii=False)


@click.command()
@click.option("--start-page", type=click.IntRange(1), help="Scrapping from this page")
@click.option("--proceed", is_flag=True, help="Proceed scrapping from next to last page in data.json")
def program(start_page, proceed):
    start_time = time()
    if start_page and proceed:
        raise click.UsageError('Use only one option')
    elif proceed:
        data = get_data_from_site(data=get_data_from_json())
    else:
        if not start_page:
            start_page = 1
        data = get_data_from_site(start_page=start_page)
    save_data_to_json(data)
    print(f"--- {time() - start_time} seconds ---")


if __name__ == '__main__':
    program()

