from concurrent.futures.process import ProcessPoolExecutor
from multiprocessing import cpu_count, Pool
from time import time

from sqlalchemy import create_engine
from sqlalchemy.orm import Session
from werkzeug.security import generate_password_hash

from utils import config
from database.posts.models import Posts
from database.users.models import Users


engine = create_engine(config.PG_URL)
db = Session(bind=engine)


# option 1 (1000 items per 17 sec, 10000 items per 200 sec)
def create_user_and_posts(u_number):
    start = time()
    user = Users(username=f'user_{u_number}',
                 email=f"{u_number}@mail.ru",
                 password=generate_password_hash('123'))
    db.add(user)
    for number, post_type in enumerate(['public', 'private', 'public']):
        post = Posts(
            title=f'Пост {number + 1} пользователя user_{u_number}',
            post='Просто пост',
            post_type=post_type,
            )
        post.user = user
        db.add(post)
    db.commit()
    print('Время процесса', time() - start)


def multi_create(users_count, step=50):
    executor = ProcessPoolExecutor(max_workers=cpu_count())
    for u_number in range(1, users_count + 1):
        executor.map(create_user_and_posts, (u_number,))
    executor.shutdown()


# option 2 (1000 items per 83 sec)
# def create_user_and_posts(data):
#     start_number = data[0]
#     step = data[1]
#     for u_number in range(start_number, start_number + step):
#         user = Users(username=f'user_{u_number}',
#                      email=f"{u_number}@mail.ru",
#                      password=generate_password_hash('123'))
#         db.add(user)
#         for number, post_type in enumerate(['public', 'private', 'public']):
#             post = Posts(
#                 title=f'Пост {number + 1} пользователя user_{u_number}',
#                 post='Просто пост',
#                 post_type=post_type,
#                 )
#             post.user = user
#             db.add(post)
#     db.commit()
#
#
# def multi_create(users_count, step=50):
#     executor = Pool(cpu_count())
#     for u_number in range(1, users_count + 1, step):
#         data = (u_number, step)
#         executor.map(create_user_and_posts, (data,))


# option 3 (1000 items per 21 sec, 10000 items per 198 sec)
# def create_user_and_posts(data):
#     start = time()
#     start_number = data[0]
#     step = data[1]
#     for u_number in range(start_number, start_number + step):
#         user = Users(username=f'user_{u_number}',
#                      email=f"{u_number}@mail.ru",
#                      password=generate_password_hash('123'))
#         db.add(user)
#         for number, post_type in enumerate(['public', 'private', 'public']):
#             post = Posts(
#                 title=f'Пост {number + 1} пользователя user_{u_number}',
#                 post='Просто пост',
#                 post_type=post_type,
#                 )
#             post.user = user
#             db.add(post)
#     db.commit()
#     print('Время процесса', time() - start)
#
#
# def multi_create(users_count, step=50):
#     executor = ProcessPoolExecutor(max_workers=cpu_count())
#     for u_number in range(1, users_count + 1, step):
#         data = (u_number, step)
#         executor.map(create_user_and_posts, (data,))
#     executor.shutdown()


if __name__ == '__main__':
    start = time()
    multi_create(1600, step=50)
    print(time() - start)
