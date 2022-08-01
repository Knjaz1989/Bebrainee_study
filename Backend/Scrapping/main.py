import click
from json_functions import save_to_json, get_json
from csv_functions import save_to_csv, get_csv, create_metadata
from xlsx_functions import save_to_xlsx, get_xlsx
from site_parsing import get_data_from_site
from time import time


def save_data_to_file(data, format):
    if format == 'json':
        save_to_json(data)
    elif format == 'csv':
        save_to_csv(data)
    else:
        save_to_xlsx(data)


def get_data_from_file(format):
    if format == 'json':
        data = get_json()
    elif format == 'csv':
        data = get_csv()
    else:
        data = get_xlsx()
    return data


@click.command()
@click.option("--start-page", type=click.IntRange(1), help="Scrapping from this page")
@click.option("--proceed", is_flag=True, help="Proceed scrapping from next to last page in data.json")
@click.option("--format", type=click.Choice(['json', 'csv', 'xlsx']), help='Can choose current file format. Default "json"')
def program(start_page, proceed, format):
    start_time = time()
    if not format:
        format = "json"
    if start_page and proceed:
        raise click.UsageError('Use only "--start-page" or "--proceed"')
    elif proceed:
        data = get_data_from_site(data=get_data_from_file(format))
    else:
        if not start_page:
            start_page = 1
        data = get_data_from_site(start_page=start_page)
    save_data_to_file(data[0], format)
    end_time = time()
    create_metadata(start_time, end_time, data[1], data[2])
    # print(f"--- {time() - start_time} seconds ---")


if __name__ == '__main__':
    program()
