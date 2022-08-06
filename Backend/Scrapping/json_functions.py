import json
import time
from click import UsageError


def save_to_json(data):
    with open("data.json", "w", encoding="utf-8") as file:
        json.dump(data, file, indent=4, ensure_ascii=False)


def get_json():
    try:
        with open(f"data.json", "r") as file:
            data = json.load(file)
            return data
    except:
        raise UsageError(f'No such file: "data.json" or use "--format" to choose another file format')
