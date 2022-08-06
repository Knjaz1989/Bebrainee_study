from csv_functions import save_to_csv, get_csv
from json_functions import save_to_json, get_json
from xlsx_functions import save_to_xlsx, get_xlsx


def save_data_to_file(data, format):
    type_dict = {'json': save_to_json, 'csv': save_to_csv, 'xlsx': save_to_xlsx}
    type_dict[format](data)


def get_data_from_file(format):
    type_dict = {'json': get_json, 'csv': get_csv, 'xlsx': get_xlsx}
    return type_dict[format]()
