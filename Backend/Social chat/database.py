from sqlalchemy import Column, Integer, String, Enum, DateTime, ForeignKey, Table, not_, desc, and_, or_
from sqlalchemy.orm import declarative_base, relationship
from application import db


Base = declarative_base()


user_subscribe = Table('user_subscribe', Base.metadata,
    Column('respondent_id', Integer, ForeignKey('user.id')),
    Column('subscriber_id', Integer, ForeignKey('user.id'))
)


class User(Base):
    __tablename__ = 'user'

    id = Column(Integer, primary_key=True)
    username = Column(String(50), nullable=False)
    email = Column(String(100), nullable=False, unique=True)
    password_hash = Column(String(200), nullable=False)
    respondents = relationship('User', secondary=user_subscribe,
                               primaryjoin=user_subscribe.c.subscriber_id==id,
                               secondaryjoin=user_subscribe.c.respondent_id==id,
                               backref="subscribers")

    def __str__(self):
        return self.username

    def get_user(self, id: int):
        return db.query(User).get(id)

    def get_resp_ids(self):
        id_list = [resp.id for resp in self.respondents]
        return id_list

    def get_respondent_posts(self, search_text):
        posts = []
        for resp in self.respondents:
            for post in resp.posts:
                if post.post_type == 'public':
                    if search_text and search_text.strip() and search_text not in post.post:
                        continue
                    posts.append(post)
        return posts

    def get_other_posts(self, search_text):
        ids_list = self.get_resp_ids()
        posts = db.query(Post).filter(
            not_(Post.user_id == self.id)
        ).filter(
            or_(
                and_(
                    Post.user_id.in_(ids_list),
                    Post.post_type == 'private'),
                Post.user_id.notin_(ids_list)
            )
        ).order_by(desc(Post.created_at))
        if search_text and search_text.strip():
            posts = posts.filter(Post.post.ilike(f"%{search_text}%"))
        return posts.all()


class Post(Base):
    __tablename__ = 'post'

    id = Column(Integer(), primary_key=True)
    title = Column(String(100), nullable=False)
    post = Column(String(1000), nullable=False)
    post_type = Column(Enum("private", "public", name="post_type"))
    created_at = Column(DateTime())
    user_id = Column(ForeignKey('user.id', ondelete='CASCADE'))
    user = relationship('User', backref='posts', uselist=False)
