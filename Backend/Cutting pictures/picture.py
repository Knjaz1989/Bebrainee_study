import cv2
import os
from threading import Thread
from multiprocessing import Process

# print("Number of cpu : ", multiprocessing.cpu_count())


def check_or_create_dirs():
    dir_list = ["large", "medium", "small"]
    for directory in dir_list:
        os.makedirs(directory, exist_ok=True)


def picture_cutting(picture_name):
    img = cv2.imread(f"picture_source/{picture_name}")
    cv2.imwrite(f"large/{picture_name}", img[0:500, 0:500])
    cv2.imwrite(f"medium/{picture_name}", img[0:500, 0:300])
    cv2.imwrite(f"small/{picture_name}", img[0:50, 0:50])


def sync_cutting(files_name):
    for name in files_name:
        picture_cutting(name)


def multiple_cutting(files_name):
    procs = []
    for picture_name in files_name:
        proc = Process(target=picture_cutting, args=(picture_name,))
        procs.append(proc)
        proc.start()

    # complete the processes
    for proc in procs:
        proc.join()


def thread_cutting(files_name):
    for picture_name in files_name:
        thread = Thread(target=picture_cutting, args=(picture_name,))
        thread.start()
