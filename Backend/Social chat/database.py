from sqlalchemy import create_engine, Column, Integer, String, Enum, DateTime, ForeignKey, Table, not_, desc, and_, or_
from sqlalchemy.orm import declarative_base, relationship, sessionmaker


engine = create_engine('postgresql+psycopg2://db_user:1234@localhost:5432/db_test')
Base = declarative_base()
Session = sessionmaker(bind=engine)
db = Session()


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

    def get_user(self, id: int):
        return db.query(User).get(id)

    def get_resp_ids(self):
        id_list = [resp.id for resp in self.respondents]
        return id_list

    def get_respondent_posts(self):
        posts = []
        for resp in self.respondents:
            for post in resp.posts:
                if post.post_type == 'public':
                    posts.append(post)
        return posts

    def get_other_posts(self):
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
        ).order_by(desc(Post.created_at)).all()
        return posts


class Post(Base):
    __tablename__ = 'post'

    id = Column(Integer(), primary_key=True)
    title = Column(String(100), nullable=False)
    post = Column(String(1000), nullable=False)
    post_type = Column(Enum("private", "public", name="status"))
    created_at = Column(DateTime())
    user_id = Column(ForeignKey('user.id', ondelete='CASCADE'))
    user = relationship('User', backref='posts', uselist=False)


# Создание таблиц
Base.metadata.create_all(engine)
