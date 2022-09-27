import os
from concurrent.futures.process import ProcessPoolExecutor
from multiprocessing import cpu_count
from sqlalchemy import create_engine
from sqlalchemy.orm import Session
from werkzeug.security import generate_password_hash
from database.models import Users, Posts


def create_user_and_posts(u_number):
    engine = create_engine(os.environ.get('DATABASE_URL'))
    db = Session(bind=engine)
    mail = f"{u_number}@mail.ru"
    pwd = '123'
    user = Users(username=f'user_{u_number}', email=mail, password=generate_password_hash(pwd))
    db.add(user)
    db.commit()
    for p_number in range(1, 3):
        post_type = 'public'
        if p_number == 2:
            post_type = 'private'
        post = Posts(
            title=f'Пост {p_number} пользователя user_{u_number}',
            post='Просто пост',
            post_type=post_type,
            user_id=user.id,
            )
        db.add(post)
    db.commit()


def multi_create():
    executor = ProcessPoolExecutor(max_workers=cpu_count())
    for u_number in range(1, 10001):
        executor.map(create_user_and_posts, (u_number,))
    executor.shutdown()


if __name__ == '__main__':
    multi_create()

