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


def create_user_and_posts(u_number):
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


def multi_create(users_count, step=50):
    executor = ProcessPoolExecutor(max_workers=cpu_count())
    for u_number in range(1, users_count + 1):
        executor.map(create_user_and_posts, (u_number,))
    executor.shutdown()


if __name__ == '__main__':
    start = time()
    multi_create(1000, step=50)
    print(time() - start)
