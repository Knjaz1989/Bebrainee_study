import cv2
import os
from threading import Thread
from multiprocessing import cpu_count
from concurrent.futures.process import ProcessPoolExecutor


def check_or_create_dirs():
    dir_list = ["large", "medium", "small"]
    for directory in dir_list:
        os.makedirs(directory, exist_ok=True)


def picture_cutting(picture_name, folder, y, x):
    img = cv2.imread(f"picture_source/{picture_name}")
    cv2.imwrite(f"{folder}/{picture_name}", img[0:y, 0:x])


def sync_cutting(files_name):
    for name in files_name:
        picture_cutting(name, "large", 500, 500)
        picture_cutting(name, "medium", 300, 500)
        picture_cutting(name, "small", 50, 50)


def multiple_cutting(files_name):
    proc_count = cpu_count()
    executor = ProcessPoolExecutor(max_workers=proc_count)
    for picture_name in files_name:
        executor.map(picture_cutting, (picture_name, "large", 500, 500))
        executor.map(picture_cutting, (picture_name, "medium", 300, 500))
        executor.map(picture_cutting, (picture_name, "small", 50, 50))
    executor.shutdown()


def thread_cutting(files_name):
    threads = []
    for picture_name in files_name:
        thread_1 = Thread(target=picture_cutting, args=(picture_name, "large", 500, 500))
        thread_2 = Thread(target=picture_cutting, args=(picture_name, "medium", 300, 500))
        thread_3 = Thread(target=picture_cutting, args=(picture_name, "small", 50, 50))
        threads.extend([thread_1, thread_2, thread_3])
        thread_1.start()
        thread_2.start()
        thread_3.start()

    for tread in threads:
        tread.join()
