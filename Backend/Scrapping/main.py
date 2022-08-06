import click
from csv_functions import create_metadata
from file_working import get_data_from_file
from site_parsing import parsing
from time import time


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
        data = parsing(format, data=get_data_from_file(format))
    else:
        if not start_page:
            start_page = 1
        data = parsing(format, start_page=start_page)
    end_time = time()
    create_metadata(start_time, end_time, data[0], data[1])


if __name__ == '__main__':
    program()
