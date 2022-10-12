from werkzeug.security import generate_password_hash

from database.base import db
from .models import Users


def get_user_by_id(user_id: int):
    return db.query(Users).get(user_id)


def get_user_by_email(email: str):
    return db.query(Users).filter(Users.email == email).first()


def user_sign_up(username: str, password: str, email: str):
    new_user = Users(username=username, email=email,
                     password=generate_password_hash(password))
    db.add(new_user)
    db.commit()
    return new_user


def user_subscribe(user_id: int, other_user_id: int):
    user = get_user_by_id(user_id)
    respondent = get_user_by_id(other_user_id)
    user.respondents.append(respondent)
    db.commit()


def user_unsubscribe(user_id: int, other_user_id: int):
    user = get_user_by_id(user_id)
    respondent = get_user_by_id(other_user_id)
    user.respondents.remove(respondent)
    db.commit()


def get_subscribers(user: Users):
    return user.subscribers


def get_respondents(user: Users):
    return user.respondents


def get_posts_by_raw(user: Users, search_text: str, offset: int, limit: int):
    ids_list = [user_.id for user_ in get_respondents(user)]
    data_dict = {
        'ids_list': ids_list, 'user_id': user.id, 'limit': limit,
        'offset': offset
    }
    raw = '''
        SELECT * FROM (
            SELECT p.title, p.post, p.post_type, p.created_at, p.user_id, u.username
            FROM posts as p
            JOIN users as u on p.user_id = u.id
            WHERE p.user_id = ANY(:ids_list) AND p.post_type = 'public'
            ORDER BY created_at DESC
            ) a
        UNION ALL
        SELECT * FROM (
            SELECT p.title, p.post, p.post_type, p.created_at, p.user_id, \
            u.username
            FROM posts as p
            JOIN users as u on p.user_id = u.id
            WHERE p.user_id <> :user_id
            AND (
                (p.post_type = 'private' AND p.user_id = ANY(:ids_list))
                OR
                p.user_id <> ALL(:ids_list)
            )
            ORDER BY created_at DESC
        ) b
        OFFSET :offset
        LIMIT :limit
        '''
    if search_text and search_text.strip():
        data_dict.update({'text': f'%{search_text}%'})
        return db.execute(
            f'''
            SELECT * FROM (
                {raw}
            ) d
            WHERE post LIKE :text
            ''', data_dict).all()
    return db.execute(raw, data_dict).all()
