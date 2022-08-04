import click
import os
from picture import check_or_create_dirs, multiple_cutting, thread_cutting, sync_cutting
from time import time


@click.command()
@click.option("--multiprocessing", is_flag=True)
@click.option("--multithreading", is_flag=True)
def program(multiprocessing, multithreading):
    start_time = time()
    check_or_create_dirs()
    files_name = os.listdir("picture_source")
    if multiprocessing and multithreading:
        raise click.UsageError('Use only "--multiprocessing" or "--multithreading"')
    elif multiprocessing:
        multiple_cutting(files_name)
    elif multithreading:
        thread_cutting(files_name)
    else:
        sync_cutting(files_name)
    print(f"Time process: {time() - start_time} seconds")


if __name__ == '__main__':
    program()
