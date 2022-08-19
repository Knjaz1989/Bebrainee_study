from concurrent.futures.process import ProcessPoolExecutor
from datetime import datetime
from multiprocessing import cpu_count, Pool
from time import time

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from werkzeug.security import generate_password_hash
from database import User, Post


def create_user_and_posts(u_number):
    engine = create_engine('postgresql+psycopg2://db_user:1234@localhost:5432/db_test')
    session = sessionmaker(bind=engine)
    db = session()
    mail = f"{u_number}@mail.ru"
    pwd = '123'
    user = User(username=f'user_{u_number}', email=mail, password_hash=generate_password_hash(pwd))
    db.add(user)
    db.commit()
    for p_number in range(1, 3):
        post_type = 'public'
        if p_number == 2:
            post_type = 'private'
        post = Post(
            title=f'Пост {p_number} пользователя user_{u_number}',
            post='Просто пост',
            post_type=post_type,
            user_id=user.id,
            created_at=datetime.now()
            )
        db.add(post)
    db.commit()


def multi_create():
    executor = ProcessPoolExecutor(max_workers=cpu_count())
    pool = Pool(cpu_count())
    start = time()
    for u_number in range(1, 10001):
        executor.map(create_user_and_posts, (u_number,))
        # create_user_and_posts(u_number)
        # break
    # pool.close()
    # pool.join()
    executor.shutdown()
    print(time() - start)


if __name__ == '__main__':
    multi_create()

