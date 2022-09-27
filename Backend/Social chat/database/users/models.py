from sqlalchemy import Column, Integer, String, ForeignKey, Table, \
    desc, and_, or_
from sqlalchemy.orm import declarative_base, relationship
from werkzeug.security import generate_password_hash

from database.base import db


UserBase = declarative_base()


users_subscribes = Table('users_subscribes', UserBase.metadata,
                         Column('respondent_id', Integer,
                                ForeignKey('users.id')),
                         Column('subscriber_id', Integer,
                                ForeignKey('users.id'))
                         )


class Users(UserBase):
    __tablename__ = 'users'

    id = Column(Integer, primary_key=True)
    username = Column(String(50), nullable=False)
    email = Column(String(100), nullable=False, unique=True)
    password = Column(String(200), nullable=False)
    respondents = relationship(
        'Users', secondary=users_subscribes,
        primaryjoin=users_subscribes.c.subscriber_id == id,
        secondaryjoin=users_subscribes.c.respondent_id == id,
        backref="subscribers"
    )

    def __str__(self):
        return self.email

    @classmethod
    def get_by_id(cls, user_id: int):
        return db.query(cls).get(user_id)

    @classmethod
    def get_by_email(cls, email: str):
        return db.query(cls).filter(cls.email == email).first()

    @classmethod
    def user_sign_up(cls, username: str, password: str, email: str):
        new_user = cls(username=username, email=email,
                       password=generate_password_hash(password))
        db.add(new_user)
        db.commit()
        return new_user

    @classmethod
    def subscribe(cls, user_id: int, other_user_id: int):
        user = cls.get_by_id(user_id)
        respondent = cls.get_by_id(other_user_id)
        user.respondents.append(respondent)
        db.commit()

    @classmethod
    def unsubscribe(cls, user_id: int, other_user_id: int):
        user = cls.get_by_id(user_id)
        respondent = cls.get_by_id(other_user_id)
        user.respondents.remove(respondent)
        db.commit()

    def get_subscribers(self):
        return self.subscribers

    def get_respondents(self):
        return self.respondents

    def get_posts(self, search_text: str):
        ids_list = [user.id for user in self.get_respondents()]
        # raw
        raw = '''
            SELECT * FROM (
                SELECT p.title, p.post, p.post_type, p.created_at, p.user_id, \
                u.username 
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
            '''
        if search_text and search_text.strip():
            return db.execute(
                f'''
                SELECT * FROM (
                    {raw}
                ) d
                WHERE post LIKE :text
                ''', {'text': f'%{search_text}%', 'ids_list': ids_list,
                      'user_id': self.id}
            )
        return db.execute(raw, {'ids_list': ids_list, 'user_id': self.id})
        # ORM
        # posts_ = db.query(Posts).filter(
        #     Posts.user_id.in_(ids_list), Posts.post_type == 'public'
        # ).order_by(desc(Posts.created_at)).union_all(
        #     db.query(Posts).filter(
        #         Posts.user_id != self.id,
        #         or_(
        #             and_(
        #                 Posts.user_id.in_(ids_list),
        #                 Posts.post_type == 'private'
        #             ),
        #             Posts.user_id.notin_(ids_list)
        #         )
        #     ).order_by(desc(Posts.created_at))
        # )
        # if search_text and search_text.strip():
        #     posts_query = posts_query.filter(Posts.post.ilike(f"%{search_text}%"))
        # return posts_
