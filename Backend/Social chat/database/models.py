from sqlalchemy import Column, Integer, String, Enum, DateTime, ForeignKey, Table, desc, and_, or_, Index
from sqlalchemy.orm import declarative_base, relationship
from sqlalchemy.sql import func
from werkzeug.security import generate_password_hash
from application import db


Base = declarative_base()


users_subscribes = Table('users_subscribes', Base.metadata,
                         Column('respondent_id', Integer, ForeignKey('users.id')),
                         Column('subscriber_id', Integer, ForeignKey('users.id'))
                         )


class Users(Base):
    __tablename__ = 'users'

    id = Column(Integer, primary_key=True)
    username = Column(String(50), nullable=False)
    email = Column(String(100), nullable=False, unique=True)
    password = Column(String(200), nullable=False)
    posts = relationship('Posts')
    respondents = relationship('Users', secondary=users_subscribes,
                               primaryjoin=users_subscribes.c.subscriber_id == id,
                               secondaryjoin=users_subscribes.c.respondent_id == id,
                               backref="subscribers")

    def __str__(self):
        return self.email

    @classmethod
    def get_by_id(cls, user_id: int):
        return db.query(cls).get(user_id)

    @classmethod
    def get_by_email(cls, email: str):
        return db.query(cls).filter(cls.email == email).first()

    @classmethod
    def user_sign_up(cls, request, email):
        password = request.form.get('password')
        username = request.form.get('name')
        new_user = cls(username=username, email=email, password=generate_password_hash(password))
        db.add(new_user)
        db.commit()

    @classmethod
    def subscribe(cls, request, user_id):
        user = cls.get_by_id(user_id)
        respondent = cls.get_by_id(int(request.form.get('other_user')))
        user.respondents.append(respondent)
        db.commit()

    @classmethod
    def unsubscribe(cls, request, user_id):
        user = cls.get_by_id(user_id)
        respondent = cls.get_by_id(int(request.form.get('other_user')))
        user.respondents.remove(respondent)
        db.commit()

    def get_subscribers(self):
        return self.subscribers

    def get_respondents(self):
        return self.respondents

    def get_posts(self, search_text):
        ids_list = [user.id for user in self.get_respondents()]
        posts_query = db.query(Posts).filter(
            Posts.user_id.in_(ids_list), Posts.post_type == 'public'
        ).order_by(desc(Posts.created_at)).union_all(
            db.query(Posts).filter(
                Posts.user_id != self.id,
                or_(
                    and_(
                        Posts.user_id.in_(ids_list),
                        Posts.post_type == 'private'
                    ),
                    Posts.user_id.notin_(ids_list)
                )
            ).order_by(desc(Posts.created_at))
        )
        if search_text and search_text.strip():
            posts_query = posts_query.filter(Posts.post.ilike(f"%{search_text}%"))
        return posts_query


class Posts(Base):
    __tablename__ = 'posts'

    id = Column(Integer(), primary_key=True)
    title = Column(String(100), nullable=False)
    post = Column(String(1000), nullable=False)
    post_type = Column(Enum("private", "public", name="post_type"), default='public')
    created_at = Column(DateTime(), default=func.now())
    user_id = Column(ForeignKey('users.id', ondelete='CASCADE'), nullable=False)
    user = relationship('Users', back_populates='posts')
    idx_post_type = Index('idx_post_type', post_type)

    def __str__(self):
        return self.title

    @classmethod
    def create_post(cls, request, user_id):
        title = request.form.get('title')
        post = request.form.get('post')
        post_type = request.form.get('post_type')
        new_post = cls(
            title=title, post=post, post_type=post_type, user_id=user_id
        )
        db.add(new_post)
        db.commit()
